/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

llvm::Constant* attribute_asum(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  if (!cublas) {
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyReadsMemory();
#else
  F->removeFnAttr(llvm::Attribute::ReadNone);
  F->addFnAttr(llvm::Attribute::ReadOnly);
#endif
  }
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
const bool cublasv2 = blas.prefix == "cublas" && llvm::StringRef(blas.suffix).contains("v2");
  if (cublasv2) argTys.push_back(getUnqual(fpType));
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  addFunctionNoCapture(F, 1 + offset);
  F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy())
    F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
  if (cublas) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::WriteOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  return res;
}
llvm::Constant* attribute_axpy(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  addFunctionNoCapture(F, 2 + offset);
  F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy())
    F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 4 + offset);
  return res;
}
llvm::Constant* attribute_copy(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  addFunctionNoCapture(F, 1 + offset);
  F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy())
    F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 3 + offset);
  return res;
}
llvm::Constant* attribute_dot(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  if (!cublas) {
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyReadsMemory();
#else
  F->removeFnAttr(llvm::Attribute::ReadNone);
  F->addFnAttr(llvm::Attribute::ReadOnly);
#endif
  }
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
const bool cublasv2 = blas.prefix == "cublas" && llvm::StringRef(blas.suffix).contains("v2");
  if (cublasv2) argTys.push_back(getUnqual(fpType));
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  addFunctionNoCapture(F, 1 + offset);
  F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy())
    F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  if (cublas) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::WriteOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  return res;
}
llvm::Constant* attribute_gemm(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(7 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(9 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(12 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy()) {
        F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 7 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(9 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(9 + offset)->isPointerTy()) {
        F->addParamAttr(9 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 9 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(10 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(10 + offset)->isPointerTy()) {
        F->addParamAttr(10 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 10 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(12 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(12 + offset)->isPointerTy()) {
        F->addParamAttr(12 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 12 + offset);
      }
  }
  addFunctionNoCapture(F, 6 + offset);
  F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy())
    F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 8 + offset);
  F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy())
    F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 11 + offset);
  return res;
}
llvm::Constant* attribute_gemv(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(7 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(10 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy()) {
        F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 7 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(10 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(10 + offset)->isPointerTy()) {
        F->addParamAttr(10 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 10 + offset);
      }
  }
  addFunctionNoCapture(F, 4 + offset);
  F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy())
    F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 6 + offset);
  F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy())
    F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 9 + offset);
  return res;
}
llvm::Constant* attribute_ger(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  return res;
}
llvm::Constant* attribute_lacpy(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  return res;
}
llvm::Constant* attribute_lascl(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  addFunctionNoCapture(F, 7 + offset);
  return res;
}
llvm::Constant* attribute_nrm2(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  if (!cublas) {
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyReadsMemory();
#else
  F->removeFnAttr(llvm::Attribute::ReadNone);
  F->addFnAttr(llvm::Attribute::ReadOnly);
#endif
  }
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
const bool cublasv2 = blas.prefix == "cublas" && llvm::StringRef(blas.suffix).contains("v2");
  if (cublasv2) argTys.push_back(getUnqual(fpType));
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  addFunctionNoCapture(F, 1 + offset);
  F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy())
    F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
  if (cublas) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::WriteOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  return res;
}
llvm::Constant* attribute_potrf(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  addFunctionNoCapture(F, 2 + offset);
  return res;
}
llvm::Constant* attribute_potrs(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  return res;
}
llvm::Constant* attribute_scal(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = (cublas ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  addFunctionNoCapture(F, 2 + offset);
  return res;
}
llvm::Constant* attribute_spmv(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  addFunctionNoCapture(F, 4 + offset);
  F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy())
    F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  return res;
}
llvm::Constant* attribute_spr2(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  return res;
}
llvm::Constant* attribute_symm(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(11 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(9 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(9 + offset)->isPointerTy()) {
        F->addParamAttr(9 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 9 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(11 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(11 + offset)->isPointerTy()) {
        F->addParamAttr(11 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 11 + offset);
      }
  }
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy())
    F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 10 + offset);
  return res;
}
llvm::Constant* attribute_symv(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(9 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy()) {
        F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 7 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(9 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(9 + offset)->isPointerTy()) {
        F->addParamAttr(9 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 9 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 8 + offset);
  return res;
}
llvm::Constant* attribute_syr2(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  addFunctionNoCapture(F, 3 + offset);
  F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy())
    F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  return res;
}
llvm::Constant* attribute_syr2k(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(11 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(9 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(9 + offset)->isPointerTy()) {
        F->addParamAttr(9 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 9 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(11 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(11 + offset)->isPointerTy()) {
        F->addParamAttr(11 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 11 + offset);
      }
  }
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy())
    F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 10 + offset);
  return res;
}
llvm::Constant* attribute_syrk(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(9 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy()) {
        F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 7 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(9 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(9 + offset)->isPointerTy()) {
        F->addParamAttr(9 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 9 + offset);
      }
  }
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 8 + offset);
  return res;
}
llvm::Constant* attribute_trmm(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(10 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(10 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(10 + offset)->isPointerTy()) {
        F->addParamAttr(10 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 10 + offset);
      }
  }
  addFunctionNoCapture(F, 7 + offset);
  F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy())
    F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 9 + offset);
  return res;
}
llvm::Constant* attribute_trmv(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(7 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy()) {
        F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 7 + offset);
      }
  }
  addFunctionNoCapture(F, 4 + offset);
  F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy())
    F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 6 + offset);
  return res;
}
llvm::Constant* attribute_trsm(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(5 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(10 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy()) {
        F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 5 + offset);
      }
  }
  if (byRefFloat) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(10 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(10 + offset)->isPointerTy()) {
        F->addParamAttr(10 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 10 + offset);
      }
  }
  addFunctionNoCapture(F, 7 + offset);
  F->removeParamAttr(7 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(7 + offset)->isPointerTy())
    F->addParamAttr(7 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 9 + offset);
  return res;
}
llvm::Constant* attribute_trtrs(BlasInfo blas, llvm::Function *F) {
  if (!F->empty())
    return F;
  llvm::Type *fpType = blas.fpType(F->getContext());
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
#if LLVM_VERSION_MAJOR >= 16
  F->setOnlyAccessesArgMemory();
#else
  F->addFnAttr(llvm::Attribute::ArgMemOnly);
#endif
  F->addFnAttr(llvm::Attribute::NoUnwind);
  F->addFnAttr(llvm::Attribute::NoRecurse);
#if LLVM_VERSION_MAJOR >= 14
  F->addFnAttr(llvm::Attribute::WillReturn);
  F->addFnAttr(llvm::Attribute::MustProgress);
#elif LLVM_VERSION_MAJOR >= 12
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::WillReturn);
  F->addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::MustProgress);
#endif
  F->addFnAttr(llvm::Attribute::NoFree);
  F->addFnAttr(llvm::Attribute::NoSync);
  F->addFnAttr("enzyme_no_escaping_allocation");
  const int offset = ((cblas || cublas) ? 1 : 0);
  llvm::Constant *res = F;
  llvm::SmallVector<llvm::Type*, 1> argTys;
  auto prevFT = F->getFunctionType();
  if (offset) argTys.push_back(prevFT->getParamType(0));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(llvm::isa<llvm::PointerType>(prevFT->getParamType(argTys.size())) ? prevFT->getParamType(argTys.size()) : getUnqual(fpType));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  argTys.push_back(prevFT->getParamType(argTys.size()));
  if (!cublas && !cblas) {
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  if (prevFT->getNumParams() > argTys.size())    argTys.push_back(prevFT->getParamType(argTys.size()));
  else    argTys.push_back(blas.intType(F->getContext()));
  F->addParamAttr(argTys.size()-1, llvm::Attribute::get(F->getContext(), llvm::Attribute::ZExt));
  }
  auto nextFT = llvm::FunctionType::get(prevFT->getReturnType(), argTys, false);
  if (nextFT != prevFT && F->empty()) {
    auto F2 = llvm::Function::Create(nextFT, F->getLinkage(), "", F->getParent());
    F->replaceAllUsesWith(llvm::ConstantExpr::getPointerCast(F2, F->getType()));
    res = llvm::ConstantExpr::getPointerCast(F2, F->getType());
    F2->copyAttributesFrom(F);
    llvm::SmallVector<std::pair<unsigned, llvm::MDNode *>, 1> MD;
    F->getAllMetadata(MD);
    for (auto pair : MD)
      F2->addMetadata(pair.first, *pair.second);
    F2->takeName(F);
    F2->setCallingConv(F->getCallingConv());
    F->eraseFromParent();
    F = F2;
  }
  F->addParamAttr(0 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(1 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(2 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(3 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(4 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(6 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  F->addParamAttr(8 + offset, llvm::Attribute::get(F->getContext(), "enzyme_inactive"));
  if (byRef) {
      F->removeParamAttr(0 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(0 + offset)->isPointerTy()) {
        F->addParamAttr(0 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 0 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(1 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(1 + offset)->isPointerTy()) {
        F->addParamAttr(1 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 1 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(2 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(2 + offset)->isPointerTy()) {
        F->addParamAttr(2 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 2 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(3 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(3 + offset)->isPointerTy()) {
        F->addParamAttr(3 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 3 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(4 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(4 + offset)->isPointerTy()) {
        F->addParamAttr(4 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 4 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(6 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(6 + offset)->isPointerTy()) {
        F->addParamAttr(6 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 6 + offset);
      }
  }
  if (byRef) {
      F->removeParamAttr(8 + offset, llvm::Attribute::ReadNone);
      if (F->getFunctionType()->getParamType(8 + offset)->isPointerTy()) {
        F->addParamAttr(8 + offset, llvm::Attribute::ReadOnly);
        addFunctionNoCapture(F, 8 + offset);
      }
  }
  addFunctionNoCapture(F, 5 + offset);
  F->removeParamAttr(5 + offset, llvm::Attribute::ReadNone);
  if (F->getFunctionType()->getParamType(5 + offset)->isPointerTy())
    F->addParamAttr(5 + offset, llvm::Attribute::ReadOnly);
  addFunctionNoCapture(F, 7 + offset);
  return res;
}
void attributeBLAS(BlasInfo blas, llvm::Function *F) {             
  if (!F->empty())
    return;
  if (blas.function == "asum") {                   
    attribute_asum(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "axpy") {                   
    attribute_axpy(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "copy") {                   
    attribute_copy(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "dot") {                   
    attribute_dot(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "gemm") {                   
    attribute_gemm(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "gemv") {                   
    attribute_gemv(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "ger") {                   
    attribute_ger(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "lacpy") {                   
    attribute_lacpy(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "lascl") {                   
    attribute_lascl(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "nrm2") {                   
    attribute_nrm2(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "potrf") {                   
    attribute_potrf(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "potrs") {                   
    attribute_potrs(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "scal") {                   
    attribute_scal(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "spmv") {                   
    attribute_spmv(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "spr2") {                   
    attribute_spr2(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "symm") {                   
    attribute_symm(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "symv") {                   
    attribute_symv(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "syr2") {                   
    attribute_syr2(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "syr2k") {                   
    attribute_syr2k(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "syrk") {                   
    attribute_syrk(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "trmm") {                   
    attribute_trmm(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "trmv") {                   
    attribute_trmv(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "trsm") {                   
    attribute_trsm(blas, F);                         
    return;                                                    
  }                                                            
  if (blas.function == "trtrs") {                   
    attribute_trtrs(blas, F);                         
    return;                                                    
  }                                                            
}                                                                
bool attributeTablegen(llvm::Function &F) {
  auto name = getFuncName(&F);
  auto changed = false;
  auto blasMetaData = extractBLAS(name);
  if (F.empty() && blasMetaData) {
    attributeBLAS(*blasMetaData, &F);
    changed = true;
  }
  if ((name == "__nv_frcp_rd" ||
      name == "__nv_frcp_rn" ||
      name == "__nv_frcp_ru" ||
      name == "__nv_frcp_rz" ||
      name == "__nv_drcp_rd" ||
      name == "__nv_drcp_rn" ||
      name == "__nv_drcp_ru" ||
      name == "__nv_drcp_rz") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "atan" ||
      name == "atanf" ||
      name == "atanl" ||
      name == "__fd_atan_1") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "atan2" ||
      name == "atan2f" ||
      name == "atan2l" ||
      name == "__nv_atan2" ||
      name == "__nv_atan2f" ||
      name == "__nv_atan2l") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cbrt" ||
      name == "cbrtf" ||
      name == "cbrtl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "hypot" ||
      name == "hypotf" ||
      name == "hypotl" ||
      name == "__hypot_finite") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "tanh") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "tanhf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "tanhl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cosh") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "coshf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "coshl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sinh") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sinhf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sinhl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "asinh" ||
      name == "asinhf" ||
      name == "asinhl" ||
      name == "__nv_asinh" ||
      name == "__nv_asinhf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "acosh" ||
      name == "acoshf" ||
      name == "acoshl" ||
      name == "__nv_acosh" ||
      name == "__nv_acoshf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "atanh" ||
      name == "atanhf" ||
      name == "atanhl" ||
      name == "__nv_atanh" ||
      name == "__nv_atanhf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "__nv_fast_expf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "exp10") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "tan" ||
      name == "tanf" ||
      name == "tanl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "remainder") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "asin" ||
      name == "asinf" ||
      name == "asinl" ||
      name == "__nv_asin" ||
      name == "__nv_asinf" ||
      name == "__nv_asinl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "acos" ||
      name == "acosf" ||
      name == "acosl" ||
      name == "__nv_acos" ||
      name == "__nv_acosf" ||
      name == "__nv_acosl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "log1p" ||
      name == "log1pf" ||
      name == "log1pl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "expm1" ||
      name == "expm1f" ||
      name == "expm1l") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "jl_rem2pi" ||
      name == "jl_rem2pif" ||
      name == "jl_rem2pil") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sinc" ||
      name == "sincf" ||
      name == "sincl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sincn" ||
      name == "sincnf" ||
      name == "sincnl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "Faddeeva_erfcx_re") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "fmod" ||
      name == "fmodf" ||
      name == "fmodl" ||
      name == "__nv_fmod" ||
      name == "__nv_fmodf" ||
      name == "__nv_fmodl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "modf" ||
      name == "modff" ||
      name == "modfl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadOnly));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadOnly));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "__fd_sincos_1" ||
      name == "__fd_sincos_1f" ||
      name == "__fd_sincos_1l") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sincospi" ||
      name == "sincospif" ||
      name == "sincospil") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "xlogy_jl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "tgamma") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "logabsgamma" ||
      name == "lgamma") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "logabsgammaf" ||
      name == "lgammaf") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "sinpi" ||
      name == "sinpif" ||
      name == "sinpil" ||
      name == "cospi" ||
      name == "cospif" ||
      name == "cospil") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "j0") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "j0f") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "y0") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "y0f") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "j1") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "j1f") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "y1") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "y1f") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "jn" ||
      name == "jnf" ||
      name == "yn" ||
      name == "ynf") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(0, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "cmplx_in") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cmplx_jn" ||
      name == "cmplx_yn") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "erf" ||
      name == "erff" ||
      name == "erfl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "erfi") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "erfc" ||
      name == "erfcf" ||
      name == "erfcl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "erfinv" ||
      name == "erfinvf" ||
      name == "erfinfvl" ||
      name == "__nv_erfinv" ||
      name == "__nv_erfinvf" ||
      name == "__nv_erfinfvl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "Faddeeva_erf") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "Faddeeva_erfi") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "Faddeeva_erfc") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "__mulsc3" ||
      name == "__muldc3" ||
      name == "__multc3" ||
      name == "__mulxc3") && F.getFunctionType()->getNumParams() == 4 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "__divsc3" ||
      name == "__divdc3" ||
      name == "__divtc3" ||
      name == "__divxc3") && F.getFunctionType()->getNumParams() == 4 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "scalbn" ||
      name == "scalbnf" ||
      name == "scalbnl" ||
      name == "scalbnln" ||
      name == "scalbnlnf" ||
      name == "scalbnlnl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "ldexp" ||
      name == "ldexpf" ||
      name == "ldexpl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "frexp" ||
      name == "frexpf" ||
      name == "frexpl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::WriteOnly));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::WriteOnly));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
    F.addParamAttr(1, llvm::Attribute::get(F.getContext(), "enzyme_inactive"));
  }
  if ((name == "cabs" ||
      name == "cabsf" ||
      name == "cabsl") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cabs" ||
      name == "cabsf" ||
      name == "cabsl") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cmplx_inv") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "fdim" ||
      name == "fdimf" ||
      name == "fdiml") && F.getFunctionType()->getNumParams() == 2 ){
    changed = true;
  #if LLVM_VERSION_MAJOR >= 16
    F.setOnlyReadsMemory();
    F.setOnlyWritesMemory();
  #elif LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::ReadNone));
  #endif 
  #if LLVM_VERSION_MAJOR >= 14
    F.addAttributeAtIndex(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #else 
    F.addAttribute(llvm::AttributeList::FunctionIndex, llvm::Attribute::get(F.getContext(), llvm::Attribute::NoUnwind));
  #endif 
  }
  if ((name == "cmplx_sqrt") && F.getFunctionType()->getNumParams() == 1 ){
    changed = true;
  }
  return changed;
}
