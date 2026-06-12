// Betaflight SITL_LOCKSTEP multi-instance IR transform, layout edition.
//
// Input: the whole firmware llvm-linked into one module (every TU except
// the harness). The pass does the linker's job for mutable state so that
// no ELF machinery (linker scripts, --emit-relocs, /proc/self/exe) is
// needed at runtime — a hard requirement for the GPU backend, where the
// module is codegenned for NVPTX and instances live in device memory.
//
//  - Every defined non-constant global (plus, transitively, any constant
//    whose initializer points into mutable state, e.g. the PG registry)
//    is packed into one synthetic global @__bf_image at a fixed offset.
//  - .pg_registry entries are placed first and contiguously;
//    __pg_registry_start/end and __pg_resetdata_start/end (normally
//    linker-script symbols) are defined by the pass itself.
//  - Every access is rewritten to image+offset+delta, where delta comes
//    from one call to i64 @__bf_delta_load() per function (CPU glue
//    returns a global set by the instance manager; GPU glue derives it
//    from the thread index — both inline away after linking).
//  - Pointer slots inside the template initializer that point back into
//    the image are emitted as a {loc, target} table (@__bf_relocs) so a
//    backend-agnostic runtime can rebase each instance copy, exactly
//    what a program loader would have done.
//
// Firmware source is untouched; only this tool and the glue know about
// instancing.
//
// Usage: instancer <in.bc> <out.bc>

#include "llvm/ADT/SetVector.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ReplaceConstant.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/Alignment.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

using namespace llvm;

static bool isMeta(const GlobalVariable &GV)
{
    return GV.getName().starts_with("llvm.");
}

static bool initializerRefs(const Constant *C,
                            function_ref<bool(const GlobalValue *)> isTarget,
                            std::set<const Constant *> &visited)
{
    if (!visited.insert(C).second) {
        return false;
    }
    if (auto *GV = dyn_cast<GlobalValue>(C)) {
        return isTarget(GV);
    }
    for (const Use &op : C->operands()) {
        if (auto *opC = dyn_cast<Constant>(op)) {
            if (initializerRefs(opC, isTarget, visited)) {
                return true;
            }
        }
    }
    return false;
}

// Pack the given globals (in order) into one struct, inserting i8-array
// padding so each lands at its required alignment. Returns the new global;
// offsets are reported through the map.
static GlobalVariable *packImage(Module &M, ArrayRef<GlobalVariable *> parts,
                                 StringRef name, bool constant,
                                 std::map<GlobalVariable *, uint64_t> &offsets,
                                 uint64_t &sizeOut, Align &alignOut)
{
    LLVMContext &ctx = M.getContext();
    const DataLayout &DL = M.getDataLayout();
    Type *i8 = Type::getInt8Ty(ctx);

    std::vector<Type *> types;
    std::vector<Constant *> inits;
    uint64_t off = 0;
    Align maxAlign(16);

    for (GlobalVariable *GV : parts) {
        Constant *init = GV->getInitializer();
        const Align a = DL.getValueOrABITypeAlignment(GV->getAlign(), init->getType());
        maxAlign = std::max(maxAlign, a);
        const uint64_t aligned = alignTo(off, a);
        if (aligned != off) {
            auto *padTy = ArrayType::get(i8, aligned - off);
            types.push_back(padTy);
            inits.push_back(ConstantAggregateZero::get(padTy));
            off = aligned;
        }
        offsets[GV] = off;
        types.push_back(init->getType());
        inits.push_back(init);
        off += DL.getTypeAllocSize(init->getType());
    }

    auto *ty = StructType::create(ctx, types, (name + "_t").str(), /*packed=*/true);
    auto *image = new GlobalVariable(M, ty, constant, GlobalValue::ExternalLinkage,
                                     ConstantStruct::get(ty, inits), name);
    image->setAlignment(maxAlign);
    if (DL.getTypeAllocSize(ty) != off) {
        report_fatal_error("packImage: layout size mismatch");
    }
    sizeOut = off;
    alignOut = maxAlign;
    return image;
}

// Recursively walk a template initializer recording every pointer-sized
// slot that points back into the image (these need per-instance rebasing).
static void scanRelocs(const DataLayout &DL, const GlobalVariable *image,
                       const Constant *C, uint64_t off,
                       std::vector<std::pair<uint64_t, uint64_t>> &relocs)
{
    if (isa<ConstantAggregateZero>(C) || isa<UndefValue>(C) ||
        isa<ConstantDataSequential>(C)) {
        return; // no pointers inside
    }
    if (auto *CS = dyn_cast<ConstantStruct>(C)) {
        const StructLayout *SL = DL.getStructLayout(CS->getType());
        for (unsigned i = 0; i < CS->getNumOperands(); i++) {
            scanRelocs(DL, image, CS->getOperand(i), off + SL->getElementOffset(i), relocs);
        }
        return;
    }
    if (auto *CA = dyn_cast<ConstantArray>(C)) {
        const uint64_t stride = DL.getTypeAllocSize(CA->getType()->getElementType());
        for (unsigned i = 0; i < CA->getNumOperands(); i++) {
            scanRelocs(DL, image, CA->getOperand(i), off + i * stride, relocs);
        }
        return;
    }
    if (C->getType()->isPointerTy()) {
        APInt delta(DL.getIndexTypeSizeInBits(C->getType()), 0);
        const Value *base = C->stripAndAccumulateConstantOffsets(DL, delta,
                                                                 /*AllowNonInbounds=*/true);
        if (base == image) {
            relocs.push_back({off, delta.getZExtValue()});
        }
        // other bases (functions, shared rodata, null) stay absolute
        return;
    }
    // Scalar int/fp leaves carry no addresses unless someone ptrtoint'd
    // state into an integer initializer; that would be silently wrong, so
    // refuse loudly.
    if (auto *CE = dyn_cast<ConstantExpr>(C)) {
        std::set<const Constant *> visited;
        if (initializerRefs(CE, [&](const GlobalValue *GV) { return GV == image; }, visited)) {
            report_fatal_error("scanRelocs: non-pointer initializer references the image");
        }
    }
}

// Replace every in-image pointer slot with null. The slots are covered by
// the reloc table and written at runtime (template fixup + per-instance
// rebase), and a self-referential initializer is something the NVPTX
// backend cannot emit (PTX initializers must be acyclic).
static Constant *stripSelfRefs(const DataLayout &DL, const GlobalVariable *image, Constant *C)
{
    if (isa<ConstantAggregateZero>(C) || isa<UndefValue>(C) ||
        isa<ConstantDataSequential>(C)) {
        return C;
    }
    if (auto *CS = dyn_cast<ConstantStruct>(C)) {
        std::vector<Constant *> elems;
        bool changed = false;
        for (unsigned i = 0; i < CS->getNumOperands(); i++) {
            Constant *e = stripSelfRefs(DL, image, CS->getOperand(i));
            changed |= (e != CS->getOperand(i));
            elems.push_back(e);
        }
        return changed ? ConstantStruct::get(CS->getType(), elems) : C;
    }
    if (auto *CA = dyn_cast<ConstantArray>(C)) {
        std::vector<Constant *> elems;
        bool changed = false;
        for (unsigned i = 0; i < CA->getNumOperands(); i++) {
            Constant *e = stripSelfRefs(DL, image, CA->getOperand(i));
            changed |= (e != CA->getOperand(i));
            elems.push_back(e);
        }
        return changed ? ConstantArray::get(CA->getType(), elems) : C;
    }
    if (C->getType()->isPointerTy()) {
        APInt delta(DL.getIndexTypeSizeInBits(C->getType()), 0);
        const Value *base = C->stripAndAccumulateConstantOffsets(DL, delta,
                                                                 /*AllowNonInbounds=*/true);
        if (base == image) {
            return ConstantPointerNull::get(cast<PointerType>(C->getType()));
        }
    }
    return C;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        errs() << "usage: " << argv[0] << " <in.bc> <out.bc>\n";
        return 1;
    }

    LLVMContext ctx;
    SMDiagnostic err;
    std::unique_ptr<Module> M = parseIRFile(argv[1], err, ctx);
    if (!M) {
        err.print(argv[0], errs());
        return 1;
    }
    const DataLayout &DL = M->getDataLayout();
    Type *i64 = Type::getInt64Ty(ctx);
    Type *i8 = Type::getInt8Ty(ctx);

    // ------------------------------------------------------------------
    // Classify. SetVector keeps module order, which keeps the layout (and
    // therefore the build) deterministic.
    // ------------------------------------------------------------------
    SetVector<GlobalVariable *> inst;
    for (GlobalVariable &GV : M->globals()) {
        if (isMeta(GV) || !GV.hasInitializer()) {
            continue;
        }
        if (!GV.isConstant() || GV.getSection() == ".pg_registry") {
            inst.insert(&GV);
        }
    }
    bool changed = true;
    while (changed) {
        changed = false;
        for (GlobalVariable &GV : M->globals()) {
            if (isMeta(GV) || !GV.hasInitializer() || inst.count(&GV)) {
                continue;
            }
            std::set<const Constant *> visited;
            if (initializerRefs(GV.getInitializer(),
                                [&](const GlobalValue *T) {
                                    return isa<GlobalVariable>(T) &&
                                           inst.count(cast<GlobalVariable>(const_cast<GlobalValue *>(T)));
                                },
                                visited)) {
                if (GV.getSection() == ".pg_resetdata") {
                    errs() << "instancer: resetdata " << GV.getName()
                           << " references mutable state\n";
                    return 1;
                }
                inst.insert(&GV);
                changed = true;
            }
        }
    }

    std::vector<GlobalVariable *> resetdata;
    for (GlobalVariable &GV : M->globals()) {
        if (!isMeta(GV) && GV.hasInitializer() && GV.getSection() == ".pg_resetdata") {
            resetdata.push_back(&GV);
        }
    }

    // ------------------------------------------------------------------
    // Order: registry entries first (contiguous, so start/end iteration
    // works), then everything else by decreasing alignment to minimise
    // padding. stable_sort keeps module order within a class.
    // ------------------------------------------------------------------
    std::vector<GlobalVariable *> ordered(inst.begin(), inst.end());
    auto alignOf = [&](GlobalVariable *GV) {
        return DL.getValueOrABITypeAlignment(GV->getAlign(), GV->getInitializer()->getType());
    };
    std::stable_sort(ordered.begin(), ordered.end(),
                     [&](GlobalVariable *A, GlobalVariable *B) {
                         const bool ra = A->getSection() == ".pg_registry";
                         const bool rb = B->getSection() == ".pg_registry";
                         if (ra != rb) {
                             return ra;
                         }
                         if (ra) {
                             return false; // registry: keep module order
                         }
                         return alignOf(A) > alignOf(B);
                     });

    uint64_t regBytes = 0;
    unsigned regCount = 0;
    for (GlobalVariable *GV : ordered) {
        if (GV->getSection() == ".pg_registry") {
            regBytes += DL.getTypeAllocSize(GV->getInitializer()->getType());
            regCount++;
        }
    }

    // ------------------------------------------------------------------
    // Pack the mutable image and the shared resetdata blob.
    // ------------------------------------------------------------------
    std::map<GlobalVariable *, uint64_t> imgOff, rstOff;
    uint64_t imgSize = 0, rstSize = 0;
    Align imgAlign(16), rstAlign(16);
    GlobalVariable *image = packImage(*M, ordered, "__bf_image", /*constant=*/false,
                                      imgOff, imgSize, imgAlign);
    GlobalVariable *rstImage = packImage(*M, resetdata, "__bf_resetdata_image",
                                         /*constant=*/true, rstOff, rstSize, rstAlign);

    // The registry must be iterable as a dense array.
    if (regBytes && imgOff.size()) {
        uint64_t end = 0;
        for (GlobalVariable *GV : ordered) {
            if (GV->getSection() != ".pg_registry") {
                continue;
            }
            if (imgOff[GV] != end) {
                errs() << "instancer: pg_registry not contiguous at " << GV->getName() << "\n";
                return 1;
            }
            end = imgOff[GV] + DL.getTypeAllocSize(GV->getInitializer()->getType());
        }
        if (end != regBytes) {
            errs() << "instancer: pg_registry layout hole\n";
            return 1;
        }
    }

    // ------------------------------------------------------------------
    // The packed members live on inside the images; drop them from the
    // llvm.used keep-alive lists before replacing them.
    // ------------------------------------------------------------------
    removeFromUsedLists(*M, [&](Constant *C) {
        auto *GV = dyn_cast<GlobalVariable>(C->stripPointerCasts());
        return GV && (imgOff.count(GV) || rstOff.count(GV));
    });

    auto gep = [&](GlobalVariable *base, uint64_t off) -> Constant * {
        return ConstantExpr::getGetElementPtr(i8, base, ConstantInt::get(i64, off),
                                              /*InBounds=*/true);
    };

    // Aliases of packed globals are flattened to their aliasee.
    for (GlobalAlias &GA : make_early_inc_range(M->aliases())) {
        std::set<const Constant *> visited;
        if (initializerRefs(GA.getAliasee(),
                            [&](const GlobalValue *T) {
                                auto *GV = dyn_cast<GlobalVariable>(const_cast<GlobalValue *>(T));
                                return GV && (imgOff.count(GV) || rstOff.count(GV));
                            },
                            visited)) {
            GA.replaceAllUsesWith(GA.getAliasee());
            GA.eraseFromParent();
        }
    }

    for (auto &kv : imgOff) {
        kv.first->replaceAllUsesWith(gep(image, kv.second));
    }
    for (auto &kv : rstOff) {
        kv.first->replaceAllUsesWith(gep(rstImage, kv.second));
    }
    for (auto &kv : imgOff) {
        kv.first->eraseFromParent();
    }
    for (auto &kv : rstOff) {
        kv.first->eraseFromParent();
    }

    // Linker-script symbols, now defined by the pass. Registry bounds point
    // into the image (offset 0 by construction) and so get the +delta
    // rewrite below; resetdata is shared and stays absolute.
    struct BoundDef { const char *name; GlobalVariable *base; uint64_t off; };
    const BoundDef bounds[] = {
        { "__pg_registry_start", image, 0 },
        { "__pg_registry_end", image, regBytes },
        { "__pg_resetdata_start", rstImage, 0 },
        { "__pg_resetdata_end", rstImage, rstSize },
    };
    for (const BoundDef &b : bounds) {
        if (GlobalVariable *GV = M->getNamedGlobal(b.name)) {
            if (GV->hasInitializer()) {
                errs() << "instancer: " << b.name << " unexpectedly defined\n";
                return 1;
            }
            GV->replaceAllUsesWith(gep(b.base, b.off));
            GV->eraseFromParent();
        }
    }

    // ------------------------------------------------------------------
    // Reloc table: template-relative pointer slots inside the image.
    // ------------------------------------------------------------------
    std::vector<std::pair<uint64_t, uint64_t>> relocs;
    scanRelocs(DL, image, image->getInitializer(), 0, relocs);
    image->setInitializer(stripSelfRefs(DL, image, image->getInitializer()));
    {
        std::set<const Constant *> visited;
        if (initializerRefs(rstImage->getInitializer(),
                            [&](const GlobalValue *T) { return T == image; }, visited)) {
            errs() << "instancer: resetdata references the mutable image\n";
            return 1;
        }
    }
    // No other surviving initializer may reference the image: anything that
    // does should have been classified into it.
    for (GlobalVariable &GV : M->globals()) {
        if (&GV == image || isMeta(GV) || !GV.hasInitializer()) {
            continue;
        }
        std::set<const Constant *> visited;
        if (initializerRefs(GV.getInitializer(),
                            [&](const GlobalValue *T) { return T == image; }, visited)) {
            errs() << "instancer: shared global " << GV.getName()
                   << " references the mutable image\n";
            return 1;
        }
    }

    auto emitConstI64 = [&](StringRef name, uint64_t v) {
        auto *g = new GlobalVariable(*M, i64, /*constant=*/true,
                                     GlobalValue::ExternalLinkage,
                                     ConstantInt::get(i64, v), name);
        g->setAlignment(Align(8));
    };
    {
        auto *pairTy = StructType::get(i64, i64);
        std::vector<Constant *> entries;
        for (auto &r : relocs) {
            entries.push_back(ConstantStruct::get(
                cast<StructType>(pairTy),
                {ConstantInt::get(i64, r.first), ConstantInt::get(i64, r.second)}));
        }
        auto *arrTy = ArrayType::get(pairTy, entries.size());
        auto *g = new GlobalVariable(*M, arrTy, /*constant=*/true,
                                     GlobalValue::ExternalLinkage,
                                     ConstantArray::get(arrTy, entries), "__bf_relocs");
        g->setAlignment(Align(8));
    }
    emitConstI64("__bf_reloc_count", relocs.size());
    emitConstI64("__bf_image_size", imgSize);
    emitConstI64("__bf_image_align", imgAlign.value());
    {
        Type *i32 = Type::getInt32Ty(ctx);
        auto *marker = new GlobalVariable(*M, i32, /*constant=*/true,
                                          GlobalValue::ExternalLinkage,
                                          ConstantInt::get(i32, 1), "__bf_instanced_build");
        marker->setAlignment(Align(4));
    }

    // Section attributes only made sense to the ELF linker; they confuse
    // GPU backends and are now meaningless.
    for (Function &F : *M) {
        if (F.hasSection()) {
            F.setSection("");
        }
    }
    for (GlobalVariable &GV : M->globals()) {
        if (!isMeta(GV) && GV.hasSection()) {
            GV.setSection("");
        }
    }

    // ------------------------------------------------------------------
    // The +delta rewrite. After the RAUW above, every mutable-state access
    // goes through @__bf_image, so one rebased base pointer per function
    // covers everything.
    // ------------------------------------------------------------------
    auto deltaLoad = M->getOrInsertFunction("__bf_delta_load",
                                            FunctionType::get(i64, /*isVarArg=*/false));
    if (auto *F = dyn_cast<Function>(deltaLoad.getCallee())) {
        F->setDoesNotThrow();
        F->setWillReturn();
    }

    convertUsersOfConstantsToInstructions({image});

    std::map<Function *, Instruction *> rebasedIn;
    uint64_t rewrittenUses = 0;
    std::vector<Use *> uses;
    for (Use &U : image->uses()) {
        uses.push_back(&U);
    }
    for (Use *U : uses) {
        auto *I = dyn_cast<Instruction>(U->getUser());
        if (!I) {
            continue; // the image's own initializer: template-relative,
                      // fixed per instance via @__bf_relocs
        }
        Function *F = I->getFunction();
        Instruction *&rebased = rebasedIn[F];
        if (!rebased) {
            IRBuilder<> B(&*F->getEntryBlock().getFirstInsertionPt());
            Value *delta = B.CreateCall(deltaLoad, {}, "bf.delta");
            rebased = cast<Instruction>(B.CreateGEP(i8, image, delta, "bf.image.inst"));
        }
        U->set(rebased);
        rewrittenUses++;
    }

    if (verifyModule(*M, &errs())) {
        errs() << "instancer: verification failed\n";
        return 1;
    }

    std::error_code ec;
    raw_fd_ostream out(argv[2], ec, sys::fs::OF_None);
    if (ec) {
        errs() << "instancer: cannot write " << argv[2] << ": " << ec.message() << "\n";
        return 1;
    }
    WriteBitcodeToFile(*M, out);

    outs() << "instancer: " << imgOff.size() << " globals packed into "
           << imgSize << "B image (align " << imgAlign.value()
           << ", registry " << regCount << "/" << regBytes << "B"
           << ", resetdata " << rstSize << "B shared), "
           << relocs.size() << " relocs, "
           << rewrittenUses << " uses rewritten in " << rebasedIn.size()
           << " functions\n";
    return 0;
}
