/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

 case Intrinsic::tanh:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          llvm::Value *local_c = nullptr;
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ({
                  // Computing cosh
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0}));
                });
                local_c = fwdarg_2_0;
                auto fwdarg_2_1 = local_c;
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          llvm::Value *local_c = nullptr;
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ({
                  // Computing cosh
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0}));
                });
                local_c = fwdarg_2_0;
                auto fwdarg_2_1 = local_c;
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          llvm::Value *local_c = nullptr;
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                auto revarg_2_0 = ({
                  // Computing cosh
                  llvm::Value* revarg_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({revarg_2_1_0->getType()})), ArrayRef<Value*>({revarg_2_1_0}));
                });
                local_c = revarg_2_0;
                auto revarg_2_1 = local_c;
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::sinh:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing cosh
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing cosh
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing cosh
                llvm::Value* revarg_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cosh, std::vector<Type*>({revarg_2_0->getType()})), ArrayRef<Value*>({revarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::cosh:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing sinh
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sinh, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing sinh
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sinh, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing sinh
                llvm::Value* revarg_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sinh, std::vector<Type*>({revarg_2_0->getType()})), ArrayRef<Value*>({revarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::sin:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing cos
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cos, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing cos
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cos, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing cos
                llvm::Value* revarg_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::cos, std::vector<Type*>({revarg_2_0->getType()})), ArrayRef<Value*>({revarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::cos:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FNeg
                auto fwdarg_2_0 = ({
                  // Computing sin
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sin, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0}));
                });
                Builder2.CreateFNeg(fwdarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FNeg
                auto fwdarg_2_0 = ({
                  // Computing sin
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sin, std::vector<Type*>({fwdarg_2_1_0->getType()})), ArrayRef<Value*>({fwdarg_2_1_0}));
                });
                Builder2.CreateFNeg(fwdarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FNeg
                auto revarg_2_0 = ({
                  // Computing sin
                  llvm::Value* revarg_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sin, std::vector<Type*>({revarg_2_1_0->getType()})), ArrayRef<Value*>({revarg_2_1_0}));
                });
                Builder2.CreateFNeg(revarg_2_0);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::exp:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Call
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                auto callval = cast<CallInst>(&I)->getCalledOperand();
                auto FT = cast<CallInst>(&I)->getFunctionType();
                auto cconv = cast<CallInst>(&I)->getCallingConv();
                CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_0})));
                V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                V->setCallingConv(cconv);
                V;
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Call
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                auto callval = cast<CallInst>(&I)->getCalledOperand();
                auto FT = cast<CallInst>(&I)->getFunctionType();
                auto cconv = cast<CallInst>(&I)->getCallingConv();
                CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_0})));
                V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                V->setCallingConv(cconv);
                V;
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing Call
                llvm::Value* revarg_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                auto callval = cast<CallInst>(&I)->getCalledOperand();
                auto FT = cast<CallInst>(&I)->getFunctionType();
                auto cconv = cast<CallInst>(&I)->getCallingConv();
                CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_0})));
                V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                V->setCallingConv(cconv);
                V;
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::exp2:
 case Intrinsic::nvvm_ex2_approx_ftz_f:
 case Intrinsic::nvvm_ex2_approx_f:
 case Intrinsic::nvvm_ex2_approx_d:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                auto fwdarg_2_1 = ({
                  // Computing Call
                  llvm::Value* fwdarg_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                auto fwdarg_2_1 = ({
                  // Computing Call
                  llvm::Value* fwdarg_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                auto revarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                auto revarg_2_1 = ({
                  // Computing Call
                  llvm::Value* revarg_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_2_0})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::log:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::log2:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                llvm::Value* fwdarg_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                llvm::Value* fwdarg_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                auto revarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "0.6931471805599453");
                llvm::Value* revarg_2_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::log10:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2.302585092994046");
                llvm::Value* fwdarg_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2.302585092994046");
                llvm::Value* fwdarg_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                auto revarg_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2.302585092994046");
                llvm::Value* revarg_2_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::copysign:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ({
                  // Computing Call
                  auto fwdarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  llvm::Value* fwdarg_2_1_1 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_1_0, fwdarg_2_1_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                auto fwdarg_2_1 = ({
                  // Computing Call
                  auto fwdarg_2_2_0 = ConstantFP::get(I.getOperand(1)->getType(), "1.0");
                  llvm::Value* fwdarg_2_2_1 = gutils->getNewFromOriginal(I.getOperand(1));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0, fwdarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                auto fwdarg_2_0 = ({
                  // Computing Call
                  auto fwdarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  llvm::Value* fwdarg_2_1_1 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_1_0, fwdarg_2_1_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                auto fwdarg_2_1 = ({
                  // Computing Call
                  auto fwdarg_2_2_0 = ConstantFP::get(I.getOperand(1)->getType(), "1.0");
                  llvm::Value* fwdarg_2_2_1 = gutils->getNewFromOriginal(I.getOperand(1));
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0, fwdarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                auto revarg_2_0 = ({
                  // Computing Call
                  auto revarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  llvm::Value* revarg_2_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_1_0, revarg_2_1_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                auto revarg_2_1 = ({
                  // Computing Call
                  auto revarg_2_2_0 = ConstantFP::get(I.getOperand(1)->getType(), "1.0");
                  llvm::Value* revarg_2_2_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_2_0, revarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::minnum:
 case Intrinsic::nvvm_fmin_f:
 case Intrinsic::nvvm_fmin_d:
 case Intrinsic::nvvm_fmin_ftz_f:
 case Intrinsic::x86_sse2_min_pd:
 case Intrinsic::minimum:
 case Intrinsic::minimumnum:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
            Value *res = ({
              // Computing Select
              auto fwdnsrarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdnsrarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdnsrarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdnsrarg_1_0, fwdnsrarg_1_1);
              });
              auto fwdnsrarg_1 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(0);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(0), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(0)->getType());
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
              auto fwdnsrarg_2 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(1);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(1), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(1)->getType());
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
                Value *V = Builder2.CreateSelect(fwdnsrarg_0, (gutils->getWidth() == 1) ? fwdnsrarg_1 : gutils->extractMeta(Builder2, fwdnsrarg_1, idx), (gutils->getWidth() == 1) ? fwdnsrarg_2 : gutils->extractMeta(Builder2, fwdnsrarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
        assert(res);
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = dif;
              auto fwdarg_2 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(fwdarg_0, (gutils->getWidth() == 1) ? fwdarg_1 : gutils->extractMeta(Builder2, fwdarg_1, idx), fwdarg_2);
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(1))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(fwdarg_0, fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* revarg_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                llvm::Value* revarg_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                Builder2.CreateFCmpOLT(revarg_1_0, revarg_1_1);
              });
              auto revarg_1 = dif;
              auto revarg_2 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(revarg_0, (gutils->getWidth() == 1) ? revarg_1 : gutils->extractMeta(Builder2, revarg_1, idx), revarg_2);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* revarg_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                llvm::Value* revarg_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                Builder2.CreateFCmpOLT(revarg_1_0, revarg_1_1);
              });
              auto revarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto revarg_2 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(revarg_0, revarg_1, (gutils->getWidth() == 1) ? revarg_2 : gutils->extractMeta(Builder2, revarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(1), toadd, Builder2, I.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::maxnum:
 case Intrinsic::nvvm_fmax_f:
 case Intrinsic::nvvm_fmax_d:
 case Intrinsic::nvvm_fmax_ftz_f:
 case Intrinsic::x86_sse2_max_pd:
 case Intrinsic::maximum:
 case Intrinsic::maximumnum:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
            Value *res = ({
              // Computing Select
              auto fwdnsrarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdnsrarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdnsrarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdnsrarg_1_0, fwdnsrarg_1_1);
              });
              auto fwdnsrarg_1 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(1);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(1), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(1)->getType());
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
              auto fwdnsrarg_2 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(0);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(0), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(0)->getType());
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
                Value *V = Builder2.CreateSelect(fwdnsrarg_0, (gutils->getWidth() == 1) ? fwdnsrarg_1 : gutils->extractMeta(Builder2, fwdnsrarg_1, idx), (gutils->getWidth() == 1) ? fwdnsrarg_2 : gutils->extractMeta(Builder2, fwdnsrarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
        assert(res);
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(fwdarg_0, fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(1))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                llvm::Value* fwdarg_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                Builder2.CreateFCmpOLT(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = dif;
              auto fwdarg_2 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(fwdarg_0, (gutils->getWidth() == 1) ? fwdarg_1 : gutils->extractMeta(Builder2, fwdarg_1, idx), fwdarg_2);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* revarg_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                llvm::Value* revarg_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                Builder2.CreateFCmpOLT(revarg_1_0, revarg_1_1);
              });
              auto revarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto revarg_2 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(revarg_0, revarg_1, (gutils->getWidth() == 1) ? revarg_2 : gutils->extractMeta(Builder2, revarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing FCmpOLT
                llvm::Value* revarg_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                llvm::Value* revarg_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                Builder2.CreateFCmpOLT(revarg_1_0, revarg_1_1);
              });
              auto revarg_1 = dif;
              auto revarg_2 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateSelect(revarg_0, (gutils->getWidth() == 1) ? revarg_1 : gutils->extractMeta(Builder2, revarg_1, idx), revarg_2);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(1), toadd, Builder2, I.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::fabs:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Select
                auto fwdarg_2_0 = ({
                  // Computing FCmpOLT
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOLT(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "-1");
                auto fwdarg_2_2 = ConstantFP::get(I.getOperand(0)->getType(), "1");
                Builder2.CreateSelect(fwdarg_2_0, fwdarg_2_1, fwdarg_2_2);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Select
                auto fwdarg_2_0 = ({
                  // Computing FCmpOLT
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOLT(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "-1");
                auto fwdarg_2_2 = ConstantFP::get(I.getOperand(0)->getType(), "1");
                Builder2.CreateSelect(fwdarg_2_0, fwdarg_2_1, fwdarg_2_2);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing Select
                auto revarg_2_0 = ({
                  // Computing FCmpOLT
                  llvm::Value* revarg_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  auto revarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOLT(revarg_2_1_0, revarg_2_1_1);
                });
                auto revarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "-1");
                auto revarg_2_2 = ConstantFP::get(I.getOperand(0)->getType(), "1");
                Builder2.CreateSelect(revarg_2_0, revarg_2_1, revarg_2_2);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::sqrt:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpUEQ
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                auto fwdarg_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                Builder2.CreateFCmpUEQ(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = ({
                // Computing FDiv
                auto fwdarg_3_0 = dif;
                auto fwdarg_3_1 = ({
                  // Computing FMul
                  auto fwdarg_3_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2");
                  auto fwdarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* fwdarg_3_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_3_2_2_0})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
#if LLVM_VERSION_MAJOR >= 16
                    V->setOnlyReadsMemory();
V->setOnlyWritesMemory();
#elif LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::ReadNone);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::ReadNone);
#endif 
#if LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::NoUnwind);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::NoUnwind);
#endif 
                    V;
                  });
                  Builder2.CreateFMul(fwdarg_3_2_0, fwdarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_3_0 : gutils->extractMeta(Builder2, fwdarg_3_0, idx), fwdarg_3_1);
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
                Value *V = Builder2.CreateSelect(fwdarg_0, fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing FCmpUEQ
                llvm::Value* fwdarg_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                auto fwdarg_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                Builder2.CreateFCmpUEQ(fwdarg_1_0, fwdarg_1_1);
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = ({
                // Computing FDiv
                auto fwdarg_3_0 = dif;
                auto fwdarg_3_1 = ({
                  // Computing FMul
                  auto fwdarg_3_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2");
                  auto fwdarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* fwdarg_3_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_3_2_2_0})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
#if LLVM_VERSION_MAJOR >= 16
                    V->setOnlyReadsMemory();
V->setOnlyWritesMemory();
#elif LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::ReadNone);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::ReadNone);
#endif 
#if LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::NoUnwind);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::NoUnwind);
#endif 
                    V;
                  });
                  Builder2.CreateFMul(fwdarg_3_2_0, fwdarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_3_0 : gutils->extractMeta(Builder2, fwdarg_3_0, idx), fwdarg_3_1);
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
                Value *V = Builder2.CreateSelect(fwdarg_0, fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing FCmpUEQ
                llvm::Value* revarg_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                auto revarg_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                Builder2.CreateFCmpUEQ(revarg_1_0, revarg_1_1);
              });
              auto revarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto revarg_2 = ({
                // Computing FDiv
                auto revarg_3_0 = dif;
                auto revarg_3_1 = ({
                  // Computing FMul
                  auto revarg_3_2_0 = ConstantFP::get(I.getOperand(0)->getType(), "2");
                  auto revarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* revarg_3_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_3_2_2_0})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
#if LLVM_VERSION_MAJOR >= 16
                    V->setOnlyReadsMemory();
V->setOnlyWritesMemory();
#elif LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::ReadNone);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::ReadNone);
#endif 
#if LLVM_VERSION_MAJOR >= 14
                    V->addAttributeAtIndex(AttributeList::FunctionIndex, Attribute::NoUnwind);
#else 
                    V->addAttribute(AttributeList::FunctionIndex, Attribute::NoUnwind);
#endif 
                    V;
                  });
                  Builder2.CreateFMul(revarg_3_2_0, revarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_3_0 : gutils->extractMeta(Builder2, revarg_3_0, idx), revarg_3_1);
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
                Value *V = Builder2.CreateSelect(revarg_0, revarg_1, (gutils->getWidth() == 1) ? revarg_2 : gutils->extractMeta(Builder2, revarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::pow:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                auto fwdarg_2_1 = ({
                  // Computing Call
                  llvm::Value* fwdarg_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_2_1 = ({
                    // Computing FSub
                    llvm::Value* fwdarg_2_2_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto fwdarg_2_2_2_1 = ConstantFP::get(I.getOperand(1)->getType(), "1");
                    Builder2.CreateFSub(fwdarg_2_2_2_0, fwdarg_2_2_2_1);
                  });
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0, fwdarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Select
                auto fwdarg_2_0 = ({
                  // Computing FCmpOEQ
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOEQ(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                auto fwdarg_2_2 = ({
                  // Computing FMul
                  auto fwdarg_2_3_0 = ({
                    // Computing Call
                    llvm::Value* fwdarg_2_3_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    llvm::Value* fwdarg_2_3_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_3_1_0, fwdarg_2_3_1_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  auto fwdarg_2_3_1 = ({
                    // Computing log
                    llvm::Value* fwdarg_2_3_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::log, std::vector<Type*>({fwdarg_2_3_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_3_2_0}));
                  });
                  Builder2.CreateFMul(fwdarg_2_3_0, fwdarg_2_3_1);
                });
                Builder2.CreateSelect(fwdarg_2_0, fwdarg_2_1, fwdarg_2_2);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FMul
                llvm::Value* fwdarg_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                auto fwdarg_2_1 = ({
                  // Computing Call
                  llvm::Value* fwdarg_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_2_1 = ({
                    // Computing FSub
                    llvm::Value* fwdarg_2_2_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto fwdarg_2_2_2_1 = ConstantFP::get(I.getOperand(1)->getType(), "1");
                    Builder2.CreateFSub(fwdarg_2_2_2_0, fwdarg_2_2_2_1);
                  });
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_2_0, fwdarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(fwdarg_2_0, fwdarg_2_1);
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(1))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing CheckedMul
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing Select
                auto fwdarg_2_0 = ({
                  // Computing FCmpOEQ
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  auto fwdarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOEQ(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                auto fwdarg_2_2 = ({
                  // Computing FMul
                  auto fwdarg_2_3_0 = ({
                    // Computing Call
                    llvm::Value* fwdarg_2_3_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    llvm::Value* fwdarg_2_3_1_1 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_2_3_1_0, fwdarg_2_3_1_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  auto fwdarg_2_3_1 = ({
                    // Computing log
                    llvm::Value* fwdarg_2_3_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::log, std::vector<Type*>({fwdarg_2_3_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_3_2_0}));
                  });
                  Builder2.CreateFMul(fwdarg_2_3_0, fwdarg_2_3_1);
                });
                Builder2.CreateSelect(fwdarg_2_0, fwdarg_2_1, fwdarg_2_2);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FMul
                llvm::Value* revarg_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                auto revarg_2_1 = ({
                  // Computing Call
                  llvm::Value* revarg_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  auto revarg_2_2_1 = ({
                    // Computing FSub
                    llvm::Value* revarg_2_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                    auto revarg_2_2_2_1 = ConstantFP::get(I.getOperand(1)->getType(), "1");
                    Builder2.CreateFSub(revarg_2_2_2_0, revarg_2_2_2_1);
                  });
                  auto callval = cast<CallInst>(&I)->getCalledOperand();
                  auto FT = cast<CallInst>(&I)->getFunctionType();
                  auto cconv = cast<CallInst>(&I)->getCallingConv();
                  CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_2_0, revarg_2_2_1})));
                  V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                  V->setCallingConv(cconv);
                  V;
                });
                Builder2.CreateFMul(revarg_2_0, revarg_2_1);
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
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing CheckedMul
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing Select
                auto revarg_2_0 = ({
                  // Computing FCmpOEQ
                  llvm::Value* revarg_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  auto revarg_2_1_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                  Builder2.CreateFCmpOEQ(revarg_2_1_0, revarg_2_1_1);
                });
                auto revarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                auto revarg_2_2 = ({
                  // Computing FMul
                  auto revarg_2_3_0 = ({
                    // Computing Call
                    llvm::Value* revarg_2_3_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    llvm::Value* revarg_2_3_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_2_3_1_0, revarg_2_3_1_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  auto revarg_2_3_1 = ({
                    // Computing log
                    llvm::Value* revarg_2_3_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::log, std::vector<Type*>({revarg_2_3_2_0->getType()})), ArrayRef<Value*>({revarg_2_3_2_0}));
                  });
                  Builder2.CreateFMul(revarg_2_3_0, revarg_2_3_1);
                });
                Builder2.CreateSelect(revarg_2_0, revarg_2_1, revarg_2_2);
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
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(1), toadd, Builder2, I.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::powi:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing Or
                auto fwdarg_1_0 = ({
                  // Computing ICmpEQ
                  llvm::Value* fwdarg_1_1_0 = gutils->getNewFromOriginal(I.getOperand(1));
                  auto fwdarg_1_1_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 0);
                  Builder2.CreateICmpEQ(fwdarg_1_1_0, fwdarg_1_1_1);
                });
                auto fwdarg_1_1 = ({
                  // Computing And
                  auto fwdarg_1_2_0 = ConstantInt::get(Type::getInt1Ty(gutils->newFunc->getContext()), gutils->strongZero);
                  auto fwdarg_1_2_1 = ({
                    // Computing FCmpOEQ
                    auto fwdarg_1_2_2_0 = dif;
                    auto fwdarg_1_2_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                    Value *res = nullptr;
                    for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                      Value *V = Builder2.CreateFCmpOEQ((gutils->getWidth() == 1) ? fwdarg_1_2_2_0 : gutils->extractMeta(Builder2, fwdarg_1_2_2_0, idx), fwdarg_1_2_2_1);
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
                    Value *V = Builder2.CreateAnd(fwdarg_1_2_0, (gutils->getWidth() == 1) ? fwdarg_1_2_1 : gutils->extractMeta(Builder2, fwdarg_1_2_1, idx));
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
                  Value *V = Builder2.CreateOr(fwdarg_1_0, (gutils->getWidth() == 1) ? fwdarg_1_1 : gutils->extractMeta(Builder2, fwdarg_1_1, idx));
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = ({
                // Computing FMul
                auto fwdarg_3_0 = dif;
                auto fwdarg_3_1 = ({
                  // Computing FMul
                  auto fwdarg_3_2_0 = ({
                    // Computing SIToFP
                    llvm::Value* fwdarg_3_2_1_0 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto fwdarg_3_2_1_1 = I.getOperand(0)->getType();
                    Builder2.CreateSIToFP(fwdarg_3_2_1_0, fwdarg_3_2_1_1);
                  });
                  auto fwdarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* fwdarg_3_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    auto fwdarg_3_2_2_1 = ({
                      // Computing Sub
                      llvm::Value* fwdarg_3_2_2_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                      auto fwdarg_3_2_2_2_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 1);
                      Builder2.CreateSub(fwdarg_3_2_2_2_0, fwdarg_3_2_2_2_1);
                    });
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_3_2_2_0, fwdarg_3_2_2_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  Builder2.CreateFMul(fwdarg_3_2_0, fwdarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_3_0 : gutils->extractMeta(Builder2, fwdarg_3_0, idx), fwdarg_3_1);
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
                Value *V = Builder2.CreateSelect((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing Select
              auto fwdarg_0 = ({
                // Computing Or
                auto fwdarg_1_0 = ({
                  // Computing ICmpEQ
                  llvm::Value* fwdarg_1_1_0 = gutils->getNewFromOriginal(I.getOperand(1));
                  auto fwdarg_1_1_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 0);
                  Builder2.CreateICmpEQ(fwdarg_1_1_0, fwdarg_1_1_1);
                });
                auto fwdarg_1_1 = ({
                  // Computing And
                  auto fwdarg_1_2_0 = ConstantInt::get(Type::getInt1Ty(gutils->newFunc->getContext()), gutils->strongZero);
                  auto fwdarg_1_2_1 = ({
                    // Computing FCmpOEQ
                    auto fwdarg_1_2_2_0 = dif;
                    auto fwdarg_1_2_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                    Value *res = nullptr;
                    for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                      Value *V = Builder2.CreateFCmpOEQ((gutils->getWidth() == 1) ? fwdarg_1_2_2_0 : gutils->extractMeta(Builder2, fwdarg_1_2_2_0, idx), fwdarg_1_2_2_1);
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
                    Value *V = Builder2.CreateAnd(fwdarg_1_2_0, (gutils->getWidth() == 1) ? fwdarg_1_2_1 : gutils->extractMeta(Builder2, fwdarg_1_2_1, idx));
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
                  Value *V = Builder2.CreateOr(fwdarg_1_0, (gutils->getWidth() == 1) ? fwdarg_1_1 : gutils->extractMeta(Builder2, fwdarg_1_1, idx));
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
              auto fwdarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto fwdarg_2 = ({
                // Computing FMul
                auto fwdarg_3_0 = dif;
                auto fwdarg_3_1 = ({
                  // Computing FMul
                  auto fwdarg_3_2_0 = ({
                    // Computing SIToFP
                    llvm::Value* fwdarg_3_2_1_0 = gutils->getNewFromOriginal(I.getOperand(1));
                    auto fwdarg_3_2_1_1 = I.getOperand(0)->getType();
                    Builder2.CreateSIToFP(fwdarg_3_2_1_0, fwdarg_3_2_1_1);
                  });
                  auto fwdarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* fwdarg_3_2_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    auto fwdarg_3_2_2_1 = ({
                      // Computing Sub
                      llvm::Value* fwdarg_3_2_2_2_0 = gutils->getNewFromOriginal(I.getOperand(1));
                      auto fwdarg_3_2_2_2_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 1);
                      Builder2.CreateSub(fwdarg_3_2_2_2_0, fwdarg_3_2_2_2_1);
                    });
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({fwdarg_3_2_2_0, fwdarg_3_2_2_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  Builder2.CreateFMul(fwdarg_3_2_0, fwdarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_3_0 : gutils->extractMeta(Builder2, fwdarg_3_0, idx), fwdarg_3_1);
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
                Value *V = Builder2.CreateSelect((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1, (gutils->getWidth() == 1) ? fwdarg_2 : gutils->extractMeta(Builder2, fwdarg_2, idx));
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing Select
              auto revarg_0 = ({
                // Computing Or
                auto revarg_1_0 = ({
                  // Computing ICmpEQ
                  llvm::Value* revarg_1_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                  auto revarg_1_1_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 0);
                  Builder2.CreateICmpEQ(revarg_1_1_0, revarg_1_1_1);
                });
                auto revarg_1_1 = ({
                  // Computing And
                  auto revarg_1_2_0 = ConstantInt::get(Type::getInt1Ty(gutils->newFunc->getContext()), gutils->strongZero);
                  auto revarg_1_2_1 = ({
                    // Computing FCmpOEQ
                    auto revarg_1_2_2_0 = dif;
                    auto revarg_1_2_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
                    Value *res = nullptr;
                    for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                      Value *V = Builder2.CreateFCmpOEQ((gutils->getWidth() == 1) ? revarg_1_2_2_0 : gutils->extractMeta(Builder2, revarg_1_2_2_0, idx), revarg_1_2_2_1);
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
                    Value *V = Builder2.CreateAnd(revarg_1_2_0, (gutils->getWidth() == 1) ? revarg_1_2_1 : gutils->extractMeta(Builder2, revarg_1_2_1, idx));
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
                  Value *V = Builder2.CreateOr(revarg_1_0, (gutils->getWidth() == 1) ? revarg_1_1 : gutils->extractMeta(Builder2, revarg_1_1, idx));
                  if (gutils->getWidth() == 1) res = V;
                  else {
                    if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                    res = Builder2.CreateInsertValue(res, V, {idx});
                  }
                }
                res;
              });
              auto revarg_1 = ConstantFP::get(I.getOperand(0)->getType(), "0");
              auto revarg_2 = ({
                // Computing FMul
                auto revarg_3_0 = dif;
                auto revarg_3_1 = ({
                  // Computing FMul
                  auto revarg_3_2_0 = ({
                    // Computing SIToFP
                    llvm::Value* revarg_3_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                    auto revarg_3_2_1_1 = I.getOperand(0)->getType();
                    Builder2.CreateSIToFP(revarg_3_2_1_0, revarg_3_2_1_1);
                  });
                  auto revarg_3_2_1 = ({
                    // Computing Call
                    llvm::Value* revarg_3_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    auto revarg_3_2_2_1 = ({
                      // Computing Sub
                      llvm::Value* revarg_3_2_2_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
                      auto revarg_3_2_2_2_1 = ConstantInt::getSigned(I.getOperand(1)->getType(), 1);
                      Builder2.CreateSub(revarg_3_2_2_2_0, revarg_3_2_2_2_1);
                    });
                    auto callval = cast<CallInst>(&I)->getCalledOperand();
                    auto FT = cast<CallInst>(&I)->getFunctionType();
                    auto cconv = cast<CallInst>(&I)->getCallingConv();
                    CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({revarg_3_2_2_0, revarg_3_2_2_1})));
                    V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                    V->setCallingConv(cconv);
                    V;
                  });
                  Builder2.CreateFMul(revarg_3_2_0, revarg_3_2_1);
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_3_0 : gutils->extractMeta(Builder2, revarg_3_0, idx), revarg_3_1);
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
                Value *V = Builder2.CreateSelect((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1, (gutils->getWidth() == 1) ? revarg_2 : gutils->extractMeta(Builder2, revarg_2, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::fma:
 case Intrinsic::fmuladd:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(1));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(I.getOperand(2))) {
          Value *dif = diffe(I.getOperand(2), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        assert(res);
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(1));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(1))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FMul
              auto fwdarg_0 = dif;
              llvm::Value* fwdarg_1 = gutils->getNewFromOriginal(I.getOperand(0));
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        if (!gutils->isConstantValue(I.getOperand(2))) {
          Value *dif = diffe(I.getOperand(2), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(2))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(2)) && !isa<PointerType>(I.getOperand(2)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(I.getOperand(1)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FMul
              auto revarg_0 = dif;
              llvm::Value* revarg_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFMul((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(1), toadd, Builder2, I.getOperand(1)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(2)) && !isa<PointerType>(I.getOperand(2)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(2)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(2), toadd, Builder2, I.getOperand(2)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::vector_reduce_fadd:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
            Value *res = ({
              // Computing Call
              auto fwdnsrarg_0 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(0);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(0), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(0)->getType());
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
              auto fwdnsrarg_1 = ({
                // Computing SelectIfActive
                llvm::Value *imVal = I.getOperand(1);
                bool condition = !gutils->isConstantValue(imVal);
                bool vectorized = false;
                (void)vectorized;
                if (condition) {
                  imVal = gutils->invertPointerM(I.getOperand(1), Builder2, TR.query(&I));
                  vectorized = true;
                } else {
                  imVal = Constant::getNullValue(I.getOperand(1)->getType());
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
              auto callval = cast<CallInst>(&I)->getCalledOperand();
              auto FT = cast<CallInst>(&I)->getFunctionType();
              auto cconv = cast<CallInst>(&I)->getCallingConv();
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                CallInst *V = cast<CallInst>(Builder2.CreateCall(FT, callval, ArrayRef<Value*>({(gutils->getWidth() == 1) ? fwdnsrarg_0 : gutils->extractMeta(Builder2, fwdnsrarg_0, idx), (gutils->getWidth() == 1) ? fwdnsrarg_1 : gutils->extractMeta(Builder2, fwdnsrarg_1, idx)})));
                V->setDebugLoc(gutils->getNewFromOriginal(I.getDebugLoc()));
                V->setCallingConv(cconv);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
        assert(res);
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = dif;
            itmp = Builder2.CreateIntrinsic(Intrinsic::fabs, ArrayRef<Type*>(itmp->getType()), ArrayRef<Value*>(itmp));
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        if (!gutils->isConstantValue(I.getOperand(1))) {
          Value *dif = diffe(I.getOperand(1), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(1))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing VectorSplat
              auto fwdarg_0 = cast<VectorType>(I.getOperand(1)->getType())->getElementCount();
              auto fwdarg_1 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateVectorSplat(fwdarg_0, (gutils->getWidth() == 1) ? fwdarg_1 : gutils->extractMeta(Builder2, fwdarg_1, idx));
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        } else if (!dif && !gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = dif;
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        if (!gutils->isConstantValue(I.getOperand(1)) && !isa<PointerType>(I.getOperand(1)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing VectorSplat
              auto revarg_0 = cast<VectorType>(I.getOperand(1)->getType())->getElementCount();
              auto revarg_1 = dif;
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateVectorSplat(revarg_0, (gutils->getWidth() == 1) ? revarg_1 : gutils->extractMeta(Builder2, revarg_1, idx));
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(1)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(1), toadd, Builder2, I.getOperand(1)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::acos:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FNeg
              auto fwdarg_0 = ({
                // Computing FDiv
                auto fwdarg_1_0 = dif;
                auto fwdarg_1_1 = ({
                  // Computing sqrt
                  auto fwdarg_1_2_0 = ({
                    // Computing FSub
                    auto fwdarg_1_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                    auto fwdarg_1_2_1_1 = ({
                      // Computing FMul
                      llvm::Value* fwdarg_1_2_1_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                      llvm::Value* fwdarg_1_2_1_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                      Builder2.CreateFMul(fwdarg_1_2_1_2_0, fwdarg_1_2_1_2_1);
                    });
                    Builder2.CreateFSub(fwdarg_1_2_1_0, fwdarg_1_2_1_1);
                  });
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({fwdarg_1_2_0->getType()})), ArrayRef<Value*>({fwdarg_1_2_0}));
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_1_0 : gutils->extractMeta(Builder2, fwdarg_1_0, idx), fwdarg_1_1);
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
            arg_diff_tmp = GradientUtils::recursiveFAdd(Builder2,res, itmp, {}, {}, arg_diff_tmp, gutils->getWidth() != 1);
          }
          res = arg_diff_tmp;
        }
        assert(res);
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FNeg
              auto fwdarg_0 = ({
                // Computing FDiv
                auto fwdarg_1_0 = dif;
                auto fwdarg_1_1 = ({
                  // Computing sqrt
                  auto fwdarg_1_2_0 = ({
                    // Computing FSub
                    auto fwdarg_1_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                    auto fwdarg_1_2_1_1 = ({
                      // Computing FMul
                      llvm::Value* fwdarg_1_2_1_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                      llvm::Value* fwdarg_1_2_1_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                      Builder2.CreateFMul(fwdarg_1_2_1_2_0, fwdarg_1_2_1_2_1);
                    });
                    Builder2.CreateFSub(fwdarg_1_2_1_0, fwdarg_1_2_1_1);
                  });
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({fwdarg_1_2_0->getType()})), ArrayRef<Value*>({fwdarg_1_2_0}));
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_1_0 : gutils->extractMeta(Builder2, fwdarg_1_0, idx), fwdarg_1_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FNeg
              auto revarg_0 = ({
                // Computing FDiv
                auto revarg_1_0 = dif;
                auto revarg_1_1 = ({
                  // Computing sqrt
                  auto revarg_1_2_0 = ({
                    // Computing FSub
                    auto revarg_1_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                    auto revarg_1_2_1_1 = ({
                      // Computing FMul
                      llvm::Value* revarg_1_2_1_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                      llvm::Value* revarg_1_2_1_2_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                      Builder2.CreateFMul(revarg_1_2_1_2_0, revarg_1_2_1_2_1);
                    });
                    Builder2.CreateFSub(revarg_1_2_1_0, revarg_1_2_1_1);
                  });
                  Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({revarg_1_2_0->getType()})), ArrayRef<Value*>({revarg_1_2_0}));
                });
                Value *res = nullptr;
                for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                  Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_1_0 : gutils->extractMeta(Builder2, revarg_1_0, idx), revarg_1_1);
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
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::atan:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FAdd
                auto fwdarg_2_0 = ({
                  // Computing FMul
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  llvm::Value* fwdarg_2_1_1 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateFMul(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                Builder2.CreateFAdd(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing FAdd
                auto fwdarg_2_0 = ({
                  // Computing FMul
                  llvm::Value* fwdarg_2_1_0 = gutils->getNewFromOriginal(I.getOperand(0));
                  llvm::Value* fwdarg_2_1_1 = gutils->getNewFromOriginal(I.getOperand(0));
                  Builder2.CreateFMul(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                auto fwdarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                Builder2.CreateFAdd(fwdarg_2_0, fwdarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing FAdd
                auto revarg_2_0 = ({
                  // Computing FMul
                  llvm::Value* revarg_2_1_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  llvm::Value* revarg_2_1_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                  Builder2.CreateFMul(revarg_2_1_0, revarg_2_1_1);
                });
                auto revarg_2_1 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                Builder2.CreateFAdd(revarg_2_0, revarg_2_1);
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
 case Intrinsic::asin:
 {
    CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&I));
    IRBuilder<> BuilderZ(newCall);
    BuilderZ.setFastMathFlags(getFast());
    if (gutils->knownRecomputeHeuristic.find(&I) !=
        gutils->knownRecomputeHeuristic.end()) {
        if (!gutils->knownRecomputeHeuristic[&I]) {
          gutils->cacheForReverse(BuilderZ, newCall,
                                  getIndex(&I, CacheType::Self, BuilderZ));
        }
    }
    eraseIfUnused(I);
    if (gutils->isConstantInstruction(&I) && gutils->isConstantValue(&I))
      return true;
    switch (Mode) {
      case DerivativeMode::ForwardModeSplit:
      case DerivativeMode::ForwardMode:{
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing sqrt
                auto fwdarg_2_0 = ({
                  // Computing FSub
                  auto fwdarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  auto fwdarg_2_1_1 = ({
                    // Computing FMul
                    llvm::Value* fwdarg_2_1_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    llvm::Value* fwdarg_2_1_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                    Builder2.CreateFMul(fwdarg_2_1_2_0, fwdarg_2_1_2_1);
                  });
                  Builder2.CreateFSub(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ForwardModeError: {
        IRBuilder<> Builder2(&I);
        getForwardBuilder(Builder2);
        Value *res = Constant::getNullValue(gutils->getShadowType(I.getType()));
        if (!gutils->isConstantValue(I.getOperand(0))) {
          Value *dif = diffe(I.getOperand(0), Builder2);
          dif = Builder2.CreateFDiv(Builder2.CreateFMul(dif, gutils->getNewFromOriginal(I.getOperand(0))), gutils->getNewFromOriginal(&I));
          Value *arg_diff_tmp = UndefValue::get(res->getType());
          {
            Value *itmp = ({
              // Computing FDiv
              auto fwdarg_0 = dif;
              auto fwdarg_1 = ({
                // Computing sqrt
                auto fwdarg_2_0 = ({
                  // Computing FSub
                  auto fwdarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  auto fwdarg_2_1_1 = ({
                    // Computing FMul
                    llvm::Value* fwdarg_2_1_2_0 = gutils->getNewFromOriginal(I.getOperand(0));
                    llvm::Value* fwdarg_2_1_2_1 = gutils->getNewFromOriginal(I.getOperand(0));
                    Builder2.CreateFMul(fwdarg_2_1_2_0, fwdarg_2_1_2_1);
                  });
                  Builder2.CreateFSub(fwdarg_2_1_0, fwdarg_2_1_1);
                });
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({fwdarg_2_0->getType()})), ArrayRef<Value*>({fwdarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? fwdarg_0 : gutils->extractMeta(Builder2, fwdarg_0, idx), fwdarg_1);
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
        res = Builder2.CreateMaxNum(get1ULP(Builder2, gutils->getNewFromOriginal(&I)), res);
        assert(res);
        Function *logFunc = I.getModule()->getFunction("enzymeLogError");
        if (logFunc) {
            std::string moduleName = I.getModule()->getModuleIdentifier() ;
            std::string functionName = I.getFunction()->getName().str();
            std::string blockName = I.getParent()->getName().str();
            int funcIdx = -1, blockIdx = -1, instIdx = -1;
            auto funcIt = std::find_if(I.getModule()->begin(), I.getModule()->end(),
              [&](const auto& func) { return &func == I.getFunction(); });
            if (funcIt != I.getModule()->end()) {
              funcIdx = std::distance(I.getModule()->begin(), funcIt);
            }
            auto blockIt = std::find_if(I.getFunction()->begin(), I.getFunction()->end(),
              [&](const auto& block) { return &block == I.getParent(); });
            if (blockIt != I.getFunction()->end()) {
              blockIdx = std::distance(I.getFunction()->begin(), blockIt);
            }
            auto instIt = std::find_if(I.getParent()->begin(), I.getParent()->end(),
              [&](const auto& curr) { return &curr == &I; });
            if (instIt != I.getParent()->end()) {
              instIdx = std::distance(I.getParent()->begin(), instIt);
            }
            Value *origValue = Builder2.CreateFPExt(gutils->getNewFromOriginal(&I), Type::getDoubleTy(I.getContext()));
            Value *errValue = Builder2.CreateFPExt(res, Type::getDoubleTy(I.getContext()));
            std::string opcodeName = I.getOpcodeName();
            std::string calleeName = "<N/A>";
            if (auto CI = dyn_cast<CallInst>(&I)) {
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
        setDiffe(&I, res, Builder2);
        break;
      }
      case DerivativeMode::ReverseModeGradient:
      case DerivativeMode::ReverseModeCombined:{
        IRBuilder<> Builder2(&I);
        getReverseBuilder(Builder2);
        Value *dif = nullptr;
        if (!dif && !gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          dif = diffe(&I, Builder2);
          setDiffe(&I, Constant::getNullValue(gutils->getShadowType(I.getType())), Builder2);
        }
        if (!gutils->isConstantValue(I.getOperand(0)) && !isa<PointerType>(I.getOperand(0)->getType()) ) {
          Value *toadd = nullptr;
          {
            Value *tmp = ({
              // Computing FDiv
              auto revarg_0 = dif;
              auto revarg_1 = ({
                // Computing sqrt
                auto revarg_2_0 = ({
                  // Computing FSub
                  auto revarg_2_1_0 = ConstantFP::get(I.getOperand(0)->getType(), "1.0");
                  auto revarg_2_1_1 = ({
                    // Computing FMul
                    llvm::Value* revarg_2_1_2_0 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    llvm::Value* revarg_2_1_2_1 = lookup(gutils->getNewFromOriginal(I.getOperand(0)), Builder2);
                    Builder2.CreateFMul(revarg_2_1_2_0, revarg_2_1_2_1);
                  });
                  Builder2.CreateFSub(revarg_2_1_0, revarg_2_1_1);
                });
                Builder2.CreateCall(getIntrinsicDeclaration(mod, Intrinsic::sqrt, std::vector<Type*>({revarg_2_0->getType()})), ArrayRef<Value*>({revarg_2_0}));
              });
              Value *res = nullptr;
              for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
                Value *V = Builder2.CreateFDiv((gutils->getWidth() == 1) ? revarg_0 : gutils->extractMeta(Builder2, revarg_0, idx), revarg_1);
                if (gutils->getWidth() == 1) res = V;
                else {
                  if (idx == 0) res = UndefValue::get(ArrayType::get(V->getType(), gutils->getWidth()));
                  res = Builder2.CreateInsertValue(res, V, {idx});
                }
              }
              res;
            });
            Value *out = UndefValue::get(gutils->getShadowType(I.getOperand(0)->getType()));
            for(unsigned int idx=0, W=gutils->getWidth(); idx<W; idx++) {
              Value *prev = toadd ? (gutils->getWidth() == 1 ? toadd : gutils->extractMeta(Builder2, toadd, idx)) : nullptr;
              Value *next = tmp;
              if (gutils->getWidth() > 1) next = gutils->extractMeta(Builder2, next, idx);
              if (prev) next = Builder2.CreateFAdd(prev, next);
              out = (gutils->getWidth() > 1) ? Builder2.CreateInsertValue(out, next, idx) : next;
            }
            toadd = out;
          }
          if (toadd) addToDiffe(I.getOperand(0), toadd, Builder2, I.getOperand(0)->getType());
        }
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
      case DerivativeMode::ReverseModePrimal:{
        auto found = gutils->invertedPointers.find(&(I));
        if (found != gutils->invertedPointers.end() && !isa<PointerType>(I.getType())) {
          PHINode* PN = dyn_cast<PHINode>(&*found->second);
          if (!PN) {
            std::string str;
            raw_string_ostream ss(str);
            ss << "Shadow of instruction is not phi:\n";
            ss << *gutils->oldFunc << "\n";
            ss << *gutils->newFunc << "\n";
            ss << "orig: " << I << "\n";
            ss << "found: " << *found->second << "\n";
            if (CustomErrorHandler) {
              CustomErrorHandler(str.c_str(), wrap(&(I)), ErrorType::InternalError,
                                 nullptr, nullptr, nullptr);
            } else {
              EmitFailure("PHIError", (I).getDebugLoc(), &(I), ss.str());
            }
          }
          assert(PN);
          gutils->invertedPointers.erase(found);
          gutils->erase(PN);
        }
        break;
      }
    }
    return true;
  }
