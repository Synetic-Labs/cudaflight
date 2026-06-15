/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

 case llvm::Instruction::FAdd:
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
    auto mod = BO.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&BO));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(BO);
    if (gutils->isConstantInstruction(&BO) && gutils->isConstantValue(&BO))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        assert(res);
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(0))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(1))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&BO)), res);
        assert(res);
        Function *logFunc = BO.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = BO.getModule()->getModuleIdentifier() ;
            std::string functionName = BO.getFunction()->getName().str();
            std::string blockName = BO.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(BO.getModule()->begin(), BO.getModule()->end(),
              [&](const auto& func) { return &func == BO.getFunction(); });
            if (funcIt != BO.getModule()->end()) {
              funcIdx = std::distance(BO.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(BO.getFunction()->begin(), BO.getFunction()->end(),
              [&](const auto& block) { return &block == BO.getParent(); });
            if (blockIt != BO.getFunction()->end()) {
              blockIdx = std::distance(BO.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(BO.getParent()->begin(), BO.getParent()->end(),
              [&](const auto& curr) { return &curr == &BO; });
            if (instIt != BO.getParent()->end()) {
              instIdx = std::distance(BO.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&BO), Type::getDoubleTy(BO.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(BO.getContext()));
            std::string opcodeName = BO.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&BO)) {
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&BO);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        }
        if (!gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(0), toadd, Builder2, BO.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(1), toadd, Builder2, BO.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
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
 case llvm::Instruction::FSub:
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
    auto mod = BO.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&BO));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(BO);
    if (gutils->isConstantInstruction(&BO) && gutils->isConstantValue(&BO))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(0))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(1))), gutils->getNewFromOriginal(&BO));
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&BO)), res);
        assert(res);
        Function *logFunc = BO.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = BO.getModule()->getModuleIdentifier() ;
            std::string functionName = BO.getFunction()->getName().str();
            std::string blockName = BO.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(BO.getModule()->begin(), BO.getModule()->end(),
              [&](const auto& func) { return &func == BO.getFunction(); });
            if (funcIt != BO.getModule()->end()) {
              funcIdx = std::distance(BO.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(BO.getFunction()->begin(), BO.getFunction()->end(),
              [&](const auto& block) { return &block == BO.getParent(); });
            if (blockIt != BO.getFunction()->end()) {
              blockIdx = std::distance(BO.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(BO.getParent()->begin(), BO.getParent()->end(),
              [&](const auto& curr) { return &curr == &BO; });
            if (instIt != BO.getParent()->end()) {
              instIdx = std::distance(BO.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&BO), Type::getDoubleTy(BO.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(BO.getContext()));
            std::string opcodeName = BO.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&BO)) {
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&BO);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        }
        if (!gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(0), toadd, Builder2, BO.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
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
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(1), toadd, Builder2, BO.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
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
 case llvm::Instruction::FMul:
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
    auto mod = BO.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&BO));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(BO);
    if (gutils->isConstantInstruction(&BO) && gutils->isConstantValue(&BO))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(BO.getOperand(1));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(BO.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(0))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(BO.getOperand(1));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(1))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(BO.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&BO)), res);
        assert(res);
        Function *logFunc = BO.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = BO.getModule()->getModuleIdentifier() ;
            std::string functionName = BO.getFunction()->getName().str();
            std::string blockName = BO.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(BO.getModule()->begin(), BO.getModule()->end(),
              [&](const auto& func) { return &func == BO.getFunction(); });
            if (funcIt != BO.getModule()->end()) {
              funcIdx = std::distance(BO.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(BO.getFunction()->begin(), BO.getFunction()->end(),
              [&](const auto& block) { return &block == BO.getParent(); });
            if (blockIt != BO.getFunction()->end()) {
              blockIdx = std::distance(BO.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(BO.getParent()->begin(), BO.getParent()->end(),
              [&](const auto& curr) { return &curr == &BO; });
            if (instIt != BO.getParent()->end()) {
              instIdx = std::distance(BO.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&BO), Type::getDoubleTy(BO.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(BO.getContext()));
            std::string opcodeName = BO.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&BO)) {
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&BO);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        }
        if (!gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedMul
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(1)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(0), toadd, Builder2, BO.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedMul
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(0)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(1), toadd, Builder2, BO.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
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
 case llvm::Instruction::FDiv:
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
    auto mod = BO.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&BO));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(BO);
    if (gutils->isConstantInstruction(&BO) && gutils->isConstantValue(&BO))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
            Value *res = ({
              // Computing CheckedDiv
              auto fwdnsrarg_0 = ({
                // Computing FSub
                auto fwdnsrarg_1_0 = ({
                  // Computing SelectIfActive
                  llvm::Value *imVal = BO.getOperand(0);
                  bool condition = !gutils->isConstantValue(imVal);
                  bool vectorized = false;
                  (void)vectorized;
                  if (condition) {
                    imVal = ({
                      // Computing FMul
                      auto fwdnsrarg_1_1_cic_0 = gutils->invertPointerM(BO.getOperand(0), Builder2, TR.query(&BO));
                      llvm::Value* fwdnsrarg_1_1_cic_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                      Value *res = nullptr;
                      for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                        Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdnsrarg_1_1_cic_0 : gutils->extractMeta(Builder2, fwdnsrarg_1_1_cic_0, idx), fwdnsrarg_1_1_cic_1);
                        if (gutils->getWidth() == 1) res = V;
                        else {
                          if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                          res = Builder2.CreateInsertValue(res, V, {idx});
                        }
                      }
                      res;
                    });
                    vectorized = true;
                  } else {
                    imVal = Constant::getNullValue(BO.getOperand(0)->getType());
                  }
                  if (!vectorized) {
                    llvm::Value* vec_imVal = gutils->getWidth() == 1 ? imVal : UndefValue::get(gutils->getShadowType(imVal->getType()));
                    if (gutils->getWidth() != 1)
                      for (size_t i=0; i<gutils->getWidth(); i++)
                        vec_imVal = Builder2.CreateInsertValue(vec_imVal, imVal, std::vector<unsigned>({(unsigned)i}));
                    imVal = vec_imVal;
                  }
                  imVal;
                  });
                auto fwdnsrarg_1_1 = ({
                  // Computing SelectIfActive
                  llvm::Value *imVal = BO.getOperand(1);
                  bool condition = !gutils->isConstantValue(imVal);
                  bool vectorized = false;
                  (void)vectorized;
                  if (condition) {
                    imVal = ({
                      // Computing FMul
                      auto fwdnsrarg_1_2_cic_0 = gutils->invertPointerM(BO.getOperand(1), Builder2, TR.query(&BO));
                      llvm::Value* fwdnsrarg_1_2_cic_1 = gutils->getNewFromOriginal(BO.getOperand(0));
                      Value *res = nullptr;
                      for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                        Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdnsrarg_1_2_cic_0 : gutils->extractMeta(Builder2, fwdnsrarg_1_2_cic_0, idx), fwdnsrarg_1_2_cic_1);
                        if (gutils->getWidth() == 1) res = V;
                        else {
                          if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                          res = Builder2.CreateInsertValue(res, V, {idx});
                        }
                      }
                      res;
                    });
                    vectorized = true;
                  } else {
                    imVal = Constant::getNullValue(BO.getOperand(1)->getType());
                  }
                  if (!vectorized) {
                    llvm::Value* vec_imVal = gutils->getWidth() == 1 ? imVal : UndefValue::get(gutils->getShadowType(imVal->getType()));
                    if (gutils->getWidth() != 1)
                      for (size_t i=0; i<gutils->getWidth(); i++)
                        vec_imVal = Builder2.CreateInsertValue(vec_imVal, imVal, std::vector<unsigned>({(unsigned)i}));
                    imVal = vec_imVal;
                  }
                  imVal;
                  });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFSub((gutils->getWidth() == 1) ? fwdnsrarg_1_0 : gutils->extractMeta(Builder2, fwdnsrarg_1_0, idx), (gutils->getWidth() == 1) ? fwdnsrarg_1_1 : gutils->extractMeta(Builder2, fwdnsrarg_1_1, idx));
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
              auto fwdnsrarg_1 = ({
                // Computing FMul
                llvm::Value* fwdnsrarg_2_0 = gutils->getNewFromOriginal(BO.getOperand(1));
                llvm::Value* fwdnsrarg_2_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                Builder2.CreateFMul(fwdnsrarg_2_0, fwdnsrarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedDiv(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdnsrarg_0 : gutils->extractMeta(Builder2, fwdnsrarg_0, idx), fwdnsrarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
        assert(res);
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(0))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedDiv
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(BO.getOperand(1));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedDiv(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(1))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FNeg
              auto fwdarg_0 = ({
                // Computing CheckedMul
                auto fwdarg_1_0 = ({
                  // Computing CheckedDiv
                  auto fwdarg_1_1_0 = dif;
                  llvm::Value* fwdarg_1_1_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                  Value *res = nullptr;
                  for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                    Value *V = checkedDiv(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_1_1_0 : gutils->extractMeta(Builder2, fwdarg_1_1_0, idx), fwdarg_1_1_1);
                    if (gutils->getWidth() == 1) res = V;
                    else {
                      if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                      res = Builder2.CreateInsertValue(res, V, {idx});
                    }
                  }
                  res;
                });
                auto fwdarg_1_1 = ({
                  // Computing FDiv
                  llvm::Value* fwdarg_1_2_0 = gutils->getNewFromOriginal(BO.getOperand(0));
                  llvm::Value* fwdarg_1_2_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                  Builder2.CreateFDiv(fwdarg_1_2_0, fwdarg_1_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_1_0 : gutils->extractMeta(Builder2, fwdarg_1_0, idx), fwdarg_1_1);
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&BO)), res);
        assert(res);
        Function *logFunc = BO.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = BO.getModule()->getModuleIdentifier() ;
            std::string functionName = BO.getFunction()->getName().str();
            std::string blockName = BO.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(BO.getModule()->begin(), BO.getModule()->end(),
              [&](const auto& func) { return &func == BO.getFunction(); });
            if (funcIt != BO.getModule()->end()) {
              funcIdx = std::distance(BO.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(BO.getFunction()->begin(), BO.getFunction()->end(),
              [&](const auto& block) { return &block == BO.getParent(); });
            if (blockIt != BO.getFunction()->end()) {
              blockIdx = std::distance(BO.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(BO.getParent()->begin(), BO.getParent()->end(),
              [&](const auto& curr) { return &curr == &BO; });
            if (instIt != BO.getParent()->end()) {
              instIdx = std::distance(BO.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&BO), Type::getDoubleTy(BO.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(BO.getContext()));
            std::string opcodeName = BO.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&BO)) {
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&BO);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        }
        if (!gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedDiv
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(1)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedDiv(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(0), toadd, Builder2, BO.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FNeg
              auto revarg_0 = ({
                // Computing CheckedMul
                auto revarg_1_0 = ({
                  // Computing CheckedDiv
                  auto revarg_1_1_0 = dif;
                  llvm::Value* revarg_1_1_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(1)), Builder2);
                  Value *res = nullptr;
                  for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                    Value *V = checkedDiv(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_1_1_0 : gutils->extractMeta(Builder2, revarg_1_1_0, idx), revarg_1_1_1);
                    if (gutils->getWidth() == 1) res = V;
                    else {
                      if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                      res = Builder2.CreateInsertValue(res, V, {idx});
                    }
                  }
                  res;
                });
                auto revarg_1_1 = ({
                  // Computing FDiv
                  llvm::Value* revarg_1_2_0 = lookup(gutils->getNewFromOriginal(BO.getOperand(0)), Builder2);
                  llvm::Value* revarg_1_2_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(1)), Builder2);
                  Builder2.CreateFDiv(revarg_1_2_0, revarg_1_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_1_0 : gutils->extractMeta(Builder2, revarg_1_0, idx), revarg_1_1);
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
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
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(1), toadd, Builder2, BO.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
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
 case llvm::Instruction::FRem:
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
    auto mod = BO.getParent()->getParent()->getParent();
    auto *const newCall = cast<llvm::Instruction>(gutils->getNewFromOriginal(&BO));
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    eraseIfUnused(BO);
    if (gutils->isConstantInstruction(&BO) && gutils->isConstantValue(&BO))
      return;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          llvm::Value *local_div = nullptr;
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FNeg
                auto fwdarg_2_0 = ({
                  // Computing copysign
                  auto fwdarg_2_1_0 = ({
                    // Computing floor
                    auto fwdarg_2_1_1_0 = ({
                      // Computing fabs
                      auto fwdarg_2_1_1_1_0 = ({
                        // Computing FDiv
                        llvm::Value* fwdarg_2_1_1_1_1_0 = gutils->getNewFromOriginal(BO.getOperand(0));
                        llvm::Value* fwdarg_2_1_1_1_1_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                        Builder2.CreateFDiv(fwdarg_2_1_1_1_1_0, fwdarg_2_1_1_1_1_1);
                      });
                      local_div = fwdarg_2_1_1_1_0;
                      Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::fabs, std::vector<Type*>({fwdarg_2_1_1_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_1_1_0}));
                    });
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::floor, std::vector<Type*>({fwdarg_2_1_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_1_0}));
                  });
                  auto fwdarg_2_1_1 = local_div;
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::copysign, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0, fwdarg_2_1_1}));
                });
                Builder2.CreateFNeg(fwdarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&BO);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(BO.getType()));
        if (!gutils->isConstantValue(BO.getOperand(0))) {
          Value *dif = diffe(BO.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(0))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(BO.getOperand(1))) {
          Value *dif = diffe(BO.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(BO.getOperand(1))), gutils->getNewFromOriginal(&BO));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          llvm::Value *local_div = nullptr;
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FNeg
                auto fwdarg_2_0 = ({
                  // Computing copysign
                  auto fwdarg_2_1_0 = ({
                    // Computing floor
                    auto fwdarg_2_1_1_0 = ({
                      // Computing fabs
                      auto fwdarg_2_1_1_1_0 = ({
                        // Computing FDiv
                        llvm::Value* fwdarg_2_1_1_1_1_0 = gutils->getNewFromOriginal(BO.getOperand(0));
                        llvm::Value* fwdarg_2_1_1_1_1_1 = gutils->getNewFromOriginal(BO.getOperand(1));
                        Builder2.CreateFDiv(fwdarg_2_1_1_1_1_0, fwdarg_2_1_1_1_1_1);
                      });
                      local_div = fwdarg_2_1_1_1_0;
                      Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::fabs, std::vector<Type*>({fwdarg_2_1_1_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_1_1_0}));
                    });
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::floor, std::vector<Type*>({fwdarg_2_1_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_1_0}));
                  });
                  auto fwdarg_2_1_1 = local_div;
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::copysign, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0, fwdarg_2_1_1}));
                });
                Builder2.CreateFNeg(fwdarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&BO)), res);
        assert(res);
        Function *logFunc = BO.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = BO.getModule()->getModuleIdentifier() ;
            std::string functionName = BO.getFunction()->getName().str();
            std::string blockName = BO.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(BO.getModule()->begin(), BO.getModule()->end(),
              [&](const auto& func) { return &func == BO.getFunction(); });
            if (funcIt != BO.getModule()->end()) {
              funcIdx = std::distance(BO.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(BO.getFunction()->begin(), BO.getFunction()->end(),
              [&](const auto& block) { return &block == BO.getParent(); });
            if (blockIt != BO.getFunction()->end()) {
              blockIdx = std::distance(BO.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(BO.getParent()->begin(), BO.getParent()->end(),
              [&](const auto& curr) { return &curr == &BO; });
            if (instIt != BO.getParent()->end()) {
              instIdx = std::distance(BO.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&BO), Type::getDoubleTy(BO.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(BO.getContext()));
            std::string opcodeName = BO.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&BO)) {
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
        setDiffe(&BO, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&BO);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          dif = diffe(&BO, Builder2);
          setDiffe(&BO, Constant::getNullValue(gutils->getShadowType(BO.getType())), Builder2);
        }
        if (!gutils->isConstantValue(BO.getOperand(0)) && !isa<PointerType>(BO.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(0), toadd, Builder2, BO.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(BO.getOperand(1)) && !isa<PointerType>(BO.getOperand(1)->getType()) ) {
          llvm::Value *local_div = nullptr;
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FNeg
                auto revarg_2_0 = ({
                  // Computing copysign
                  auto revarg_2_1_0 = ({
                    // Computing floor
                    auto revarg_2_1_1_0 = ({
                      // Computing fabs
                      auto revarg_2_1_1_1_0 = ({
                        // Computing FDiv
                        llvm::Value* revarg_2_1_1_1_1_0 = lookup(gutils->getNewFromOriginal(BO.getOperand(0)), Builder2);
                        llvm::Value* revarg_2_1_1_1_1_1 = lookup(gutils->getNewFromOriginal(BO.getOperand(1)), Builder2);
                        Builder2.CreateFDiv(revarg_2_1_1_1_1_0, revarg_2_1_1_1_1_1);
                      });
                      local_div = revarg_2_1_1_1_0;
                      Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::fabs, std::vector<Type*>({revarg_2_1_1_1_0->getType()})), ArrayRef<Value*>({revarg_2_1_1_1_0}));
                    });
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::floor, std::vector<Type*>({revarg_2_1_1_0->getType()})), ArrayRef<Value*>({revarg_2_1_1_0}));
                  });
                  auto revarg_2_1_1 = local_div;
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::copysign, std::vector<Type*>({revarg_2_1_0->getType()})), ArrayRef<Value*>({revarg_2_1_0, revarg_2_1_1}));
                });
                Builder2.CreateFNeg(revarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = checkedMul(gutils->strongZero, Builder2, (gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(BO.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(BO.getOperand(1), toadd, Builder2, BO.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(BO));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(BO.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << BO << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(BO)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (BO).getDebugLoc(), &(BO), ss.str());
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
