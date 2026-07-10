// cudaflight: C shared library exposing the GPU Betaflight fleet as an RL
// environment. cudaflight_create() boots, settles and arms N firmware
// instances and snapshots them as the episode start state; cudaflight_step()
// and the reset calls then operate entirely on device buffers whose
// pointers are exported, so a Python wrapper can map actions / obs /
// rewards / dones as zero-copy CUDA tensors.
//
// Runs in the CUDA *primary* context (cuDevicePrimaryCtxRetain) so the
// buffers live in the same context PyTorch uses. Every entry point is
// synchronous — the context is synchronized before returning — so callers
// may touch the buffers from any stream once a call returns.
//
// Build: g++ -O2 -shared -fPIC cudaflight.cpp -I/opt/cuda/include -lcuda

#include <cuda.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

static char g_err[512];

static bool cuOk(CUresult rc, const char *what)
{
    if (rc == CUDA_SUCCESS) {
        return true;
    }
    const char *msg = nullptr;
    cuGetErrorString(rc, &msg);
    snprintf(g_err, sizeof(g_err), "%s: %s (%d)", what, msg ? msg : "?", (int)rc);
    return false;
}

#define CUTRY(call) \
    do { \
        if (!cuOk((call), #call)) { \
            return -1; \
        } \
    } while (0)

struct cudaflight {
    CUdevice dev;
    CUcontext ctx;          // primary context, retained
    CUmodule mod;
    CUfunction fInit, fBoot, fRun, fFinish, fSnapshot, fReset, fStep, fFwStep;
    CUfunction fOsd, fGradFD, fRateEval, fJacFD, fSetBase, fGrad;
    CUfunction fJacFDPure, fJacGradPure;  // value-threaded (rebase-aware) Jacobians
    CUfunction fSetAux;             // host-driven AUX RC channels (optional)
    uint32_t n;
    unsigned grid, block;
    uint64_t imageSize, stride, stateSize, actDim, obsDim, auxDim;
    uint64_t osdRows, osdCols;
    CUdeviceptr instBuf, stateBuf, snapBuf, snapStBuf;
    CUdeviceptr actBuf, obsBuf, rewBuf, doneBuf;
    CUdeviceptr hashBuf, altBuf, armedBuf;
    CUdeviceptr sensBuf, motorBuf;  // external-physics exchange: [n x 7] in, [n x 4] out
    CUdeviceptr auxBuf;             // host-driven AUX RC channels: [n x auxDim] us floats
    CUdeviceptr osdBuf, osdAttrBuf; // OSD char grids: [n x rows*cols] u8 each
    CUdeviceptr seedBuf, dactBuf;   // FD gradient: [n x 4] motor cotangent in, [n x 4] action grad out
    CUdeviceptr gradScratch;        // [n x stride] per-instance blob save for FD restore
    CUdeviceptr fullRelocBuf;       // complete {loc, targetOff} reloc table (static + runtime)
    uint64_t fullRelocCount;        // entries in fullRelocBuf (0 if not discovered)
};

// The primary context may not be current on the calling thread (or torch
// may have it current already — pushing the same context again is fine).
struct CtxGuard {
    explicit CtxGuard(CUcontext c) { cuCtxPushCurrent(c); }
    ~CtxGuard()
    {
        CUcontext dummy;
        cuCtxPopCurrent(&dummy);
    }
};

static int launch(cudaflight *g, CUfunction f, void **args)
{
    // Entry fence: JAX/XLA (and torch on non-default streams) enqueue
    // reads of our buffers on non-blocking streams, which the NULL
    // stream does not implicitly order against.
    CUTRY(cuCtxSynchronize());
    CUTRY(cuLaunchKernel(f, g->grid, 1, 1, g->block, 1, 1, 0, 0, args, nullptr));
    CUTRY(cuCtxSynchronize());
    return 0;
}

// Chunked so long sim stretches survive display-GPU kernel watchdogs.
static int runMs(cudaflight *g, unsigned ms)
{
    const unsigned chunk = 250;
    for (unsigned done = 0; done < ms;) {
        unsigned step = ms - done < chunk ? ms - done : chunk;
        void *args[] = { &g->stateBuf, &step };
        if (launch(g, g->fRun, args)) {
            return -1;
        }
        done += step;
    }
    return 0;
}

static uint64_t readU64(cudaflight *g, const char *name, int *err)
{
    CUdeviceptr p;
    size_t sz;
    uint64_t v = 0;
    if (!cuOk(cuModuleGetGlobal(&p, &sz, g->mod, name), name) ||
        !cuOk(cuMemcpyDtoH(&v, p, sizeof(v)), name)) {
        *err = 1;
    }
    return v;
}

static int writeGlobal(cudaflight *g, const char *name, const void *src, size_t len)
{
    CUdeviceptr p;
    size_t sz;
    CUTRY(cuModuleGetGlobal(&p, &sz, g->mod, name));
    CUTRY(cuMemcpyHtoD(p, src, len));
    return 0;
}

// ---------------------------------------------------------------------------
// Complete relocation discovery.
//
// __bf_relocs (emitted by the instancer) covers only pointer slots present in
// the compile-time template initializer. The firmware writes MORE self-pointers
// at boot (scheduler queue, currentPidProfile, ...) that are absent from that
// table — which is exactly why a *booted* blob is position-DEPENDENT: a snapshot
// is only valid restored to its original address (see bfReset/bfSnapshot).
//
// We recover the complete set empirically. Instances boot deterministically
// identical except for their self-pointers, so a pointer-sized word at blob
// offset `o` is a self-pointer iff its value across instances forms an
// arithmetic progression with common difference == stride:
//     word_k(o) = instBuf + k*stride + targetOff.
// Diffing instances 0/1 finds candidates; instance 2 cross-validates exactly.
// A non-pointer instance-divergent byte (e.g. the per-instance OSD craft name)
// does NOT form such a progression, so it is neither found nor falsely flagged.
// The result is a complete {loc, targetOff} table — same layout/semantics as
// __bf_relocs — usable to rebase a booted blob to ANY address.
static int discoverRelocs(cudaflight *g)
{
    if (g->n < 3) {
        // No skip-and-continue here: without the full table, rebase-on-move is a
        // silent no-op and value-threaded blobs lose every runtime-written
        // self-pointer (the gyro/acc dataReady handshake among them) — the fleet
        // boots fine but rate feedback is dead. Refuse creation instead.
        snprintf(g_err, sizeof(g_err),
                 "fleet size %u is too small for cudaflight: discovering the runtime "
                 "relocation table needs >=3 identically-booted instances, and without "
                 "it firmware state cannot be rebased (sensors silently stop reaching "
                 "the PID). For small fleets use the bundled CPU SITL backend "
                 "(cudaflight.lib.load_cpu / libcpuflight.so) — it is faster than the "
                 "GPU at this fleet size anyway.", g->n);
        fprintf(stderr, "[cudaflight-reloc] FAIL: %s\n", g_err);
        return -1;
    }
    const uint64_t stride = g->stride;
    const uint64_t base = (uint64_t)g->instBuf;
    const uint64_t words = g->imageSize / 8;
    // The cuMemcpyDtoH below copies imageSize bytes; size the host buffers to
    // hold that many (ceil to whole words) so a non-8-multiple imageSize does
    // not overflow the vector and corrupt the host heap. Only the first `words`
    // whole 8-byte words are scanned as candidate pointers.
    const uint64_t cap = (g->imageSize + 7) / 8;

    std::vector<uint64_t> b0(cap), b1(cap), b2(cap);
    if (!cuOk(cuMemcpyDtoH(b0.data(), g->snapBuf + 0 * stride, g->imageSize), "reloc read 0") ||
        !cuOk(cuMemcpyDtoH(b1.data(), g->snapBuf + 1 * stride, g->imageSize), "reloc read 1") ||
        !cuOk(cuMemcpyDtoH(b2.data(), g->snapBuf + 2 * stride, g->imageSize), "reloc read 2")) {
        return -1;
    }

    std::vector<std::pair<uint64_t, uint64_t>> table; // {loc bytes, targetOff}
    uint64_t inconsistent = 0;
    for (uint64_t w = 0; w < words; w++) {
        const bool found = (b1[w] - b0[w] == stride);       // candidate from 0/1
        const bool confirm = (b2[w] - b0[w] == 2 * stride); // 3rd-instance check
        if (found && confirm) {
            table.push_back({ w * 8, b0[w] - base });       // inst0 ptr -> offset
        } else if (found || confirm) {
            inconsistent++; // looks like a pointer in one pair but not the other
        }
    }

    int err = 0;
    const uint64_t staticCount = readU64(g, "__bf_reloc_count", &err);
    printf("[cudaflight-reloc] discovered %zu self-pointers (static table had %llu; "
           "%lld runtime-written recovered), %llu inconsistent words\n",
           table.size(), (unsigned long long)staticCount,
           (long long)table.size() - (long long)(err ? 0 : staticCount),
           (unsigned long long)inconsistent);
    if (inconsistent) {
        // Any inconsistency means the arithmetic-progression assumption was
        // violated for some word — discovery cannot be trusted as complete.
        snprintf(g_err, sizeof(g_err),
                 "reloc discovery found %llu inconsistent words; table not complete",
                 (unsigned long long)inconsistent);
        fprintf(stderr, "[cudaflight-reloc] FAIL: %s\n", g_err);
        return -1;
    }

    // Upload the complete table for later use (rebase-on-move).
    g->fullRelocCount = table.size();
    if (table.empty()) {
        return 0;
    }
    if (!cuOk(cuMemAlloc(&g->fullRelocBuf, 16 * table.size()), "alloc full relocs") ||
        !cuOk(cuMemcpyHtoD(g->fullRelocBuf, table.data(), 16 * table.size()),
              "upload full relocs")) {
        return -1;
    }
    // Publish the table to the device so the kernels' rebase-on-move can find it.
    if (writeGlobal(g, "__bf_full_relocs", &g->fullRelocBuf, sizeof(g->fullRelocBuf)) ||
        writeGlobal(g, "__bf_full_reloc_count", &g->fullRelocCount, sizeof(g->fullRelocCount))) {
        return -1;
    }
    printf("[cudaflight-reloc] PASS: complete reloc table validated across 3 instances\n");
    return 0;
}

extern "C" {

const char *cudaflight_error(void)
{
    return g_err;
}

void cudaflight_destroy(cudaflight *g)
{
    if (!g) {
        return;
    }
    cuCtxPushCurrent(g->ctx);
    for (CUdeviceptr p : { g->instBuf, g->stateBuf, g->snapBuf, g->snapStBuf,
                           g->actBuf, g->obsBuf, g->rewBuf, g->doneBuf,
                           g->hashBuf, g->altBuf, g->armedBuf,
                           g->sensBuf, g->motorBuf, g->osdBuf, g->osdAttrBuf,
                           g->seedBuf, g->dactBuf, g->gradScratch, g->fullRelocBuf,
                           g->auxBuf }) {
        if (p) {
            cuMemFree(p);
        }
    }
    if (g->mod) {
        cuModuleUnload(g->mod);
    }
    CUcontext dummy;
    cuCtxPopCurrent(&dummy);
    cuDevicePrimaryCtxRelease(g->dev);
    delete g;
}

// Boot, settle and arm n instances, snapshot the armed state as the
// episode start. settle_ms == 0 means the default 7000 (6s gyro
// calibration + arming grace, 1s armed on the ground), matching the
// validated harness schedule. eeprom_path optionally names a boot-ready
// config image (from the CPU harness's --cli-dump converter) preloaded
// into every instance's RAM EEPROM before boot, so the fleet flies that
// configuration instead of defaults. Returns NULL on failure (see
// cudaflight_error).
// Full create. with_grad allocates the differentiable-rollout scratch buffers
// (seedBuf/dactBuf, and crucially gradScratch = stride*n — a per-instance blob
// save-slot as large as the entire instance array). PPO does NOT use them, so
// passing with_grad=0 nearly halves the dominant per-instance memory and lets a
// far larger fleet fit. The plain cudaflight_create_eeprom() wrapper keeps with_grad=1
// for ABI compatibility (grad harnesses / the differentiable env).
cudaflight *cudaflight_create_eeprom_ex(const char *cubin_path, uint32_t n, int device,
                              uint32_t settle_ms, const char *eeprom_path,
                              int with_grad)
{
    if (settle_ms == 0) {
        settle_ms = 7000;
    }
    cudaflight *g = new cudaflight();
    memset(g, 0, sizeof(*g));
    g->n = n;
    g->block = 32;
    g->grid = (n + g->block - 1) / g->block;

    if (!cuOk(cuInit(0), "cuInit") ||
        !cuOk(cuDeviceGet(&g->dev, device), "cuDeviceGet") ||
        !cuOk(cuDevicePrimaryCtxRetain(&g->ctx, g->dev), "cuDevicePrimaryCtxRetain")) {
        delete g;
        return nullptr;
    }

    CtxGuard guard(g->ctx);

    // The whole firmware runs on one thread's stack. The driver reserves
    // stack for the device-wide max resident thread count, so this can't
    // be extravagant: 32 KB ≈ 8 GB reserved on a 5090. (Resizable at any time.)
    if (!cuOk(cuCtxSetLimit(CU_LIMIT_STACK_SIZE, 32 * 1024), "cuCtxSetLimit") ||
        !cuOk(cuModuleLoad(&g->mod, cubin_path), "cuModuleLoad")) {
        cudaflight_destroy(g);
        return nullptr;
    }
    // The Enzyme reverse-mode kernels (bfFwStepGrad / bfFwStepJacGradPure)
    // heap-allocate their value tape via device malloc (hundreds of small
    // enzyme_cache_alloc per thread); the default 8 MB device heap is too small
    // once many instances run at once, and an out-of-heap malloc returns null ->
    // illegal address. Size it up — but best-effort: CU_LIMIT_MALLOC_HEAP_SIZE is
    // locked once the context is initialized, and we usually share XLA's primary
    // context (which jax touches before cudaflight_create), so the call returns
    // INVALID_VALUE. That is non-fatal: the heap keeps its current size, which is
    // ample for the small fleets used in tests/grad checks. To guarantee the
    // larger heap under XLA, set it before jax initializes — e.g. export
    // XLA_PYTHON_CLIENT_PREALLOCATE=false and create the env before any jax op,
    // or raise it via the driver in the host process first.
    const CUresult heapRc = cuCtxSetLimit(CU_LIMIT_MALLOC_HEAP_SIZE, (size_t)128 << 20);
    if (heapRc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(heapRc, &msg);
        fprintf(stderr, "[cudaflight] note: could not enlarge device malloc heap to "
                "128 MB (%s); keeping current size — fine for small fleets, but "
                "large Enzyme-gradient fleets may exhaust the default 8 MB heap\n",
                msg ? msg : "?");
    }

    int err = 0;
    g->imageSize = readU64(g, "__bf_image_size", &err);
    uint64_t align = readU64(g, "__bf_image_align", &err);
    g->stateSize = readU64(g, "__bf_state_size", &err);
    g->actDim = readU64(g, "__bf_act_dim", &err);
    g->obsDim = readU64(g, "__bf_obs_dim", &err);
    // AUX RC channels are optional: a module built before bfSetAux lacks
    // __bf_aux_dim, so read it with a separate flag and degrade to "no aux"
    // rather than failing create.
    int auxErr = 0;
    g->auxDim = readU64(g, "__bf_aux_dim", &auxErr);
    if (auxErr) {
        g->auxDim = 0;
    }
    g->osdRows = readU64(g, "__bf_osd_rows", &err);
    g->osdCols = readU64(g, "__bf_osd_cols", &err);
    if (err) {
        cudaflight_destroy(g);
        return nullptr;
    }
    if (align < 256) {
        align = 256; // cuMemAlloc returns >=256-aligned; keep deltas congruent
    }
    g->stride = (g->imageSize + align - 1) & ~(align - 1);

    bool ok =
        cuOk(cuMemAlloc(&g->instBuf, g->stride * n), "alloc instances") &&
        cuOk(cuMemAlloc(&g->snapBuf, g->stride * n), "alloc snapshot") &&
        cuOk(cuMemAlloc(&g->stateBuf, g->stateSize * n), "alloc state") &&
        cuOk(cuMemAlloc(&g->snapStBuf, g->stateSize * n), "alloc snap state") &&
        cuOk(cuMemAlloc(&g->actBuf, 4 * g->actDim * n), "alloc actions") &&
        cuOk(cuMemAlloc(&g->obsBuf, 4 * g->obsDim * n), "alloc obs") &&
        cuOk(cuMemAlloc(&g->rewBuf, 4 * n), "alloc rewards") &&
        cuOk(cuMemAlloc(&g->doneBuf, n), "alloc dones") &&
        cuOk(cuMemAlloc(&g->hashBuf, 8 * n), "alloc hashes") &&
        cuOk(cuMemAlloc(&g->altBuf, 4 * n), "alloc alt") &&
        cuOk(cuMemAlloc(&g->armedBuf, n), "alloc armed") &&
        cuOk(cuMemAlloc(&g->sensBuf, 4 * 7 * n), "alloc sensors") &&
        cuOk(cuMemAlloc(&g->motorBuf, 4 * 4 * n), "alloc motors") &&
        cuOk(cuMemAlloc(&g->osdBuf, g->osdRows * g->osdCols * n), "alloc osd") &&
        cuOk(cuMemAlloc(&g->osdAttrBuf, g->osdRows * g->osdCols * n), "alloc osd attrs") &&
        cuOk(cuMemsetD8(g->osdBuf, 0x20, g->osdRows * g->osdCols * n), "blank osd") &&
        cuOk(cuMemsetD8(g->osdAttrBuf, 0, g->osdRows * g->osdCols * n), "zero osd attrs") &&
        cuOk(cuMemsetD8(g->actBuf, 0, 4 * g->actDim * n), "zero actions") &&
        cuOk(cuMemsetD8(g->obsBuf, 0, 4 * g->obsDim * n), "zero obs") &&
        cuOk(cuMemsetD8(g->rewBuf, 0, 4 * n), "zero rewards") &&
        cuOk(cuMemsetD8(g->doneBuf, 0, n), "zero dones") &&
        cuOk(cuMemsetD8(g->sensBuf, 0, 4 * 7 * n), "zero sensors") &&
        cuOk(cuMemsetD8(g->motorBuf, 0, 4 * 4 * n), "zero motors");
    if (!ok) {
        cudaflight_destroy(g);
        return nullptr;
    }

    // AUX channel buffer (manual / free flight). Default AUX1 high (1800us =
    // armed), the rest low (1000us) — so a fw step that never calls set_aux
    // keeps the auto-armed, acro behaviour the RL path relies on.
    if (g->auxDim) {
        std::vector<float> aux(g->auxDim * (size_t)n, 1000.0f);
        for (uint32_t k = 0; k < n; k++) {
            aux[(size_t)k * g->auxDim] = 1800.0f; // AUX1 = arm
        }
        if (!cuOk(cuMemAlloc(&g->auxBuf, 4 * g->auxDim * n), "alloc aux") ||
            !cuOk(cuMemcpyHtoD(g->auxBuf, aux.data(), 4 * g->auxDim * (size_t)n), "init aux")) {
            cudaflight_destroy(g);
            return nullptr;
        }
    }

    // Differentiable-rollout scratch — only when requested. gradScratch alone is
    // stride*n (as big as the whole instance array), so skipping it for PPO is
    // the difference between fitting ~1.5x more worlds and OOMing. Left null
    // otherwise; cudaflight_destroy's free loop tolerates null (cuMemFree(0) no-ops).
    if (with_grad) {
        ok = cuOk(cuMemAlloc(&g->seedBuf, 4 * 4 * n), "alloc grad seed") &&
             cuOk(cuMemAlloc(&g->dactBuf, 4 * 4 * n), "alloc grad out") &&
             cuOk(cuMemAlloc(&g->gradScratch, g->stride * n), "alloc grad scratch");
        if (!ok) {
            cudaflight_destroy(g);
            return nullptr;
        }
    }

    char *base = (char *)g->instBuf;
    if (writeGlobal(g, "__bf_inst_base", &base, sizeof(base)) ||
        writeGlobal(g, "__bf_inst_stride", &g->stride, sizeof(g->stride)) ||
        writeGlobal(g, "__bf_inst_count", &n, sizeof(n))) {
        cudaflight_destroy(g);
        return nullptr;
    }

    ok = cuOk(cuModuleGetFunction(&g->fInit, g->mod, "bfInstanceInit"), "bfInstanceInit") &&
         cuOk(cuModuleGetFunction(&g->fBoot, g->mod, "bfBoot"), "bfBoot") &&
         cuOk(cuModuleGetFunction(&g->fRun, g->mod, "bfRun"), "bfRun") &&
         cuOk(cuModuleGetFunction(&g->fFinish, g->mod, "bfFinish"), "bfFinish") &&
         cuOk(cuModuleGetFunction(&g->fSnapshot, g->mod, "bfSnapshot"), "bfSnapshot") &&
         cuOk(cuModuleGetFunction(&g->fReset, g->mod, "bfReset"), "bfReset") &&
         cuOk(cuModuleGetFunction(&g->fStep, g->mod, "bfStep"), "bfStep") &&
         cuOk(cuModuleGetFunction(&g->fFwStep, g->mod, "bfFwStep"), "bfFwStep") &&
         cuOk(cuModuleGetFunction(&g->fOsd, g->mod, "bfOsdSnapshot"), "bfOsdSnapshot") &&
         // Value-threaded (rebase-aware) Jacobians for the differentiable rollout.
         // Both are part of the canonical build (DIFF=1); the Python package picks
         // which one the custom_vjp uses at runtime (Enzyme by default, FD oracle).
         cuOk(cuModuleGetFunction(&g->fJacFDPure, g->mod, "bfFwStepJacFDPure"), "bfFwStepJacFDPure") &&
         cuOk(cuModuleGetFunction(&g->fJacGradPure, g->mod, "bfFwStepJacGradPure"), "bfFwStepJacGradPure");
    if (!ok) {
        cudaflight_destroy(g);
        return nullptr;
    }
    // Optional: the finite-difference gradient kernel (present when the
    // module was built with the FD control core). Non-fatal if absent.
    cuModuleGetFunction(&g->fGradFD, g->mod, "bfFwStepGradFD");
    cuModuleGetFunction(&g->fRateEval, g->mod, "bfRateEval");
    cuModuleGetFunction(&g->fJacFD, g->mod, "bfFwStepJacFD");
    cuModuleGetFunction(&g->fSetBase, g->mod, "bfSetBase");
    // Optional: the AUX-channel writer (present when built with bfSetAux).
    cuModuleGetFunction(&g->fSetAux, g->mod, "bfSetAux");
    // Optional: the Enzyme reverse-mode VJP kernel (present when built DIFF=1).
    cuModuleGetFunction(&g->fGrad, g->mod, "bfFwStepGrad");

    uint32_t perturb = UINT32_MAX;
    void *initArgs[] = { &g->stateBuf, &perturb };
    void *bootArgs[] = { &g->stateBuf };
    void *snapArgs[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf };
    if (launch(g, g->fInit, initArgs)) {
        cudaflight_destroy(g);
        return nullptr;
    }

    if (eeprom_path) {
        FILE *f = fopen(eeprom_path, "rb");
        if (!f) {
            snprintf(g_err, sizeof(g_err), "cannot open eeprom image '%s'", eeprom_path);
            cudaflight_destroy(g);
            return nullptr;
        }
        fseek(f, 0, SEEK_END);
        const uint64_t eeLen = (uint64_t)ftell(f);
        rewind(f);
        std::vector<uint8_t> ee(eeLen);
        const bool readOk = fread(ee.data(), 1, eeLen, f) == eeLen;
        fclose(f);
        if (!readOk) {
            snprintf(g_err, sizeof(g_err), "failed to read eeprom image '%s'", eeprom_path);
            cudaflight_destroy(g);
            return nullptr;
        }

        CUfunction fLoadEeprom;
        CUdeviceptr eeBuf = 0;
        uint64_t len = eeLen;
        uint32_t perInstance = 0;
        void *eeArgs[] = { &eeBuf, &len, &perInstance };
        const bool ok =
            cuOk(cuModuleGetFunction(&fLoadEeprom, g->mod, "bfLoadEeprom"), "bfLoadEeprom") &&
            cuOk(cuMemAlloc(&eeBuf, eeLen), "alloc eeprom") &&
            cuOk(cuMemcpyHtoD(eeBuf, ee.data(), eeLen), "upload eeprom") &&
            launch(g, fLoadEeprom, eeArgs) == 0;
        if (eeBuf) {
            cuMemFree(eeBuf);
        }
        if (!ok) {
            cudaflight_destroy(g);
            return nullptr;
        }
    }

    if (launch(g, g->fBoot, bootArgs) ||
        runMs(g, settle_ms)) {
        cudaflight_destroy(g);
        return nullptr;
    }

    // Every instance must be armed at the snapshot point, or episodes
    // would start dead.
    void *finArgs[] = { &g->stateBuf, &g->hashBuf, &g->altBuf, &g->armedBuf };
    std::vector<uint8_t> armed(n);
    if (launch(g, g->fFinish, finArgs) ||
        !cuOk(cuMemcpyDtoH(armed.data(), g->armedBuf, n), "read armed")) {
        cudaflight_destroy(g);
        return nullptr;
    }
    for (uint32_t k = 0; k < n; k++) {
        if (!armed[k]) {
            snprintf(g_err, sizeof(g_err), "instance %u failed to arm during create", k);
            cudaflight_destroy(g);
            return nullptr;
        }
    }

    if (launch(g, g->fSnapshot, snapArgs)) {
        cudaflight_destroy(g);
        return nullptr;
    }

    // Recover the complete relocation table (static + runtime-written pointers)
    // from the just-snapshotted fleet. Required for position-independent blob
    // handling; aborts create if the table cannot be validated as complete.
    if (discoverRelocs(g)) {
        cudaflight_destroy(g);
        return nullptr;
    }
    return g;
}

// Eeprom create, kept for ABI compatibility — grad buffers ON (the grad harness
// and any caller of the old 5-arg symbol get the differentiable scratch).
cudaflight *cudaflight_create_eeprom(const char *cubin_path, uint32_t n, int device,
                           uint32_t settle_ms, const char *eeprom_path)
{
    return cudaflight_create_eeprom_ex(cubin_path, n, device, settle_ms, eeprom_path,
                                  /*with_grad=*/1);
}

// Default-config create, kept for ABI compatibility.
cudaflight *cudaflight_create(const char *cubin_path, uint32_t n, int device, uint32_t settle_ms)
{
    return cudaflight_create_eeprom(cubin_path, n, device, settle_ms, nullptr);
}

uint32_t cudaflight_num_envs(cudaflight *g) { return g->n; }
uint32_t cudaflight_act_dim(cudaflight *g) { return (uint32_t)g->actDim; }
uint32_t cudaflight_obs_dim(cudaflight *g) { return (uint32_t)g->obsDim; }

// Device pointers for zero-copy tensor wrapping.
uint64_t cudaflight_actions_ptr(cudaflight *g) { return (uint64_t)g->actBuf; }
uint64_t cudaflight_obs_ptr(cudaflight *g) { return (uint64_t)g->obsBuf; }
uint64_t cudaflight_rewards_ptr(cudaflight *g) { return (uint64_t)g->rewBuf; }
uint64_t cudaflight_dones_ptr(cudaflight *g) { return (uint64_t)g->doneBuf; }

// Apply the actions buffer for `decimation` 1ms control steps, then write
// obs / rewards / dones. decimation == 0 refreshes obs without advancing
// the sim (used for the post-reset observation).
int cudaflight_step(cudaflight *g, uint32_t decimation)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->actBuf, &g->obsBuf, &g->rewBuf, &g->doneBuf, &decimation };
    return launch(g, g->fStep, args);
}

// Restore instances flagged in a device uint8[n] mask to the snapshot.
int cudaflight_reset_mask(cudaflight *g, uint64_t mask_devptr)
{
    CtxGuard guard(g->ctx);
    CUdeviceptr mask = (CUdeviceptr)mask_devptr;
    void *args[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf, &mask };
    return launch(g, g->fReset, args);
}

// Auto-reset composition: the dones buffer doubles as the reset mask.
int cudaflight_reset_done(cudaflight *g)
{
    return cudaflight_reset_mask(g, (uint64_t)g->doneBuf);
}

int cudaflight_reset_all(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    // reuse armedBuf as an all-ones mask; it is 1 everywhere after create
    // but may be stale, so rewrite it
    CUTRY(cuMemsetD8(g->armedBuf, 1, g->n));
    return cudaflight_reset_mask(g, (uint64_t)g->armedBuf);
}

// Retake the episode-start snapshot at the current state (curriculum /
// domain randomization hooks).
int cudaflight_snapshot(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf };
    return launch(g, g->fSnapshot, args);
}

// Copy actions from another device buffer (float32 [n, act_dim]) into the
// actions buffer — for frameworks with immutable arrays (JAX) that cannot
// write into our buffer in place. Caller must ensure the source is fully
// written (e.g. block_until_ready) before calling.
int cudaflight_write_actions(cudaflight *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoD(g->actBuf, (CUdeviceptr)src_devptr, 4 * g->actDim * g->n));
    CUTRY(cuCtxSynchronize()); // source may be freed/reused once we return
    return 0;
}

// Host fallback for the same.
int cudaflight_write_actions_host(cudaflight *g, const float *src)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyHtoD(g->actBuf, src, 4 * g->actDim * g->n));
    return 0;
}

// Drain all device work (every stream in the context).
int cudaflight_sync(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuCtxSynchronize());
    return 0;
}

// ---------------------------------------------------------------------------
// External-physics mode: the firmware advances against sensor values from an
// outside simulator, which in turn consumes the firmware's motor outputs.
// Per 1ms control step: write sensors -> cudaflight_fw_step(1) -> read motors.
// The in-kernel physics is bypassed (its state goes stale); obs/reward/done
// are the external simulator's business.

// Device pointers for the exchange buffers.
uint64_t cudaflight_sensors_ptr(cudaflight *g) { return (uint64_t)g->sensBuf; }
uint64_t cudaflight_motors_ptr(cudaflight *g) { return (uint64_t)g->motorBuf; }
uint64_t cudaflight_armed_ptr(cudaflight *g) { return (uint64_t)g->armedBuf; }

// Copy sensors (float32 [n, 7]: gyro NED rad/s, specific force NED m/s^2,
// baro Pa) from another device buffer. Caller must ensure the source is
// fully written before calling.
int cudaflight_write_sensors(cudaflight *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoD(g->sensBuf, (CUdeviceptr)src_devptr, 4 * 7 * g->n));
    CUTRY(cuCtxSynchronize()); // source may be freed/reused once we return
    return 0;
}

// Host fallback for the same.
int cudaflight_write_sensors_host(cudaflight *g, const float *src)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyHtoD(g->sensBuf, src, 4 * 7 * g->n));
    return 0;
}

// Advance the firmware `substeps` 1ms control steps against the current
// sensors and actions buffers; writes normalised motor outputs [n, 4] and
// the armed flags [n].
int cudaflight_fw_step(cudaflight *g, uint32_t substeps)
{
    CtxGuard guard(g->ctx);
    // Apply host-driven AUX channels (arm / flight mode) before stepping, if the
    // module supports them. No-op for the RL path, which never writes auxBuf
    // (it stays at the armed-on-create default).
    if (g->fSetAux && g->auxDim && g->auxBuf) {
        void *auxArgs[] = { &g->stateBuf, &g->auxBuf };
        if (launch(g, g->fSetAux, auxArgs)) {
            return -1;
        }
    }
    void *args[] = { &g->stateBuf, &g->actBuf, &g->sensBuf, &g->motorBuf,
                     &g->armedBuf, &substeps };
    return launch(g, g->fFwStep, args);
}

// AUX RC channels for manual / free flight (arm switch, flight mode, ...). The
// buffer is [n x aux_dim] float32 RC microsecond values (1000..2000); bfSetAux
// copies it into each instance's rc[4..] before the fw step. cudaflight_aux_dim()
// returns 0 if the loaded module predates AUX support.
uint32_t cudaflight_aux_dim(cudaflight *g) { return (uint32_t)g->auxDim; }
uint64_t cudaflight_aux_ptr(cudaflight *g) { return (uint64_t)g->auxBuf; }

int cudaflight_write_aux(cudaflight *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    if (!g->auxBuf || !g->auxDim) {
        return 0;
    }
    CUTRY(cuMemcpyDtoD(g->auxBuf, (CUdeviceptr)src_devptr, 4 * g->auxDim * g->n));
    CUTRY(cuCtxSynchronize());
    return 0;
}

int cudaflight_write_aux_host(cudaflight *g, const float *src)
{
    CtxGuard guard(g->ctx);
    if (!g->auxBuf || !g->auxDim) {
        return 0;
    }
    CUTRY(cuMemcpyHtoD(g->auxBuf, src, 4 * g->auxDim * g->n));
    return 0;
}

// ---------------------------------------------------------------------------
// Finite-difference gradient of the real control law. Reads the actions
// buffer and the motor-cotangent seed buffer, writes the action gradient
// buffer: dActions = J^T . seed, J = d(motor)/d(action) of bflRateCore at the
// current per-instance state (state saved/restored around each perturbation).

uint64_t cudaflight_seed_ptr(cudaflight *g) { return (uint64_t)g->seedBuf; }   // [n x 4] in
uint64_t cudaflight_dact_ptr(cudaflight *g) { return (uint64_t)g->dactBuf; }   // [n x 4] out

int cudaflight_write_seed(cudaflight *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoD(g->seedBuf, (CUdeviceptr)src_devptr, 4 * 4 * g->n));
    CUTRY(cuCtxSynchronize());
    return 0;
}

// Run the FD gradient over the current actions/seed buffers. eps is the
// action-space central-difference step (e.g. 1e-3).
int cudaflight_grad_fd(cudaflight *g, float eps)
{
    CtxGuard guard(g->ctx);
    if (!g->fGradFD) {
        snprintf(g_err, sizeof(g_err), "module has no bfFwStepGradFD kernel");
        return -1;
    }
    void *args[] = { &g->actBuf, &g->seedBuf, &g->dactBuf, &g->gradScratch, &eps };
    return launch(g, g->fGradFD, args);
}

// Debug: run the control core once over the actions buffer, writing the raw
// float mixer output to the motors buffer (read via cudaflight_motors_ptr). Mutates
// instance state — use cudaflight_reset_all after to restore.
int cudaflight_rate_eval(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    if (!g->fRateEval) {
        snprintf(g_err, sizeof(g_err), "module has no bfRateEval kernel");
        return -1;
    }
    void *args[] = { &g->actBuf, &g->motorBuf };
    return launch(g, g->fRateEval, args);
}

int cudaflight_motors_read_host(cudaflight *g, float *dst)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoH(dst, g->motorBuf, 4 * 4 * g->n));
    return 0;
}

// Host-side staging helpers (for tests / non-CUDA callers).
int cudaflight_write_seed_host(cudaflight *g, const float *src)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyHtoD(g->seedBuf, src, 4 * 4 * g->n));
    return 0;
}

int cudaflight_grad_read_host(cudaflight *g, float *dst)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoH(dst, g->dactBuf, 4 * 4 * g->n));
    return 0;
}

// ---------------------------------------------------------------------------
// OSD readback. The firmware draws its OSD into per-instance character
// grids as part of normal stepping; cudaflight_osd_update() snapshots every
// instance's grid (and displayport attributes: severity bits + blink in
// bit 7) into the exported [n, rows*cols] uint8 device buffers, which a
// renderer maps as zero-copy tensors.

uint32_t cudaflight_osd_rows(cudaflight *g) { return (uint32_t)g->osdRows; }
uint32_t cudaflight_osd_cols(cudaflight *g) { return (uint32_t)g->osdCols; }
uint64_t cudaflight_osd_ptr(cudaflight *g) { return (uint64_t)g->osdBuf; }
uint64_t cudaflight_osd_attrs_ptr(cudaflight *g) { return (uint64_t)g->osdAttrBuf; }

int cudaflight_osd_update(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->osdBuf, &g->osdAttrBuf };
    return launch(g, g->fOsd, args);
}

// ---------------------------------------------------------------------------
// Raw launch parameters for external launchers (e.g. an XLA FFI handler
// that runs bfFwStep inside a jitted JAX program, on XLA's stream, with
// no synchronization). The caller owns stream ordering; the kernel only
// touches the firmware blobs plus whatever buffers the caller passes.

uint64_t cudaflight_fw_step_kernel(cudaflight *g) { return (uint64_t)g->fFwStep; }
uint64_t cudaflight_state_ptr(cudaflight *g) { return (uint64_t)g->stateBuf; }
uint64_t cudaflight_ctx(cudaflight *g) { return (uint64_t)g->ctx; }
uint32_t cudaflight_grid(cudaflight *g) { return g->grid; }
uint32_t cudaflight_block(cudaflight *g) { return g->block; }

// Pure (value-threaded) FFI path: the firmware blob and bfFlight_t state become
// donated JAX buffers. bfSetBase sets the per-launch instance base; the blob is
// `stride` bytes/instance, the bfFlight_t state `stateSize` bytes/instance. The
// initial values are copied from the episode-start snapshot buffers
// (cudaflight_snap_ptr / cudaflight_snap_state_ptr).
uint64_t cudaflight_set_base_kernel(cudaflight *g) { return (uint64_t)g->fSetBase; }
uint64_t cudaflight_stride(cudaflight *g) { return g->stride; }
uint64_t cudaflight_state_size(cudaflight *g) { return g->stateSize; }
uint64_t cudaflight_inst_ptr(cudaflight *g) { return (uint64_t)g->instBuf; }

// Reset kernel (bfReset) launch params, for an in-jit masked reset FFI.
// bfReset(state, snapSt, snap, mask): restores masked instances to the
// snapshot. Same grid/block/ctx as the step kernel above.
uint64_t cudaflight_reset_kernel(cudaflight *g) { return (uint64_t)g->fReset; }
uint64_t cudaflight_snap_state_ptr(cudaflight *g) { return (uint64_t)g->snapStBuf; }
uint64_t cudaflight_snap_ptr(cudaflight *g) { return (uint64_t)g->snapBuf; }

// Raw launch params for the in-jit FD Jacobian FFI: bfFwStepJacFD(actions,
// jacOut[N,16], scratch, eps) computes J[k][i][d] = d(motor_i)/d(action_d) of
// the real control law at the current per-instance state. gradScratch is the
// per-instance blob save buffer the kernel uses for state restore.
uint64_t cudaflight_jac_fd_kernel(cudaflight *g) { return (uint64_t)g->fJacFD; }
uint64_t cudaflight_grad_scratch_ptr(cudaflight *g) { return (uint64_t)g->gradScratch; }

// Raw launch params for the value-threaded ("pure") Jacobian FFIs used by the
// differentiable rollout's custom_vjp. Both rebase the donated blob on entry
// (bfSetBase must point __bf_inst_base at it first) and write the full
// jacOut[N,16], J[k][i][d] = d(motor_i)/d(action_d) at the pre-step state:
//   bfFwStepJacFDPure(actions[N,4], jacOut[N,16], scratch, eps)  — finite diff
//   bfFwStepJacGradPure(actions[N,4], jacOut[N,16], scratch)     — Enzyme reverse
// Both share gradScratch for the per-instance state save/restore.
uint64_t cudaflight_jac_fd_pure_kernel(cudaflight *g) { return (uint64_t)g->fJacFDPure; }
uint64_t cudaflight_jac_grad_pure_kernel(cudaflight *g) { return (uint64_t)g->fJacGradPure; }

// Raw launch param for the in-jit Enzyme VJP FFI: bfFwStepGrad(actions[N,4],
// seedMotors[N,4], dActions[N,4]) computes dActions = J^T . seedMotors of the
// real control law at the current per-instance state (reverse-mode autodiff).
// 0 if the module was not built DIFF=1.
uint64_t cudaflight_grad_kernel(cudaflight *g) { return (uint64_t)g->fGrad; }

// Self-test for position-independent relocation: run a deterministic burst from
// the snapshot, relocate the whole fleet's blobs to a freshly allocated buffer
// (copy bytes + repoint the global base; rebase-on-move fixes the pointers on
// the next step), run the identical burst, and compare motor-trace hashes.
// Bit-identical hashes prove relocation is exact. Returns 0 on full match, 1 on
// any mismatch, -1 on error. Restores the original buffer/base before returning
// (call cudaflight_reset_all before normal use afterwards).
int cudaflight_hashes(cudaflight *g, uint64_t *out); // defined below

int cudaflight_relocate_selftest(cudaflight *g)
{
    CtxGuard guard(g->ctx);
    if (!g->fullRelocCount) {
        snprintf(g_err, sizeof(g_err), "no reloc table (need >=3 instances)");
        return -1;
    }
    const uint32_t STEPS = 200, DEC = 10;
    std::vector<uint64_t> h1(g->n), h2(g->n);

    // reference burst at the original base
    if (cudaflight_reset_all(g)) {
        return -1;
    }
    for (uint32_t i = 0; i < STEPS; i++) {
        if (cudaflight_step(g, DEC)) {
            return -1;
        }
    }
    if (cudaflight_hashes(g, h1.data())) {
        return -1;
    }

    // relocate: move the blob bytes to a fresh buffer and repoint the base.
    CUdeviceptr alt = 0;
    if (!cuOk(cuMemAlloc(&alt, g->stride * g->n), "alloc relocate") ||
        !cuOk(cuMemcpyDtoD(alt, g->instBuf, g->stride * g->n), "copy blob")) {
        if (alt) {
            cuMemFree(alt);
        }
        return -1;
    }
    const CUdeviceptr origInst = g->instBuf;
    char *altBase = (char *)alt;
    if (writeGlobal(g, "__bf_inst_base", &altBase, sizeof(altBase))) {
        cuMemFree(alt);
        return -1;
    }
    g->instBuf = alt; // reset/step now operate on the relocated buffer

    // identical burst at the new base — rebase-on-entry fires on the first step
    int rc = 0;
    if (cudaflight_reset_all(g)) {
        rc = -1;
    }
    for (uint32_t i = 0; i < STEPS && !rc; i++) {
        if (cudaflight_step(g, DEC)) {
            rc = -1;
        }
    }
    if (!rc && cudaflight_hashes(g, h2.data())) {
        rc = -1;
    }

    // restore the original buffer/base regardless of outcome
    char *origBase = (char *)origInst;
    writeGlobal(g, "__bf_inst_base", &origBase, sizeof(origBase));
    g->instBuf = origInst;
    cuMemFree(alt);
    if (rc) {
        return rc;
    }

    uint32_t mism = 0;
    for (uint32_t k = 0; k < g->n; k++) {
        if (h1[k] != h2[k]) {
            mism++;
        }
    }
    printf("[cudaflight-reloc] relocate self-test: %u/%u instances bit-identical "
           "after relocation\n", g->n - mism, g->n);
    return mism ? 1 : 0;
}

// Motor-trace hashes (host out, n entries) — the determinism oracle.
int cudaflight_hashes(cudaflight *g, uint64_t *out)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->hashBuf, &g->altBuf, &g->armedBuf };
    if (launch(g, g->fFinish, args)) {
        return -1;
    }
    CUTRY(cuMemcpyDtoH(out, g->hashBuf, 8 * g->n));
    return 0;
}

} // extern "C"
