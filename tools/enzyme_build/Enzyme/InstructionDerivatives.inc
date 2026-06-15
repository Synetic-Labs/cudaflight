/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

 case llvm::Instruction::FNeg:
 {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
    auto mod = inst.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&inst));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(inst);
    if (gutils->isConstantInstruction(&inst) && gutils->isConstantValue(&inst))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&inst);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(inst.getType()));
        if (!gutils->isConstantValue(inst.getOperand(0))) {
          Value *dif = diffe(inst.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FNeg
              auto fwdarg_0 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFNeg((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        assert(res);
        setDiffe(&inst, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&inst);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(inst.getType()));
        if (!gutils->isConstantValue(inst.getOperand(0))) {
          Value *dif = diffe(inst.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(inst.getOperand(0))), gutils->getNewFromOriginal(&inst));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FNeg
              auto fwdarg_0 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFNeg((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&inst)), res);
        assert(res);
        Function *logFunc = inst.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = inst.getModule()->getModuleIdentifier() ;
            std::string functionName = inst.getFunction()->getName().str();
            std::string blockName = inst.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(inst.getModule()->begin(), inst.getModule()->end(),
              [&](const auto& func) { return &func == inst.getFunction(); });
            if (funcIt != inst.getModule()->end()) {
              funcIdx = std::distance(inst.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(inst.getFunction()->begin(), inst.getFunction()->end(),
              [&](const auto& block) { return &block == inst.getParent(); });
            if (blockIt != inst.getFunction()->end()) {
              blockIdx = std::distance(inst.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(inst.getParent()->begin(), inst.getParent()->end(),
              [&](const auto& curr) { return &curr == &inst; });
            if (instIt != inst.getParent()->end()) {
              instIdx = std::distance(inst.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&inst), Type::getDoubleTy(inst.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(inst.getContext()));
            std::string opcodeName = inst.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&inst)) {
                if (Function *fn = CI->getCalledFunction()) {
                    calleeName = fn->getName();
                } else {
                    calleeName = "<Unknown>";
                }
            }
#if LLVM_VERSION_MAJOR >= 17
            Value *moduleNameValue = Builder2.CreateGlobalString(moduleName);
            Value *functionNameValue = Builder2.CreateGlobalString(functionName + " (" +std::to_string(funcIdx) + ")");
            Value *blockNameValue = Builder2.CreateGlobalString(blockName + " (" +std::to_string(blockIdx) + ")");
            Value *opcodeNameValue = Builder2.CreateGlobalString(opcodeName + " (" +std::to_string(instIdx) + ")");
            Value *calleeNameValue = Builder2.CreateGlobalString(calleeName);
#else
            Value *moduleNameValue = Builder2.CreateGlobalStringPtr(moduleName);
            Value *functionNameValue = Builder2.CreateGlobalStringPtr(functionName + " (" +std::to_string(funcIdx) + ")");
            Value *blockNameValue = Builder2.CreateGlobalStringPtr(blockName + " (" +std::to_string(blockIdx) + ")");
            Value *opcodeNameValue = Builder2.CreateGlobalStringPtr(opcodeName + " (" +std::to_string(instIdx) + ")");
            Value *calleeNameValue = Builder2.CreateGlobalStringPtr(calleeName);
#endif
            Builder2.CreateCall(logFunc, {origValue, errValue, opcodeNameValue, calleeNameValue, moduleNameValue, functionNameValue, blockNameValue});
        }
        setDiffe(&inst, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&inst);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(inst.getOperand(0)) && !isa<PointerType>(inst.getOperand(0)->getType()) ) {
          dif = diffe(&inst, Builder2);
          setDiffe(&inst, Constant::getNullValue(gutils->getShadowType(inst.getType())), Builder2);
        }
        if (!gutils->isConstantValue(inst.getOperand(0)) && !isa<PointerType>(inst.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FNeg
              auto revarg_0 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFNeg((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(inst.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(inst.getOperand(0), toadd, Builder2, inst.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(inst));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(inst.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << inst << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(inst)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (inst).getDebugLoc(), &(inst), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(inst));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(inst.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << inst << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(inst)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (inst).getDebugLoc(), &(inst), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return;
  }
 case llvm::Instruction::Freeze:
 {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
    auto mod = inst.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&inst));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(inst);
    if (gutils->isConstantInstruction(&inst) && gutils->isConstantValue(&inst))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&inst);
        getForwardBuilder(Builder2);
            Value *res = ({
              // Computing Freeze
              auto fwdnsrarg_0 = gutils->invertPointerM(inst.getOperand(0), Builder2, TR.query(&inst));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFreeze((gutils->getWidth() == 1) ? fwdnsrarg_0 : gutils->extractMeta(Builder2, fwdnsrarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
        assert(res);
        setDiffe(&inst, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&inst);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(inst.getType()));
        if (!gutils->isConstantValue(inst.getOperand(0))) {
          Value *dif = diffe(inst.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(inst.getOperand(0))), gutils->getNewFromOriginal(&inst));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Freeze
              auto fwdarg_0 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFreeze((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&inst)), res);
        assert(res);
        Function *logFunc = inst.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = inst.getModule()->getModuleIdentifier() ;
            std::string functionName = inst.getFunction()->getName().str();
            std::string blockName = inst.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(inst.getModule()->begin(), inst.getModule()->end(),
              [&](const auto& func) { return &func == inst.getFunction(); });
            if (funcIt != inst.getModule()->end()) {
              funcIdx = std::distance(inst.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(inst.getFunction()->begin(), inst.getFunction()->end(),
              [&](const auto& block) { return &block == inst.getParent(); });
            if (blockIt != inst.getFunction()->end()) {
              blockIdx = std::distance(inst.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(inst.getParent()->begin(), inst.getParent()->end(),
              [&](const auto& curr) { return &curr == &inst; });
            if (instIt != inst.getParent()->end()) {
              instIdx = std::distance(inst.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&inst), Type::getDoubleTy(inst.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(inst.getContext()));
            std::string opcodeName = inst.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&inst)) {
                if (Function *fn = CI->getCalledFunction()) {
                    calleeName = fn->getName();
                } else {
                    calleeName = "<Unknown>";
                }
            }
#if LLVM_VERSION_MAJOR >= 17
            Value *moduleNameValue = Builder2.CreateGlobalString(moduleName);
            Value *functionNameValue = Builder2.CreateGlobalString(functionName + " (" +std::to_string(funcIdx) + ")");
            Value *blockNameValue = Builder2.CreateGlobalString(blockName + " (" +std::to_string(blockIdx) + ")");
            Value *opcodeNameValue = Builder2.CreateGlobalString(opcodeName + " (" +std::to_string(instIdx) + ")");
            Value *calleeNameValue = Builder2.CreateGlobalString(calleeName);
#else
            Value *moduleNameValue = Builder2.CreateGlobalStringPtr(moduleName);
            Value *functionNameValue = Builder2.CreateGlobalStringPtr(functionName + " (" +std::to_string(funcIdx) + ")");
            Value *blockNameValue = Builder2.CreateGlobalStringPtr(blockName + " (" +std::to_string(blockIdx) + ")");
            Value *opcodeNameValue = Builder2.CreateGlobalStringPtr(opcodeName + " (" +std::to_string(instIdx) + ")");
            Value *calleeNameValue = Builder2.CreateGlobalStringPtr(calleeName);
#endif
            Builder2.CreateCall(logFunc, {origValue, errValue, opcodeNameValue, calleeNameValue, moduleNameValue, functionNameValue, blockNameValue});
        }
        setDiffe(&inst, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&inst);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(inst.getOperand(0)) && !isa<PointerType>(inst.getOperand(0)->getType()) ) {
          dif = diffe(&inst, Builder2);
          setDiffe(&inst, Constant::getNullValue(gutils->getShadowType(inst.getType())), Builder2);
        }
        if (!gutils->isConstantValue(inst.getOperand(0)) && !isa<PointerType>(inst.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Freeze
              auto revarg_0 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFreeze((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(inst.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(inst.getOperand(0), toadd, Builder2, inst.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(inst));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(inst.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << inst << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(inst)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (inst).getDebugLoc(), &(inst), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(inst));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(inst.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << inst << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(inst)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (inst).getDebugLoc(), &(inst), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return;
  }
