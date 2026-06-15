/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

bool handleBLAS(llvm::CallInst &call, llvm::Function *called,BlasInfo blas,const std::vector<bool> &overwritten_args) {         
  using llvm::Type;                                                
  bool result = true;                                              
  if (!gutils->isConstantInstruction(&call)) {                     
    Type *fpType = blas.fpType(call.getContext());                 
     if (blas.function == "axpy") {                           
      result = handle_axpy(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "copy") {                           
      result = handle_copy(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "dot") {                           
      result = handle_dot(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "gemm") {                           
      result = handle_gemm(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "gemv") {                           
      result = handle_gemv(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "lacpy") {                           
      result = handle_lacpy(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "nrm2") {                           
      result = handle_nrm2(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "potrf") {                           
      result = handle_potrf(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "potrs") {                           
      result = handle_potrs(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "scal") {                           
      result = handle_scal(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "spmv") {                           
      result = handle_spmv(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "symm") {                           
      result = handle_symm(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "symv") {                           
      result = handle_symv(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "syrk") {                           
      result = handle_syrk(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "trmm") {                           
      result = handle_trmm(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "trmv") {                           
      result = handle_trmv(blas, call, called, overwritten_args, fpType);                 
    } else  if (blas.function == "trtrs") {                           
      result = handle_trtrs(blas, call, called, overwritten_args, fpType);                 
    } else {                                                       
      return false;                                                
    }                                                              
  } else {                                                         
    auto found = gutils->knownRecomputeHeuristic.find(&call);      
    auto end = gutils->knownRecomputeHeuristic.end();              
    bool shouldErase = true;
    if (found != end) {
      if (!found->second) {                                        
       auto newCall = gutils->getNewFromOriginal(&call);
       llvm::IRBuilder<> BuilderZ(newCall);
       gutils->cacheForReverse(BuilderZ, newCall,
       getIndex(&call, CacheType::Self, BuilderZ));
       shouldErase = false;
      }
    }
    if (shouldErase) {
      if (Mode == DerivativeMode::ReverseModeGradient) {             
        eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
      } else {                                                       
        eraseIfUnused(call);                                         
      }                                                              
    }
  }
  return result;                                                   
}                                                                  
enum cublasOperation_t {
  CUBLAS_OP_N = 0,
  CUBLAS_OP_T = 1,
  CUBLAS_OP_C = 2,
};
enum cublasSideMode_t {
  CUBLAS_SIDE_LEFT = 0,
  CUBLAS_SIDE_RIGHT = 1,
};
enum cublasFillMode_t {
  CUBLAS_FILL_MODE_LOWER = 0,
  CUBLAS_FILL_MODE_UPPER = 1,
  CUBLAS_FILL_MODE_FULL = 2,
};

bool handle_axpy(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = (cublas ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

  const int pos_n = 0 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 1 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_x = 2 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 3 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_y = 4 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 5 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    Value *rt_inactive_out = nullptr;
    if (active_y) {
      rt_inactive_out = rt_inactive_y;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (active_y) {
      rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_out, "rt.inactive." "y");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "axpy" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = byRef ? (Type*) getInt8PtrTy(call.getContext()) : (Type*) Type::getInt8Ty(call.getContext());
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_x->isPointerTy();
  Type* type_vec_like = type_x;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x || active_alpha;
  bool need_alpha = active_x;
  bool need_x = active_alpha;
  bool need_incx = active_x || active_alpha;
  bool need_y = false;
  bool need_incy = active_x || active_alpha;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_x, Value *d_y  ) {
      Value *dres = nullptr;
        {
      // Seq
      if (d_x && d_y) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_alpha}) _0.push_back(item);
        for (auto item : {d_x, arg_incx}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_y && d_alpha) {
        {
      // BlasCall axpy
        std::vector<Value *>_1;
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {d_alpha}) _1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _1.push_back(item);
        for (auto item : {d_y, arg_incy}) _1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _1, Defs));
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_x, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "axpy" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_x, Value *d_y) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall dot
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, cache_x ? ValueType::Primal : ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, args1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, args1[args1.size()-1]);
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_x && d_x && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
      if (active_y) {
        Value *toadd = nullptr;
      }
    },
    d_x, d_y  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_copy(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = (cublas ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

  const int pos_n = 0 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_x = 1 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 2 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_y = 3 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 4 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    Value *rt_inactive_out = nullptr;
    if (active_y) {
      rt_inactive_out = rt_inactive_y;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (active_y) {
      rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_out, "rt.inactive." "y");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "copy" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = byRef ? (Type*) getInt8PtrTy(call.getContext()) : (Type*) Type::getInt8Ty(call.getContext());
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_x->isPointerTy();
  Type* type_vec_like = type_x;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_y;
  bool need_x = false;
  bool need_incx = active_y;
  bool need_y = false;
  bool need_incy = active_y;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_x, Value *d_y  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_x && d_y) {
        {
      // BlasCall copy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_n} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {arg_n} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _0.push_back(item);
        for (auto item : {d_x, arg_incx}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, _0, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_y) {
        {
      // BlasCall scal
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0")}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_x, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "copy" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_x, Value *d_y) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_x) {
        Value *toadd = nullptr;
      }
      if (active_y && d_x && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_y = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".y.active");
          nextBlock_y = gutils->addReverseBlock(activeBlock, bb_name + ".y.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_y, nextBlock_y, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, args1, Defs));
        if (nextBlock_y) {
          Builder2.CreateBr(nextBlock_y);
          Builder2.SetInsertPoint(nextBlock_y);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_y);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_y);
      }
        }
        }
      }
    },
    d_x, d_y  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_dot(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = (cublas ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

  const int pos_n = 0 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_x = 1 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 2 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_y = 3 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 4 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);

  if (cublasv2) {
    const int pos_ret = 5;
    const auto orig_ret = call.getArgOperand(pos_ret);
    auto arg_ret = gutils->getNewFromOriginal(orig_ret);
    const auto type_ret = arg_ret->getType();
    const bool overwritten_ret = (cacheMode ? overwritten_args[pos_ret] : false);
    bool active_ret = !gutils->isConstantValue(orig_ret);
    Value *rt_inactive_ret = nullptr;
  }


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "dot" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = byRef ? (Type*) getInt8PtrTy(call.getContext()) : (Type*) Type::getInt8Ty(call.getContext());
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_x->isPointerTy();
  Type* type_vec_like = type_x;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x || active_y;
  bool need_x = active_y;
  bool need_incx = active_x || active_y;
  bool need_y = active_x;
  bool need_incy = active_x || active_y;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_x, Value *d_y  ) {
      Value *dres = nullptr;
        {
      // FAdd
      Value *sub_0 = nullptr;
      if (d_x) {
        {
      // BlasCall dot
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_x, arg_incx}) _0.push_back(item);
        for (auto item : {arg_y, (cache_y ? const_one : arg_incy)}) _0.push_back(item);
        if (byRef) {
        }
           if (cublasv2) _0.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, _0, Defs));
        sub_0 = cubcall;
         if (cublasv2) sub_0 = Builder2.CreateLoad(fpType, _0[_0.size()-1]);
        }
        }
       if(sub_0 && dres) dres = Builder2.CreateFAdd(dres, sub_0);
       else if(sub_0) dres = sub_0;
      Value *sub_1 = nullptr;
      if (d_y) {
        {
      // BlasCall dot
        std::vector<Value *>_1;
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _1.push_back(item);
        for (auto item : {d_y, arg_incy}) _1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) _1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, _1, Defs));
        sub_1 = cubcall;
         if (cublasv2) sub_1 = Builder2.CreateLoad(fpType, _1[_1.size()-1]);
        }
        }
       if(sub_1 && dres) dres = Builder2.CreateFAdd(dres, sub_1);
       else if(sub_1) dres = sub_1;
         if (!dres) dres = ConstantFP::get(fpType, 0.0);
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_x, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "dot" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *dif = cublasv2 ? gutils->invertPointerM(call.getArgOperand(5 + offset), Builder2) : diffe(&call, Builder2);
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_x, Value *d_y, Value *dif) {
        if (byRef && !cublasv2) {
          Builder2.CreateStore(dif, alloc);
          dif = alloc;
        }
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_x && d_x) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {dif}) args1.push_back(item);
        for (auto item : {arg_y, (cache_y ? const_one : arg_incy)}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
      if (active_y && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_y = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".y.active");
          nextBlock_y = gutils->addReverseBlock(activeBlock, bb_name + ".y.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_y, nextBlock_y, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {dif}) args1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {cache_x ? ValueType::Primal : ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, args1, Defs));
        if (nextBlock_y) {
          Builder2.CreateBr(nextBlock_y);
          Builder2.SetInsertPoint(nextBlock_y);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_y);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_y);
      }
        }
        }
      }
    if (cublasv2) {
      auto mod = gutils->oldFunc->getParent();
      auto DL = mod->getDataLayout();
      Value* inps[] = { gutils->lookupM(dif, Builder2), Constant::getNullValue(Type::getInt32Ty(dif->getContext())), ConstantInt::get(Type::getInt64Ty(dif->getContext()), DL.getTypeSizeInBits(fpType) / 8) };
      Type *tys[] = { inps[0]->getType(), inps[1]->getType(), inps[2]->getType() };
      Builder2.CreateCall(mod->getOrInsertFunction("cudaMemset", FunctionType::get(Type::getVoidTy(dif->getContext()), tys, false)), inps);
   }
    },
    d_x, d_y, dif);
  if (!cublasv2)
    setDiffe(
      &call,
      Constant::getNullValue(gutils->getShadowType(call.getType())),
      Builder2);
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_gemm(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_transa = 0 + offset;
  const auto orig_transa = call.getArgOperand(pos_transa);
  auto arg_transa = gutils->getNewFromOriginal(orig_transa);
  const auto type_transa = arg_transa->getType();
  const bool overwritten_transa = (cacheMode ? overwritten_args[pos_transa] : false);

  const int pos_transb = 1 + offset;
  const auto orig_transb = call.getArgOperand(pos_transb);
  auto arg_transb = gutils->getNewFromOriginal(orig_transb);
  const auto type_transb = arg_transb->getType();
  const bool overwritten_transb = (cacheMode ? overwritten_args[pos_transb] : false);

  const int pos_m = 2 + offset;
  const auto orig_m = call.getArgOperand(pos_m);
  auto arg_m = gutils->getNewFromOriginal(orig_m);
  const auto type_m = arg_m->getType();
  const bool overwritten_m = (cacheMode ? overwritten_args[pos_m] : false);

  const int pos_n = 3 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_k = 4 + offset;
  const auto orig_k = call.getArgOperand(pos_k);
  auto arg_k = gutils->getNewFromOriginal(orig_k);
  const auto type_k = arg_k->getType();
  const bool overwritten_k = (cacheMode ? overwritten_args[pos_k] : false);

  const int pos_alpha = 5 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 6 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 7 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 8 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 9 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);

  const int pos_beta = 10 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_C = 11 + offset;
  const auto orig_C = call.getArgOperand(pos_C);
  auto arg_C = gutils->getNewFromOriginal(orig_C);
  const auto type_C = arg_C->getType();
  const bool overwritten_C = (cacheMode ? overwritten_args[pos_C] : false);
  bool active_C = !gutils->isConstantValue(orig_C);
  Value *rt_inactive_C = nullptr;

  const int pos_ldc = 12 + offset;
  const auto orig_ldc = call.getArgOperand(pos_ldc);
  auto arg_ldc = gutils->getNewFromOriginal(orig_ldc);
  const auto type_ldc = arg_ldc->getType();
  const bool overwritten_ldc = (cacheMode ? overwritten_args[pos_ldc] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      auto shadow_C = gutils->invertPointerM(orig_C, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_C = BuilderZ.CreateICmpEQ(shadow_C, arg_C, "rt.tmp.inactive." "C");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_C_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_C, i), arg_C, "rt.tmp.inactive." "C." + std::to_string(i));
          if (i == 0) rt_inactive_C = rt_inactive_C_tmp;
          else rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_C_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    Value *rt_inactive_out = nullptr;
    if (active_C) {
      rt_inactive_out = rt_inactive_C;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_out, "rt.inactive." "C");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "gemm" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_transa;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_transa;
  Type* blasIntType = type_m;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_transa = active_alpha || active_B || active_A;
  bool need_transb = active_alpha || active_B || active_A;
  bool need_m = active_alpha || active_beta || active_C || active_B || active_A;
  bool need_n = active_alpha || active_beta || active_C || active_B || active_A;
  bool need_k = active_alpha || active_B || active_A;
  bool need_alpha = active_B || active_A;
  bool need_A = active_alpha || active_B;
  bool need_lda = active_alpha || active_B || active_A;
  bool need_B = active_alpha || active_A;
  bool need_ldb = active_alpha || active_B || active_A;
  bool need_beta = active_C;
  bool need_C = false;
  bool need_ldc = active_alpha || active_beta || active_C || active_B || active_A;
  bool cache_transa = cacheMode && byRef && overwritten_transa && need_transa;
  bool cache_transb = cacheMode && byRef && overwritten_transb && need_transb;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_k = cacheMode && byRef && overwritten_k && need_k;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_C = cacheMode && overwritten_C && need_C;
  bool cache_ldc = cacheMode && byRef && overwritten_ldc && need_ldc;
  // we cache the following matrix,
  // since one rule uses input<C>
  if (active_beta) {
    need_C = true;
    cache_C = true;
  }
  if (cache_transa)
    cacheTypes.push_back(charType);
  if (cache_transb)
    cacheTypes.push_back(charType);
  if (cache_m)
    cacheTypes.push_back(intType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_k)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_ldc)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_C)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_transa, cache_transa, charType, cacheValues, BuilderZ, "transa");
        addValueToCache(arg_transb, cache_transb, charType, cacheValues, BuilderZ, "transb");
        addValueToCache(arg_m, cache_m, intType, cacheValues, BuilderZ, "m");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_k, cache_k, intType, cacheValues, BuilderZ, "k");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_ldc, cache_ldc, intType, cacheValues, BuilderZ, "ldc");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      Value *normal = is_normal(BuilderZ, arg_transa, byRef, cublas);
      M = BuilderZ.CreateSelect(normal, arg_m, arg_k);
      N = BuilderZ.CreateSelect(normal, arg_k, arg_m);
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      Value *normal = is_normal(BuilderZ, arg_transb, byRef, cublas);
      M = BuilderZ.CreateSelect(normal, arg_k, arg_n);
      N = BuilderZ.CreateSelect(normal, arg_n, arg_k);
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[10] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_C) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.C", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[12] = ValueType::Primal;
      if (byRef) valueTypes[13] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_C, arg_ldc, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldc, byRef);
        Value *args[5] = {malins, arg_C, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  Value *true_ldc = arg_ldc;
  Value *ldc = true_ldc;
  Value *free_C = nullptr;
  Value *input_C = nullptr;
  Value *free_input_C = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_transa) {
        arg_transa = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.transa");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.transa");
        Builder2.CreateStore(arg_transa, alloc);
        arg_transa = Builder2.CreatePointerCast(
            alloc, type_transa, "cast.transa");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_transb) {
        arg_transb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.transb");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.transb");
        Builder2.CreateStore(arg_transb, alloc);
        arg_transb = Builder2.CreatePointerCast(
            alloc, type_transb, "cast.transb");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_m) {
        arg_m = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.m");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.m");
        Builder2.CreateStore(arg_m, alloc);
        arg_m = Builder2.CreatePointerCast(
            alloc, type_m, "cast.m");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_k) {
        arg_k = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.k");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.k");
        Builder2.CreateStore(arg_k, alloc);
        arg_k = Builder2.CreatePointerCast(
            alloc, type_k, "cast.k");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldc) {
        arg_ldc = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldc");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldc");
        Builder2.CreateStore(arg_ldc, alloc);
        arg_ldc = Builder2.CreatePointerCast(
            alloc, type_ldc, "cast.ldc");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
    if (active_beta) {
      input_C = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.C");
      free_input_C = input_C;
      if (type_C->isIntegerTy()) {
        input_C = Builder2.CreatePtrToInt(input_C, type_C);
      } else if (input_C->getType() != type_C){
        input_C = Builder2.CreatePointerCast(input_C, type_C);
      }
    }
    if (cache_C) {
      arg_C = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.C");
      free_C = arg_C;
      if (type_C->isIntegerTy()) {
        arg_C = Builder2.CreatePtrToInt(arg_C, type_C);
      } else if (arg_C->getType() != type_C){
        arg_C = Builder2.CreatePointerCast(arg_C, type_C);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *d_C = active_C
     ? gutils->invertPointerM(orig_C, Builder2)
     : nullptr;
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_B, Value *d_beta, Value *d_C  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_C && d_beta) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item :             ({std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument within gemm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
            ArrayRef<Value*>(); })) _0.push_back(item);
        for (auto item : {d_beta}) _0.push_back(item);
        for (auto item : {arg_C}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_C && d_B) {
        {
      // BlasCall gemm
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_transa}) _1.push_back(item);
        for (auto item : {arg_transb}) _1.push_back(item);
        for (auto item : {arg_m}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_k}) _1.push_back(item);
        for (auto item : {arg_alpha}) _1.push_back(item);
        for (auto item : {arg_A}) _1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transa}, arg_lda, arg_k, arg_m, cache_A, byRef, cublas)}) _1.push_back(item);
        for (auto item : {d_B, arg_ldb}) _1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _1.push_back(item);
        for (auto item : {d_C, arg_ldc}) _1.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_1.size()) : intType, 1));
           _1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, _1, Defs));
        }
        }
      if (d_C && d_A) {
        {
      // BlasCall gemm
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_transa}) _2.push_back(item);
        for (auto item : {arg_transb}) _2.push_back(item);
        for (auto item : {arg_m}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_k}) _2.push_back(item);
        for (auto item : {arg_alpha}) _2.push_back(item);
        for (auto item : {d_A, arg_lda}) _2.push_back(item);
        for (auto item : {arg_B}) _2.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transb}, arg_ldb, arg_n, arg_k, cache_B, byRef, cublas)}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _2.push_back(item);
        for (auto item : {d_C, arg_ldc}) _2.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, _2, Defs));
        }
        }
      if (d_C && d_alpha) {
        {
      // BlasCall gemm
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : {arg_transa}) _3.push_back(item);
        for (auto item : {arg_transb}) _3.push_back(item);
        for (auto item : {arg_m}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {arg_k}) _3.push_back(item);
        for (auto item : {d_alpha}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transa}, arg_lda, arg_k, arg_m, cache_A, byRef, cublas)}) _3.push_back(item);
        for (auto item : {arg_B}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transb}, arg_ldb, arg_n, arg_k, cache_B, byRef, cublas)}) _3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _3.push_back(item);
        for (auto item : {d_C, arg_ldc}) _3.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_gemm->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, _3, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_C) {
        {
      // BlasCall lascl
        std::vector<Value *>_0;
        if (cblas) _0.push_back(arg_layout);
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {to_blas_callconv(Builder2, valueG, byRef, cublas, nullptr, allocationBuilder, "constant.char.G")}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _0.push_back(item);
        for (auto item : {arg_beta}) _0.push_back(item);
        for (auto item : {arg_m}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) _0.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           _0.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > _0.size() ) ? tmpF_lascl->getFunctionType()->getParamType(_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_B, d_beta, d_C);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "gemm" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_C = active_C
     ? lookup(gutils->invertPointerM(orig_C, Builder2), Builder2)
     : nullptr;
    if (!cache_transa && need_transa)
      arg_transa = lookup(arg_transa, Builder2);
    if (!cache_transb && need_transb)
      arg_transb = lookup(arg_transb, Builder2);
    if (!cache_m && need_m)
      arg_m = lookup(arg_m, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_k && need_k)
      arg_k = lookup(arg_k, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_C && need_C)
      arg_C = lookup(arg_C, Builder2);
    if (!cache_ldc && need_ldc)
      arg_ldc = lookup(arg_ldc, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_C) {
      rt_inactive_C = lookup(rt_inactive_C, Builder2);
    }
  }
    llvm::Value* arg_transposed_transa = nullptr;
    llvm::Value* arg_transposed_transb = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B, Value *d_C) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_C) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len1 = load_if_ref(Builder2, intType,arg_m, byRef);
    Value *len2 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_AB = Builder2.CreateNUWMul(len1, len2, "size_AB");
    CallInst * malloccall_mat_AB = nullptr;
    Value * true_mat_AB = CreateAllocation(Builder2, fpType, size_AB, "mat_AB", &malloccall_mat_AB);
    MDNode *ident_mat_AB = nullptr;
    if (malloccall_mat_AB) {
      ident_mat_AB = MDNode::getDistinct(malloccall_mat_AB->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_AB->getContext()))});
      malloccall_mat_AB->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_AB->getContext(), {ident_mat_AB}));
    }
    Value * mat_AB = true_mat_AB;
    if (type_vec_like->isIntegerTy()) {
      mat_AB = Builder2.CreatePtrToInt(mat_AB, type_vec_like);
    } else if (mat_AB->getType() != type_vec_like){
      mat_AB = Builder2.CreatePointerCast(mat_AB, type_vec_like);
    }
        {
      // BlasCall gemm
        std::vector<Value *>alpha_0;
        if (cblas) alpha_0.push_back(arg_layout);
        if (cublas) alpha_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_transa}) alpha_0.push_back(item);
        for (auto item : {arg_transb}) alpha_0.push_back(item);
        for (auto item : {arg_m}) alpha_0.push_back(item);
        for (auto item : {arg_n}) alpha_0.push_back(item);
        for (auto item : {arg_k}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) alpha_0.push_back(item);
        for (auto item : {arg_A}) alpha_0.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transa}, arg_lda, arg_k, arg_m, cache_A, byRef, cublas)}) alpha_0.push_back(item);
        for (auto item : {arg_B}) alpha_0.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_transb}, arg_ldb, arg_k, arg_n, cache_B, byRef, cublas)}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) alpha_0.push_back(item);
        for (auto item : {mat_AB}) alpha_0.push_back(item);
        for (auto item : {arg_m}) alpha_0.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           alpha_0.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > alpha_0.size() ) ? tmpF_gemm->getFunctionType()->getParamType(alpha_0.size()) : intType, 1));
           alpha_0.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > alpha_0.size() ) ? tmpF_gemm->getFunctionType()->getParamType(alpha_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, alpha_0, Defs));
        }
        {
      // FrobInnerProd
        std::vector<Value *>alpha_1;
        if (cublas) alpha_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_m}) alpha_1.push_back(item);
        for (auto item : {arg_n}) alpha_1.push_back(item);
        for (auto item : {d_C, arg_ldc}) alpha_1.push_back(item);
        for (auto item : {mat_AB}) alpha_1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) alpha_1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    auto derivcall_inner_prod = 
      getorInsertInnerProd(Builder2, *gutils->oldFunc->getParent(), blas, intType, type_vec_like, type_n, fpType, ArrayRef<Value *>(alpha_1), Defs, byRef, cublas, julia_decl);
        CallInst *cubcall = cast<CallInst>(derivcall_inner_prod);
        toadd = cubcall;
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_AB);
      if (freecall && ident_mat_AB) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_AB}));
      }
    }
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_C && d_A) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall gemm
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_transb}; auto concat_1 = {arg_transa}; auto concat_2 = {arg_k}; auto concat_3 = {arg_m}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2, concat_3); }); auto brow_1 = ({auto concat_0 = {arg_transa}; auto concat_1 = {(arg_transposed_transb = arg_transposed_transb ? arg_transposed_transb : transpose(blas.floatType, Builder2, arg_transb, byRef, cublas, charType, allocationBuilder, "transb"))}; auto concat_2 = {arg_m}; auto concat_3 = {arg_k}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2, concat_3); }); auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_B}; auto concat_1 = {get_cached_mat_width(Builder2, {arg_transb}, arg_ldb, arg_n, arg_k, cache_B, byRef, cublas)}; auto concat_2 = {d_C, arg_ldc}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2); }); auto brow_1 = ({auto concat_0 = {d_C, arg_ldc}; auto concat_1 = {arg_B}; auto concat_2 = {get_cached_mat_width(Builder2, {arg_transb}, arg_ldb, arg_n, arg_k, cache_B, byRef, cublas)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2); }); auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_A, arg_lda}) args1.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, args1, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_B && d_C && d_B) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall gemm
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_transb}; auto concat_1 = {arg_transa}; auto concat_2 = {arg_n}; auto concat_3 = {arg_k}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2, concat_3); }); auto brow_1 = ({auto concat_0 = {(arg_transposed_transa = arg_transposed_transa ? arg_transposed_transa : transpose(blas.floatType, Builder2, arg_transa, byRef, cublas, charType, allocationBuilder, "transa"))}; auto concat_1 = {arg_transb}; auto concat_2 = {arg_k}; auto concat_3 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2, concat_3); }); auto brow_0 = {arg_transb}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : ({auto brow_2 = ({auto concat_0 = {d_C, arg_ldc}; auto concat_1 = {arg_A}; auto concat_2 = {get_cached_mat_width(Builder2, {arg_transa}, arg_lda, arg_k, arg_m, cache_A, byRef, cublas)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2); }); auto brow_1 = ({auto concat_0 = {arg_A}; auto concat_1 = {get_cached_mat_width(Builder2, {arg_transa}, arg_lda, arg_k, arg_m, cache_A, byRef, cublas)}; auto concat_2 = {d_C, arg_ldc}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1, concat_2); }); auto brow_0 = {arg_transb}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
      if (active_beta && d_C) {
        Value *toadd = nullptr;
        {
      // FrobInnerProd
        BasicBlock *nextBlock_beta = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".beta.active");
          nextBlock_beta = gutils->addReverseBlock(activeBlock, bb_name + ".beta.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_beta, nextBlock_beta, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_C, arg_ldc}) args1.push_back(item);
        for (auto item : {input_C}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    auto derivcall_inner_prod = 
      getorInsertInnerProd(Builder2, *gutils->oldFunc->getParent(), blas, intType, type_vec_like, type_n, fpType, ArrayRef<Value *>(args1), Defs, byRef, cublas, julia_decl);
        CallInst *cubcall = cast<CallInst>(derivcall_inner_prod);
        toadd = cubcall;
        if (nextBlock_beta && byRefFloat) {
          Builder2.CreateBr(nextBlock_beta);
          Builder2.SetInsertPoint(nextBlock_beta);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_beta);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_beta);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_C && d_C) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_C = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".C.active");
          nextBlock_C = gutils->addReverseBlock(activeBlock, bb_name + ".C.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_C, nextBlock_C, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall lascl
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {to_blas_callconv(Builder2, valueG, byRef, cublas, nullptr, allocationBuilder, "constant.char.G")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_C, arg_ldc}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_lascl->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, args1, Defs));
        if (nextBlock_C) {
          Builder2.CreateBr(nextBlock_C);
          Builder2.SetInsertPoint(nextBlock_C);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_C);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_C);
      }
        }
        }
      }
    },
    d_A, d_B, d_C  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_C) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_C);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_gemv(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_transa = 0 + offset;
  const auto orig_transa = call.getArgOperand(pos_transa);
  auto arg_transa = gutils->getNewFromOriginal(orig_transa);
  const auto type_transa = arg_transa->getType();
  const bool overwritten_transa = (cacheMode ? overwritten_args[pos_transa] : false);

  const int pos_m = 1 + offset;
  const auto orig_m = call.getArgOperand(pos_m);
  auto arg_m = gutils->getNewFromOriginal(orig_m);
  const auto type_m = arg_m->getType();
  const bool overwritten_m = (cacheMode ? overwritten_args[pos_m] : false);

  const int pos_n = 2 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 3 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 4 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 5 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_x = 6 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 7 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_beta = 8 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_y = 9 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 10 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    Value *rt_inactive_out = nullptr;
    if (active_y) {
      rt_inactive_out = rt_inactive_y;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_out, "rt.inactive." "y");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "gemv" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_transa;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_transa;
  Type* blasIntType = type_m;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_transa = active_alpha || active_A || active_beta || active_x || active_y;
  bool need_m = active_alpha || active_A || active_beta || active_x || active_y;
  bool need_n = active_alpha || active_A || active_beta || active_x || active_y;
  bool need_alpha = active_A || active_x;
  bool need_A = active_alpha || active_x;
  bool need_lda = active_alpha || active_A || active_x;
  bool need_x = active_alpha || active_A;
  bool need_incx = active_alpha || active_A || active_x;
  bool need_beta = active_y;
  bool need_y = false;
  bool need_incy = active_alpha || active_A || active_beta || active_x || active_y;
  bool cache_transa = cacheMode && byRef && overwritten_transa && need_transa;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  // we cache the following matrix,
  // since one rule uses input<y>
  if (active_beta) {
    need_y = true;
    cache_y = true;
  }
  if (cache_transa)
    cacheTypes.push_back(charType);
  if (cache_m)
    cacheTypes.push_back(intType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_transa, cache_transa, charType, cacheValues, BuilderZ, "transa");
        addValueToCache(arg_m, cache_m, intType, cacheValues, BuilderZ, "m");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      auto norm = is_normal(BuilderZ, arg_transa, byRef, cublas);
      malloc_size = CreateSelect(BuilderZ, norm, arg_n, arg_m);
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      auto norm = is_normal(BuilderZ, arg_transa, byRef, cublas);
      malloc_size = CreateSelect(BuilderZ, norm, arg_m, arg_n);
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[10] = ValueType::Primal;
      if (byRef) valueTypes[11] = ValueType::Primal;
      if (byRef) valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  Value *input_y = nullptr;
  Value *free_input_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_transa) {
        arg_transa = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.transa");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.transa");
        Builder2.CreateStore(arg_transa, alloc);
        arg_transa = Builder2.CreatePointerCast(
            alloc, type_transa, "cast.transa");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_m) {
        arg_m = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.m");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.m");
        Builder2.CreateStore(arg_m, alloc);
        arg_m = Builder2.CreatePointerCast(
            alloc, type_m, "cast.m");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (active_beta) {
      input_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_input_y = input_y;
      if (type_y->isIntegerTy()) {
        input_y = Builder2.CreatePtrToInt(input_y, type_y);
      } else if (input_y->getType() != type_y){
        input_y = Builder2.CreatePointerCast(input_y, type_y);
      }
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_x, Value *d_beta, Value *d_y  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_beta && d_y) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : ({auto brow_2 = {arg_n}; auto brow_1 = {arg_m}; auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) _0.push_back(item);
        for (auto item : {d_beta}) _0.push_back(item);
        for (auto item : {arg_y, (cache_y ? const_one : arg_incy)}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_x && d_y) {
        {
      // BlasCall gemv
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_transa}) _1.push_back(item);
        for (auto item : {arg_m}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_alpha}) _1.push_back(item);
        for (auto item : {arg_A}) _1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_m, arg_m, cache_A, byRef, cublas)}) _1.push_back(item);
        for (auto item : {d_x, arg_incx}) _1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _1.push_back(item);
        for (auto item : {d_y, arg_incy}) _1.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_gemv->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, _1, Defs));
        }
        }
      if (d_A && d_y) {
        {
      // BlasCall gemv
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_transa}) _2.push_back(item);
        for (auto item : {arg_m}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_alpha}) _2.push_back(item);
        for (auto item : {d_A, arg_lda}) _2.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _2.push_back(item);
        for (auto item : {d_y, arg_incy}) _2.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_gemv->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, _2, Defs));
        }
        }
      if (d_y && d_alpha) {
        {
      // BlasCall gemv
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : {arg_transa}) _3.push_back(item);
        for (auto item : {arg_m}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {d_alpha}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_m, arg_m, cache_A, byRef, cublas)}) _3.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _3.push_back(item);
        for (auto item : {d_y, arg_incy}) _3.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_gemv->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, _3, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_y) {
        {
      // BlasCall scal
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : ({auto brow_2 = {arg_n}; auto brow_1 = {arg_m}; auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) _0.push_back(item);
        for (auto item : {arg_beta}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_x, d_beta, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "gemv" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_transa && need_transa)
      arg_transa = lookup(arg_transa, Builder2);
    if (!cache_m && need_m)
      arg_m = lookup(arg_m, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    llvm::Value* arg_transposed_transa = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_x, Value *d_y) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_y) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len1 = load_if_ref(Builder2, intType,arg_m, byRef);
    Value *len2 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_Ax = Builder2.CreateSelect(is_normal(Builder2, arg_transa, byRef, cublas), len1, len2);
    CallInst * malloccall_mat_Ax = nullptr;
    Value * true_mat_Ax = CreateAllocation(Builder2, fpType, size_Ax, "mat_Ax", &malloccall_mat_Ax);
    MDNode *ident_mat_Ax = nullptr;
    if (malloccall_mat_Ax) {
      ident_mat_Ax = MDNode::getDistinct(malloccall_mat_Ax->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_Ax->getContext()))});
      malloccall_mat_Ax->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_Ax->getContext(), {ident_mat_Ax}));
    }
    Value * mat_Ax = true_mat_Ax;
    if (type_vec_like->isIntegerTy()) {
      mat_Ax = Builder2.CreatePtrToInt(mat_Ax, type_vec_like);
    } else if (mat_Ax->getType() != type_vec_like){
      mat_Ax = Builder2.CreatePointerCast(mat_Ax, type_vec_like);
    }
        {
      // BlasCall gemv
        std::vector<Value *>alpha_0;
        if (cblas) alpha_0.push_back(arg_layout);
        if (cublas) alpha_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_transa}) alpha_0.push_back(item);
        for (auto item : {arg_m}) alpha_0.push_back(item);
        for (auto item : {arg_n}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) alpha_0.push_back(item);
        for (auto item : {arg_A}) alpha_0.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_m, arg_m, cache_A, byRef, cublas)}) alpha_0.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) alpha_0.push_back(item);
        for (auto item : {mat_Ax}) alpha_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_0.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           alpha_0.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > alpha_0.size() ) ? tmpF_gemv->getFunctionType()->getParamType(alpha_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, alpha_0, Defs));
        }
        {
      // BlasCall dot
        std::vector<Value *>alpha_1;
        if (cublas) alpha_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = {arg_n}; auto brow_1 = {arg_m}; auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) alpha_1.push_back(item);
        for (auto item : {d_y, arg_incy}) alpha_1.push_back(item);
        for (auto item : {mat_Ax}) alpha_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) alpha_1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, alpha_1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, alpha_1[alpha_1.size()-1]);
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_Ax);
      if (freecall && ident_mat_Ax) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_Ax}));
      }
    }
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_y && d_A) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall ger
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_x, (cache_x ? const_one : arg_incx)}; auto concat_1 = {d_y, arg_incy}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_1 = ({auto concat_0 = {d_y, arg_incy}; auto concat_1 = {arg_x, (cache_x ? const_one : arg_incx)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {d_A, arg_lda}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTger = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_ger = blas.prefix + blas.floatType + "ger" + blas.suffix;
    auto derivcall_ger = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_ger), FTger);
    if (auto F = dyn_cast<Function>(derivcall_ger.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_ger);
      auto newF = attribute_ger(blas, F);
      derivcall_ger = FunctionCallee(derivcall_ger.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_ger, args1, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_x && d_x && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall gemv
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {(arg_transposed_transa = arg_transposed_transa ? arg_transposed_transa : transpose(blas.floatType, Builder2, arg_transa, byRef, cublas, charType, allocationBuilder, "transa"))}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_m, arg_m, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_gemv->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
      if (active_beta && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_beta = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".beta.active");
          nextBlock_beta = gutils->addReverseBlock(activeBlock, bb_name + ".beta.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_beta, nextBlock_beta, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall dot
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = {arg_n}; auto brow_1 = {arg_m}; auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {input_y, (cache_y ? const_one : arg_incy)}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, args1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, args1[args1.size()-1]);
        if (nextBlock_beta && byRefFloat) {
          Builder2.CreateBr(nextBlock_beta);
          Builder2.SetInsertPoint(nextBlock_beta);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_beta);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_beta);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_y && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_y = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".y.active");
          nextBlock_y = gutils->addReverseBlock(activeBlock, bb_name + ".y.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_y, nextBlock_y, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall scal
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = {arg_n}; auto brow_1 = {arg_m}; auto brow_0 = {arg_transa}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, args1, Defs));
        if (nextBlock_y) {
          Builder2.CreateBr(nextBlock_y);
          Builder2.SetInsertPoint(nextBlock_y);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_y);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_y);
      }
        }
        }
      }
    },
    d_A, d_x, d_y  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_lacpy(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_m = 1 + offset;
  const auto orig_m = call.getArgOperand(pos_m);
  auto arg_m = gutils->getNewFromOriginal(orig_m);
  const auto type_m = arg_m->getType();
  const bool overwritten_m = (cacheMode ? overwritten_args[pos_m] : false);

  const int pos_n = 2 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_A = 3 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 4 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 5 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 6 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    Value *rt_inactive_out = nullptr;
    if (active_B) {
      rt_inactive_out = rt_inactive_B;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "lacpy" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_m;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_B || active_A;
  bool need_m = active_B || active_A;
  bool need_n = active_B || active_A;
  bool need_A = false;
  bool need_lda = active_A;
  bool need_B = false;
  bool need_ldb = active_B || active_A;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_m)
    cacheTypes.push_back(intType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_m, cache_m, intType, cacheValues, BuilderZ, "m");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_m) {
        arg_m = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.m");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.m");
        Builder2.CreateStore(arg_m, alloc);
        arg_m = Builder2.CreatePointerCast(
            alloc, type_m, "cast.m");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_A, Value *d_B  ) {
      Value *dres = nullptr;
        {
      // Seq
      if (d_B && d_A) {
        {
      // BlasCall lacpy
        std::vector<Value *>_0;
        if (cblas) _0.push_back(arg_layout);
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_uplo}) _0.push_back(item);
        for (auto item : {arg_m}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_A, arg_lda}) _0.push_back(item);
        for (auto item : {d_B, arg_ldb}) _0.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           _0.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > _0.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, _0, Defs));
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_A, d_B);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "lacpy" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_m && need_m)
      arg_m = lookup(arg_m, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_A && d_B && d_A) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // MemcpyMatAdd
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_A, arg_lda}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
        args1[0] = load_if_ref(Builder2, charType, args1[0], byRef);
        args1[1] = load_if_ref(Builder2, intType, args1[1], byRef);
        args1[2] = load_if_ref(Builder2, intType, args1[2], byRef);
        args1[4] = load_if_ref(Builder2, intType, args1[4], byRef);
        args1[6] = load_if_ref(Builder2, intType, args1[6], byRef);
        auto dmemcpymat = getOrInsertDifferentialFloatMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(args1[args1.size() - 2]->getType()), intType, charType, 0, 0, 1);
    auto cubcall = cast<CallInst>(Builder2.CreateCall(dmemcpymat, args1, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_B && d_B && !active_A) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall lascl
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_lascl->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
    },
    d_A, d_B  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_nrm2(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = (cublas ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

  const int pos_n = 0 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_x = 1 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 2 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  if (cublasv2) {
    const int pos_ret = 3;
    const auto orig_ret = call.getArgOperand(pos_ret);
    auto arg_ret = gutils->getNewFromOriginal(orig_ret);
    const auto type_ret = arg_ret->getType();
    const bool overwritten_ret = (cacheMode ? overwritten_args[pos_ret] : false);
    bool active_ret = !gutils->isConstantValue(orig_ret);
    Value *rt_inactive_ret = nullptr;
  }


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "nrm2" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = byRef ? (Type*) getInt8PtrTy(call.getContext()) : (Type*) Type::getInt8Ty(call.getContext());
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_x->isPointerTy();
  Type* type_vec_like = type_x;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x;
  bool need_x = active_x;
  bool need_incx = active_x;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None};
      valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_x  ) {
      Value *dres = nullptr;
        {
      // BFDiv
      Value *subnum = nullptr;
      Value *subdenom = nullptr;
      if (d_x) {
        {
      // BlasCall dot
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _0.push_back(item);
        for (auto item : {d_x, arg_incx}) _0.push_back(item);
        if (byRef) {
        }
           if (cublasv2) _0.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, _0, Defs));
        subnum = cubcall;
         if (cublasv2) subnum = Builder2.CreateLoad(fpType, _0[_0.size()-1]);
        }
        }
       if(subnum) {
        SmallVector<Value*, 1> subdenomar;        for (auto item : ({      // BlasCall nrm2 (Arg)
        std::vector<Value *>marg;
        if (cublas) marg.push_back(arg_handle);
        for (auto item : {arg_n}) marg.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) marg.push_back(item);
        if (byRef) {
        }
           if (cublasv2) marg.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {cache_x ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : marg) tys.push_back(arg->getType());
    llvm::FunctionType *FTnrm2 = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_nrm2 = blas.prefix + blas.floatType + "nrm2" + blas.suffix;
    auto derivcall_nrm2 = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_nrm2), FTnrm2);
    if (auto F = dyn_cast<Function>(derivcall_nrm2.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_nrm2);
      auto newF = attribute_nrm2(blas, F);
      derivcall_nrm2 = FunctionCallee(derivcall_nrm2.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_nrm2, marg, Defs));
         SmallVector<Value*, 1> resvec(1, cublasv2 ?  (Value*)Builder2.CreateLoad(fpType, marg[marg.size()-1]) : (Value*)cubcall);
         resvec[0] = to_blas_fp_callconv(Builder2, resvec[0], byRefFloat, blasFPType, allocationBuilder, "blascall");
         resvec;
 })
) subdenomar.push_back(item);
           assert(subdenomar.size() == 1);
           subdenom = subdenomar[0];
           subdenom = load_if_ref(Builder2, fpType, subdenom, byRefFloat);
           assert(subnum);
           assert(subdenom);
dres = Builder2.CreateFDiv(subnum, subdenom);
         } else dres = ConstantFP::get(fpType, 0.0);
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_x);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "nrm2" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *dif = cublasv2 ? gutils->invertPointerM(call.getArgOperand(3 + offset), Builder2) : diffe(&call, Builder2);
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_x, Value *dif) {
        if (byRef && !cublasv2) {
          Builder2.CreateStore(dif, alloc);
          dif = alloc;
        }
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_x && d_x) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {dif} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({      // BlasCall nrm2 (Arg)
        std::vector<Value *>marg;
        if (cublas) marg.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) marg.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) marg.push_back(item);
        if (byRef) {
        }
           if (cublasv2) marg.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {cache_x ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : marg) tys.push_back(arg->getType());
    llvm::FunctionType *FTnrm2 = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_nrm2 = blas.prefix + blas.floatType + "nrm2" + blas.suffix;
    auto derivcall_nrm2 = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_nrm2), FTnrm2);
    if (auto F = dyn_cast<Function>(derivcall_nrm2.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_nrm2);
      auto newF = attribute_nrm2(blas, F);
      derivcall_nrm2 = FunctionCallee(derivcall_nrm2.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_nrm2, marg, Defs));
         SmallVector<Value*, 1> resvec(1, cublasv2 ?  (Value*)Builder2.CreateLoad(fpType, marg[marg.size()-1]) : (Value*)cubcall);
         resvec[0] = to_blas_fp_callconv(Builder2, resvec[0], byRefFloat, blasFPType, allocationBuilder, "blascall");
         resvec;
 })
 ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, fpType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, fpType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateFDiv(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "FDiv" ));
 }
 vals; })) args1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
    if (cublasv2) {
      auto mod = gutils->oldFunc->getParent();
      auto DL = mod->getDataLayout();
      Value* inps[] = { gutils->lookupM(dif, Builder2), Constant::getNullValue(Type::getInt32Ty(dif->getContext())), ConstantInt::get(Type::getInt64Ty(dif->getContext()), DL.getTypeSizeInBits(fpType) / 8) };
      Type *tys[] = { inps[0]->getType(), inps[1]->getType(), inps[2]->getType() };
      Builder2.CreateCall(mod->getOrInsertFunction("cudaMemset", FunctionType::get(Type::getVoidTy(dif->getContext()), tys, false)), inps);
   }
    },
    d_x, dif);
  if (!cublasv2)
    setDiffe(
      &call,
      Constant::getNullValue(gutils->getShadowType(call.getType())),
      Builder2);
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_potrf(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_n = 1 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_A = 2 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 3 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_info = 4 + offset;
  const auto orig_info = call.getArgOperand(pos_info);
  auto arg_info = gutils->getNewFromOriginal(orig_info);
  const auto type_info = arg_info->getType();
  const bool overwritten_info = (cacheMode ? overwritten_args[pos_info] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    Value *rt_inactive_out = nullptr;
    if (active_A) {
      rt_inactive_out = rt_inactive_A;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "potrf" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_A;
  bool need_n = active_A;
  bool need_A = active_A;
  bool need_lda = active_A;
  bool need_info = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_info = cacheMode && byRef && overwritten_info && need_info;
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
BuilderZ.SetInsertPoint(gutils->getNewFromOriginal(&call)->getNextNode());
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall, &SubZero);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = arg_uplo;
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
Builder2.SetInsertPoint(gutils->getNewFromOriginal(&call)->getNextNode());
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_A  ) {
      Value *dres = nullptr;
        {
      // Seq
    Value *len = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tri = Builder2.CreateMul(len, len);
    CallInst * malloccall_mat_tri = nullptr;
    Value * true_mat_tri = CreateAllocation(Builder2, fpType, size_tri, "mat_tri", &malloccall_mat_tri);
    MDNode *ident_mat_tri = nullptr;
    if (malloccall_mat_tri) {
      ident_mat_tri = MDNode::getDistinct(malloccall_mat_tri->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tri->getContext()))});
      malloccall_mat_tri->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tri->getContext(), {ident_mat_tri}));
    }
    Value * mat_tri = true_mat_tri;
    if (type_vec_like->isIntegerTy()) {
      mat_tri = Builder2.CreatePtrToInt(mat_tri, type_vec_like);
    } else if (mat_tri->getType() != type_vec_like){
      mat_tri = Builder2.CreatePointerCast(mat_tri, type_vec_like);
    }
      if (d_A) {
        {
      // BlasCall lacpy
        std::vector<Value *>_0;
        if (cblas) _0.push_back(arg_layout);
        if (cublas) _0.push_back(arg_handle);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'u');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'U'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'l'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "flip_uplo.uplo") }; vs; })) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_A, arg_lda}) _0.push_back(item);
        for (auto item : {mat_tri}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           _0.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > _0.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, _0, Defs));
        }
        }
        {
        // LowerToUpper
        Value *arg_0[] = {arg_layout};
        Value *arg_1[] = {arg_uplo};
        SmallVector<Value*, 2> arg_2;
  for (auto v : {d_A, arg_lda}) arg_2.push_back(v);
        Value *arg_3[] = {arg_n};
 copy_lower_to_upper(Builder2, fpType, blas, byRef, 
                     arg_0[0] ? load_if_ref(Builder2, charType, arg_0[0], byRef) : nullptr,
                     is_lower(Builder2, arg_1[0], byRef, cublas),
                     arg_2[0],
                     load_if_ref(Builder2, intType, arg_2[1], byRef),
                     load_if_ref(Builder2, intType, arg_3[0], byRef)
                     );
        }
      if (d_A) {
        {
      // BlasCall trsm
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {to_blas_callconv(Builder2, valueL, byRef, cublas, nullptr, allocationBuilder, "constant.char.L")}) _2.push_back(item);
        for (auto item : {arg_uplo}) _2.push_back(item);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_normal.uplo") }; vs; })) _2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _2.push_back(item);
        for (auto item : {arg_A}) _2.push_back(item);
        for (auto item : {arg_lda}) _2.push_back(item);
        for (auto item : {d_A, arg_lda}) _2.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, _2, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall trsm
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : {to_blas_callconv(Builder2, valueR, byRef, cublas, nullptr, allocationBuilder, "constant.char.R")}) _3.push_back(item);
        for (auto item : {arg_uplo}) _3.push_back(item);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 't');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'n'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_trans.uplo") }; vs; })) _3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {arg_lda}) _3.push_back(item);
        for (auto item : {d_A, arg_lda}) _3.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, _3, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall scal
        std::vector<Value *>_4;
        if (cublas) _4.push_back(arg_handle);
        for (auto item : {arg_n}) _4.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.5), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.5")}) _4.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) _4.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) _4.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _4) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, _4, Defs));
        }
        }
        {
      // Seq
    Value *len1 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tmp = len1;
    CallInst * malloccall_mat_tmp = nullptr;
    Value * true_mat_tmp = CreateAllocation(Builder2, fpType, size_tmp, "mat_tmp", &malloccall_mat_tmp);
    MDNode *ident_mat_tmp = nullptr;
    if (malloccall_mat_tmp) {
      ident_mat_tmp = MDNode::getDistinct(malloccall_mat_tmp->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tmp->getContext()))});
      malloccall_mat_tmp->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tmp->getContext(), {ident_mat_tmp}));
    }
    Value * mat_tmp = true_mat_tmp;
    if (type_vec_like->isIntegerTy()) {
      mat_tmp = Builder2.CreatePtrToInt(mat_tmp, type_vec_like);
    } else if (mat_tmp->getType() != type_vec_like){
      mat_tmp = Builder2.CreatePointerCast(mat_tmp, type_vec_like);
    }
      if (d_A) {
        {
      // BlasCall copy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) _0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) _0.push_back(item);
        for (auto item : {mat_tmp}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, _0, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall lascl
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'u');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'U'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'l'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "flip_uplo.uplo") }; vs; })) _1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {d_A, arg_lda}) _1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) _1.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_lascl->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, _1, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall copy
        std::vector<Value *>_2;
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {mat_tmp}) _2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) _2.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, _2, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall trmm
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'L');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'R'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_side.uplo") }; vs; })) _3.push_back(item);
        for (auto item : {arg_uplo}) _3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) _3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {arg_lda}) _3.push_back(item);
        for (auto item : {d_A, arg_lda}) _3.push_back(item);
        if (byRef) {
    auto tmpF_trmm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trmm" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trmm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trmm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trmm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_trmm->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrmm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trmm = blas.prefix + blas.floatType + "trmm" + blas.suffix;
    auto derivcall_trmm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trmm), FTtrmm);
    if (auto F = dyn_cast<Function>(derivcall_trmm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trmm);
      auto newF = attribute_trmm(blas, F);
      derivcall_trmm = FunctionCallee(derivcall_trmm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trmm, _3, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall copy
        std::vector<Value *>_4;
        if (cublas) _4.push_back(arg_handle);
        for (auto item : {arg_n}) _4.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) _4.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) _4.push_back(item);
        for (auto item : {mat_tmp}) _4.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) _4.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _4) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, _4, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall lacpy
        std::vector<Value *>_5;
        if (cblas) _5.push_back(arg_layout);
        if (cublas) _5.push_back(arg_handle);
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'u');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'U'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'l'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "flip_uplo.uplo") }; vs; })) _5.push_back(item);
        for (auto item : {arg_n}) _5.push_back(item);
        for (auto item : {arg_n}) _5.push_back(item);
        for (auto item : {mat_tri}) _5.push_back(item);
        for (auto item : {arg_n}) _5.push_back(item);
        for (auto item : {d_A, arg_lda}) _5.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           _5.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > _5.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(_5.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _5) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, _5, Defs));
        }
        }
      if (d_A) {
        {
      // BlasCall copy
        std::vector<Value *>_6;
        if (cublas) _6.push_back(arg_handle);
        for (auto item : {arg_n}) _6.push_back(item);
        for (auto item : {mat_tmp}) _6.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) _6.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) _6.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) _6.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _6) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, _6, Defs));
        }
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tmp);
      if (freecall && ident_mat_tmp) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tmp}));
      }
    }
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tri);
      if (freecall && ident_mat_tri) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tri}));
      }
    }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_A);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "potrf" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_info && need_info)
      arg_info = lookup(arg_info, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_A) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_A && d_A) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_ztri = Builder2.CreateMul(len, len);
    CallInst * malloccall_mat_ztri = nullptr;
    Value * true_mat_ztri = CreateAllocation(Builder2, fpType, size_ztri, "mat_ztri", &malloccall_mat_ztri);
    MDNode *ident_mat_ztri = nullptr;
    if (malloccall_mat_ztri) {
      ident_mat_ztri = MDNode::getDistinct(malloccall_mat_ztri->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_ztri->getContext()))});
      malloccall_mat_ztri->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_ztri->getContext(), {ident_mat_ztri}));
    }
    {
    auto &M = *Builder2.GetInsertBlock()->getParent()->getParent();
    auto AlignI = M.getDataLayout().getTypeAllocSizeInBits(fpType) / 8;
    auto Align = ConstantInt::get(intType, AlignI);
    auto PT = cast<PointerType>(malloccall_mat_ztri->getType());
    Value *tozero = malloccall_mat_ztri;

    bool needsCast = false;
#if LLVM_VERSION_MAJOR < 17
#if LLVM_VERSION_MAJOR >= 15
    if (PT->getContext().supportsTypedPointers()) {
#endif
      needsCast = !PT->getPointerElementType()->isIntegerTy(8);
#if LLVM_VERSION_MAJOR >= 15
    }
#endif
#endif
    if (needsCast)
      tozero = Builder2.CreatePointerCast(
          tozero, PointerType::get(Type::getInt8Ty(PT->getContext()),
                                   PT->getAddressSpace()));
    Value *args[] = {
        tozero, ConstantInt::get(Type::getInt8Ty(malloccall_mat_ztri->getContext()), 0),
        Builder2.CreateMul(Align, size_ztri, "", true, true),
        ConstantInt::getFalse(malloccall_mat_ztri->getContext())};
    Type *tys[] = {args[0]->getType(), args[2]->getType()};

    Builder2.CreateCall(
        getIntrinsicDeclaration(&M, Intrinsic::memset, tys), args);
    }
    Value * mat_ztri = true_mat_ztri;
    if (type_vec_like->isIntegerTy()) {
      mat_ztri = Builder2.CreatePtrToInt(mat_ztri, type_vec_like);
    } else if (mat_ztri->getType() != type_vec_like){
      mat_ztri = Builder2.CreatePointerCast(mat_ztri, type_vec_like);
    }
        {
      // BlasCall lacpy
        std::vector<Value *>A_0;
        if (cblas) A_0.push_back(arg_layout);
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {d_A, arg_lda}) A_0.push_back(item);
        for (auto item : {mat_ztri}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           A_0.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(A_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, A_0, Defs));
        }
        {
      // BlasCall trmm
        std::vector<Value *>A_1;
        if (cblas) A_1.push_back(arg_layout);
        if (cublas) A_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'L');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'R'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_side.uplo") }; vs; })) A_1.push_back(item);
        for (auto item : {arg_uplo}) A_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueT, byRef, cublas, nullptr, allocationBuilder, "constant.char.T")}) A_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_1.push_back(item);
        for (auto item : {arg_A}) A_1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_1.push_back(item);
        for (auto item : {mat_ztri}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        if (byRef) {
    auto tmpF_trmm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trmm" + blas.suffix));
           A_1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
           A_1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
           A_1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
           A_1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrmm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trmm = blas.prefix + blas.floatType + "trmm" + blas.suffix;
    auto derivcall_trmm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trmm), FTtrmm);
    if (auto F = dyn_cast<Function>(derivcall_trmm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trmm);
      auto newF = attribute_trmm(blas, F);
      derivcall_trmm = FunctionCallee(derivcall_trmm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trmm, A_1, Defs));
        }
        {
      // Seq
    Value *len1 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tmp = len1;
    CallInst * malloccall_mat_tmp = nullptr;
    Value * true_mat_tmp = CreateAllocation(Builder2, fpType, size_tmp, "mat_tmp", &malloccall_mat_tmp);
    MDNode *ident_mat_tmp = nullptr;
    if (malloccall_mat_tmp) {
      ident_mat_tmp = MDNode::getDistinct(malloccall_mat_tmp->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tmp->getContext()))});
      malloccall_mat_tmp->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tmp->getContext(), {ident_mat_tmp}));
    }
    Value * mat_tmp = true_mat_tmp;
    if (type_vec_like->isIntegerTy()) {
      mat_tmp = Builder2.CreatePtrToInt(mat_tmp, type_vec_like);
    } else if (mat_tmp->getType() != type_vec_like){
      mat_tmp = Builder2.CreatePointerCast(mat_tmp, type_vec_like);
    }
        {
      // BlasCall copy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {mat_ztri}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {mat_tmp}) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_0, Defs));
        }
        {
      // BlasCall scal
        std::vector<Value *>A_1;
        if (cublas) A_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.5), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.5")}) A_1.push_back(item);
        for (auto item : {mat_tmp}) A_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, A_1, Defs));
        }
        {
      // BlasCall lascl
        std::vector<Value *>A_2;
        if (cblas) A_2.push_back(arg_layout);
        if (cublas) A_2.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'u');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'U'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'l'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "flip_uplo.uplo") }; vs; })) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) A_2.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_2.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {mat_ztri}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) A_2.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           A_2.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > A_2.size() ) ? tmpF_lascl->getFunctionType()->getParamType(A_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_2) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, A_2, Defs));
        }
        {
      // BlasCall copy
        std::vector<Value *>A_3;
        if (cublas) A_3.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {mat_tmp}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_3.push_back(item);
        for (auto item : {mat_ztri}) A_3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_3.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_3) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_3, Defs));
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tmp);
      if (freecall && ident_mat_tmp) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tmp}));
      }
    }
        }
        {
      // BlasCall trsm
        std::vector<Value *>A_3;
        if (cblas) A_3.push_back(arg_layout);
        if (cublas) A_3.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'R');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'L'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_rside.uplo") }; vs; })) A_3.push_back(item);
        for (auto item : {arg_uplo}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_3.push_back(item);
        for (auto item : {arg_A}) A_3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_3.push_back(item);
        for (auto item : {mat_ztri}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_3) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, A_3, Defs));
        }
        {
      // BlasCall trsm
        std::vector<Value *>A_4;
        if (cblas) A_4.push_back(arg_layout);
        if (cublas) A_4.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'L');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'R'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_side.uplo") }; vs; })) A_4.push_back(item);
        for (auto item : {arg_uplo}) A_4.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueT, byRef, cublas, nullptr, allocationBuilder, "constant.char.T")}) A_4.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_4.push_back(item);
        for (auto item : {arg_A}) A_4.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_4.push_back(item);
        for (auto item : {mat_ztri}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_4) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, A_4, Defs));
        }
        {
      // For
      auto lim_ar = ({SmallVector<Value*, 1> marg_0;
 for (auto tmp :     ({ auto V = load_if_ref(Builder2, intType, arg_n, byRef);
    SmallVector<Value*, 1> vs = {to_blas_callconv(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 0)), byRef, cublas, julia_decl_type, allocationBuilder, "is_zero")};
    vs; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; });
      Value *lim = (*lim_ar.begin());
      lim = load_if_ref(Builder2, intType, lim, byRef);
      BasicBlock *current = Builder2.GetInsertBlock();
      auto loopBlock = gutils->addReverseBlock(current,current->getName() + "_loop");
      auto endBlock = gutils->addReverseBlock(loopBlock,current->getName() + "_end", /*fork*/true, /*push*/false);
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, ConstantInt::get(lim->getType(), 0)), endBlock, loopBlock);
      Builder2.SetInsertPoint(loopBlock);
      auto phi_i = Builder2.CreatePHI(lim->getType(), 2);
      phi_i->addIncoming(ConstantInt::get(lim->getType(), 0), current);
      auto phi_i_inc = Builder2.CreateAdd(phi_i, ConstantInt::get(lim->getType(), 1), "", true, true);
      auto phi_b_i = to_blas_callconv(Builder2, phi_i, byRef, cublas, julia_decl_type, allocationBuilder, "for.i");
      Value *for_res = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {phi_b_i} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {mat_ztri}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {phi_b_i} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {phi_b_i} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
 ptr = to_blas_callconv(Builder2, ptr, byRef, cublas, nullptr, allocationBuilder, "offset");
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {mat_ztri}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {phi_b_i} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {phi_b_i} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
 ptr = to_blas_callconv(Builder2, ptr, byRef, cublas, nullptr, allocationBuilder, "offset");
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_0, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      phi_i->addIncoming(phi_i_inc, Builder2.GetInsertBlock());
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, phi_i_inc), endBlock, loopBlock);
      Builder2.SetInsertPoint(endBlock);
      {
        auto found = gutils->reverseBlockToPrimal.find(endBlock);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(endBlock);
      }
        }
        {
      // BlasCall lacpy
        std::vector<Value *>A_6;
        if (cblas) A_6.push_back(arg_layout);
        if (cublas) A_6.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_6.push_back(item);
        for (auto item : {arg_n}) A_6.push_back(item);
        for (auto item : {arg_n}) A_6.push_back(item);
        for (auto item : {mat_ztri}) A_6.push_back(item);
        for (auto item : {arg_n}) A_6.push_back(item);
        for (auto item : {d_A, arg_lda}) A_6.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           A_6.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > A_6.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(A_6.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_6) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, A_6, Defs));
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_ztri);
      if (freecall && ident_mat_ztri) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_ztri}));
      }
    }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
    },
    d_A  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_potrs(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_n = 1 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_nrhs = 2 + offset;
  const auto orig_nrhs = call.getArgOperand(pos_nrhs);
  auto arg_nrhs = gutils->getNewFromOriginal(orig_nrhs);
  const auto type_nrhs = arg_nrhs->getType();
  const bool overwritten_nrhs = (cacheMode ? overwritten_args[pos_nrhs] : false);

  const int pos_A = 3 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 4 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 5 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 6 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);

  const int pos_info = 7 + offset;
  const auto orig_info = call.getArgOperand(pos_info);
  auto arg_info = gutils->getNewFromOriginal(orig_info);
  const auto type_info = arg_info->getType();
  const bool overwritten_info = (cacheMode ? overwritten_args[pos_info] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    Value *rt_inactive_out = nullptr;
    if (active_B) {
      rt_inactive_out = rt_inactive_B;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "potrs" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_B || active_A;
  bool need_n = active_B || active_A;
  bool need_nrhs = active_B || active_A;
  bool need_A = active_B || active_A;
  bool need_lda = active_B || active_A;
  bool need_B = false;
  bool need_ldb = active_B || active_A;
  bool need_info = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_nrhs = cacheMode && byRef && overwritten_nrhs && need_nrhs;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  bool cache_info = cacheMode && byRef && overwritten_info && need_info;
  // we cache the following matrix,
  // since one rule uses input<B>
  if (active_A) {
    need_B = true;
    cache_B = true;
  }
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_nrhs)
    cacheTypes.push_back(intType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_nrhs, cache_nrhs, intType, cacheValues, BuilderZ, "nrhs");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = arg_uplo;
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_nrhs;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  Value *input_B = nullptr;
  Value *free_input_B = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_nrhs) {
        arg_nrhs = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.nrhs");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.nrhs");
        Builder2.CreateStore(arg_nrhs, alloc);
        arg_nrhs = Builder2.CreatePointerCast(
            alloc, type_nrhs, "cast.nrhs");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (active_A) {
      input_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_input_B = input_B;
      if (type_B->isIntegerTy()) {
        input_B = Builder2.CreatePtrToInt(input_B, type_B);
      } else if (input_B->getType() != type_B){
        input_B = Builder2.CreatePointerCast(input_B, type_B);
      }
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_A, Value *d_B  ) {
      Value *dres = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument  within potrs of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_A, d_B);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "potrs" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_nrhs && need_nrhs)
      arg_nrhs = lookup(arg_nrhs, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
    if (!cache_info && need_info)
      arg_info = lookup(arg_info, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_A && d_B && d_A) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tri = Builder2.CreateMul(len, len);
    CallInst * malloccall_mat_tri = nullptr;
    Value * true_mat_tri = CreateAllocation(Builder2, fpType, size_tri, "mat_tri", &malloccall_mat_tri);
    MDNode *ident_mat_tri = nullptr;
    if (malloccall_mat_tri) {
      ident_mat_tri = MDNode::getDistinct(malloccall_mat_tri->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tri->getContext()))});
      malloccall_mat_tri->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tri->getContext(), {ident_mat_tri}));
    }
    Value * mat_tri = true_mat_tri;
    if (type_vec_like->isIntegerTy()) {
      mat_tri = Builder2.CreatePtrToInt(mat_tri, type_vec_like);
    } else if (mat_tri->getType() != type_vec_like){
      mat_tri = Builder2.CreatePointerCast(mat_tri, type_vec_like);
    }
        {
      // BlasCall syr2k
        std::vector<Value *>A_0;
        if (cblas) A_0.push_back(arg_layout);
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {to_blas_callconv(Builder2, valueU, byRef, cublas, nullptr, allocationBuilder, "constant.char.U")}) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {arg_nrhs}) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_0.push_back(item);
        for (auto item : {input_B}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {d_B, arg_ldb}) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) A_0.push_back(item);
        for (auto item : {mat_tri}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        if (byRef) {
    auto tmpF_syr2k = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "syr2k" + blas.suffix));
           A_0.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(A_0.size()) : intType, 1));
           A_0.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(A_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTsyr2k = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_syr2k = blas.prefix + blas.floatType + "syr2k" + blas.suffix;
    auto derivcall_syr2k = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_syr2k), FTsyr2k);
    if (auto F = dyn_cast<Function>(derivcall_syr2k.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_syr2k);
      auto newF = attribute_syr2k(blas, F);
      derivcall_syr2k = FunctionCallee(derivcall_syr2k.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_syr2k, A_0, Defs));
        }
        {
        // LowerToUpper
        Value *arg_0[] = {arg_layout};
        Value *arg_1[] = {to_blas_callconv(Builder2, valueU, byRef, cublas, nullptr, allocationBuilder, "constant.char.U")};
        SmallVector<Value*, 2> arg_2;
  for (auto v : ({auto concat_0 = {mat_tri}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); })) arg_2.push_back(v);
        Value *arg_3[] = {arg_n};
 copy_lower_to_upper(Builder2, fpType, blas, byRef, 
                     arg_0[0] ? load_if_ref(Builder2, charType, arg_0[0], byRef) : nullptr,
                     is_lower(Builder2, arg_1[0], byRef, cublas),
                     arg_2[0],
                     load_if_ref(Builder2, intType, arg_2[1], byRef),
                     load_if_ref(Builder2, intType, arg_3[0], byRef)
                     );
        }
        {
      // BlasCall trsm
        std::vector<Value *>A_2;
        if (cblas) A_2.push_back(arg_layout);
        if (cublas) A_2.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'R');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'L'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_rside.uplo") }; vs; })) A_2.push_back(item);
        for (auto item : {arg_uplo}) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueT, byRef, cublas, nullptr, allocationBuilder, "constant.char.T")}) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_2.push_back(item);
        for (auto item : {arg_A}) A_2.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_2.push_back(item);
        for (auto item : {mat_tri}) A_2.push_back(item);
        for (auto item : {arg_n}) A_2.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           A_2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_2.size()) : intType, 1));
           A_2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_2.size()) : intType, 1));
           A_2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_2.size()) : intType, 1));
           A_2.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_2.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_2) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, A_2, Defs));
        }
        {
      // BlasCall trsm
        std::vector<Value *>A_3;
        if (cblas) A_3.push_back(arg_layout);
        if (cublas) A_3.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'L');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'R'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_side.uplo") }; vs; })) A_3.push_back(item);
        for (auto item : {arg_uplo}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_3.push_back(item);
        for (auto item : {arg_A}) A_3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_3.push_back(item);
        for (auto item : {mat_tri}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
           A_3.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_3) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, A_3, Defs));
        }
        {
      // BlasCall trsm
        std::vector<Value *>A_4;
        if (cblas) A_4.push_back(arg_layout);
        if (cublas) A_4.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_uplo;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.uplo");
    Value *res = ConstantInt::get(charType, 'L');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'L'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 'R'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'u')), ConstantInt::get(res->getType(), 'R'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "uplo_to_side.uplo") }; vs; })) A_4.push_back(item);
        for (auto item : {arg_uplo}) A_4.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueT, byRef, cublas, nullptr, allocationBuilder, "constant.char.T")}) A_4.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_4.push_back(item);
        for (auto item : {arg_A}) A_4.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) A_4.push_back(item);
        for (auto item : {mat_tri}) A_4.push_back(item);
        for (auto item : {arg_n}) A_4.push_back(item);
        if (byRef) {
    auto tmpF_trsm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trsm" + blas.suffix));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
           A_4.push_back(ConstantInt::get((tmpF_trsm && tmpF_trsm->getFunctionType()->getNumParams() > A_4.size() ) ? tmpF_trsm->getFunctionType()->getParamType(A_4.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_4) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrsm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trsm = blas.prefix + blas.floatType + "trsm" + blas.suffix;
    auto derivcall_trsm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trsm), FTtrsm);
    if (auto F = dyn_cast<Function>(derivcall_trsm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trsm);
      auto newF = attribute_trsm(blas, F);
      derivcall_trsm = FunctionCallee(derivcall_trsm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trsm, A_4, Defs));
        }
        {
      // For
      auto lim_ar = {arg_n};
      Value *lim = (*lim_ar.begin());
      lim = load_if_ref(Builder2, intType, lim, byRef);
      BasicBlock *current = Builder2.GetInsertBlock();
      auto loopBlock = gutils->addReverseBlock(current,current->getName() + "_loop");
      auto endBlock = gutils->addReverseBlock(loopBlock,current->getName() + "_end", /*fork*/true, /*push*/false);
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, ConstantInt::get(lim->getType(), 0)), endBlock, loopBlock);
      Builder2.SetInsertPoint(loopBlock);
      auto phi_i = Builder2.CreatePHI(lim->getType(), 2);
      phi_i->addIncoming(ConstantInt::get(lim->getType(), 0), current);
      auto phi_i_inc = Builder2.CreateAdd(phi_i, ConstantInt::get(lim->getType(), 1), "", true, true);
      auto phi_b_i = to_blas_callconv(Builder2, phi_i, byRef, cublas, julia_decl_type, allocationBuilder, "for.i");
      Value *for_res = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {phi_b_i} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, -1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.-1.0")}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {mat_tri}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : {phi_b_i} ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : {phi_b_i} ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
 ptr = to_blas_callconv(Builder2, ptr, byRef, cublas, nullptr, allocationBuilder, "offset");
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : {d_A, arg_lda} ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : {phi_b_i} ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : {phi_b_i} ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto concat_1 = {arg_lda}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
 ptr = to_blas_callconv(Builder2, ptr, byRef, cublas, nullptr, allocationBuilder, "offset");
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_0, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      phi_i->addIncoming(phi_i_inc, Builder2.GetInsertBlock());
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, phi_i_inc), endBlock, loopBlock);
      Builder2.SetInsertPoint(endBlock);
      {
        auto found = gutils->reverseBlockToPrimal.find(endBlock);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(endBlock);
      }
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tri);
      if (freecall && ident_mat_tri) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tri}));
      }
    }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_B && d_B) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall potrs
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_nrhs}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_potrs = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "potrs" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_potrs && tmpF_potrs->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_potrs->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTpotrs = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_potrs = blas.prefix + blas.floatType + "potrs" + blas.suffix;
    auto derivcall_potrs = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_potrs), FTpotrs);
    if (auto F = dyn_cast<Function>(derivcall_potrs.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_potrs);
      auto newF = attribute_potrs(blas, F);
      derivcall_potrs = FunctionCallee(derivcall_potrs.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_potrs, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
    },
    d_A, d_B  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_scal(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = (cublas ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

  const int pos_n = 0 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 1 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_x = 2 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 3 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    Value *rt_inactive_out = nullptr;
    if (active_x) {
      rt_inactive_out = rt_inactive_x;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "scal" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = byRef ? (Type*) getInt8PtrTy(call.getContext()) : (Type*) Type::getInt8Ty(call.getContext());
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_x->isPointerTy();
  Type* type_vec_like = type_x;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x || active_alpha;
  bool need_alpha = active_x;
  bool need_x = active_alpha;
  bool need_incx = active_x || active_alpha;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[0] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_x  ) {
      Value *dres = nullptr;
        {
      // Seq
      if (d_x) {
        {
      // BlasCall scal
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_alpha}) _0.push_back(item);
        for (auto item : {d_x, arg_incx}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, _0, Defs));
        }
        }
      if (d_x && d_alpha) {
        {
      // BlasCall axpy
        std::vector<Value *>_1;
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {d_alpha}) _1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _1.push_back(item);
        for (auto item : {d_x, arg_incx}) _1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _1, Defs));
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_x);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "scal" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_x) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_x) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall dot
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, args1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, args1[args1.size()-1]);
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_x && d_x) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall scal
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
    },
    d_x  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_spmv(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_n = 1 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 2 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_ap = 3 + offset;
  const auto orig_ap = call.getArgOperand(pos_ap);
  auto arg_ap = gutils->getNewFromOriginal(orig_ap);
  const auto type_ap = arg_ap->getType();
  const bool overwritten_ap = (cacheMode ? overwritten_args[pos_ap] : false);
  bool active_ap = !gutils->isConstantValue(orig_ap);
  Value *rt_inactive_ap = nullptr;

  const int pos_x = 4 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 5 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_beta = 6 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_y = 7 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 8 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_ap) {
      auto shadow_ap = gutils->invertPointerM(orig_ap, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_ap = BuilderZ.CreateICmpEQ(shadow_ap, arg_ap, "rt.tmp.inactive." "ap");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_ap_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_ap, i), arg_ap, "rt.tmp.inactive." "ap." + std::to_string(i));
          if (i == 0) rt_inactive_ap = rt_inactive_ap_tmp;
          else rt_inactive_ap = BuilderZ.CreateOr(rt_inactive_ap, rt_inactive_ap_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_ap) : rt_inactive_ap;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    Value *rt_inactive_out = nullptr;
    if (active_y) {
      rt_inactive_out = rt_inactive_y;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_ap) {
      rt_inactive_ap = BuilderZ.CreateOr(rt_inactive_ap, rt_inactive_out, "rt.inactive." "ap");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_ap) : rt_inactive_ap;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_out, "rt.inactive." "y");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "spmv" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_ap->isPointerTy();
  Type* type_vec_like = type_ap;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_ap || active_x || active_alpha;
  bool need_n = active_ap || active_y || active_x || active_alpha || active_beta;
  bool need_alpha = active_ap || active_x;
  bool need_ap = active_x || active_alpha;
  bool need_x = active_ap || active_alpha;
  bool need_incx = active_ap || active_x || active_alpha;
  bool need_beta = active_y;
  bool need_y = false;
  bool need_incy = active_ap || active_y || active_x || active_alpha || active_beta;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_ap = cacheMode && overwritten_ap && need_ap;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  // we cache the following matrix,
  // since one rule uses input<y>
  if (active_beta) {
    need_y = true;
    cache_y = true;
  }
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_ap)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_ap) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.ap", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      Value *margs[] = {malins, arg_ap, malloc_size, llvm::ConstantInt::getFalse(IntegerType::getInt1Ty(call.getContext()))};
      Type *tys[] = {margs[0]->getType(), margs[1]->getType(),                     margs[2]->getType()};
      auto memcpyF = getIntrinsicDeclaration(gutils->oldFunc->getParent(), Intrinsic::memcpy, tys);
      BuilderZ.CreateCall(memcpyF, margs);
      cacheValues.push_back(malins);
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *free_ap = nullptr;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  Value *input_y = nullptr;
  Value *free_input_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_ap) {
      arg_ap = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ap");
      free_ap = arg_ap;
      if (type_ap->isIntegerTy()) {
        arg_ap = Builder2.CreatePtrToInt(arg_ap, type_ap);
      } else if (arg_ap->getType() != type_ap){
        arg_ap = Builder2.CreatePointerCast(arg_ap, type_ap);
      }
      cacheidx++;
    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (active_beta) {
      input_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_input_y = input_y;
      if (type_y->isIntegerTy()) {
        input_y = Builder2.CreatePtrToInt(input_y, type_y);
      } else if (input_y->getType() != type_y){
        input_y = Builder2.CreatePointerCast(input_y, type_y);
      }
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_ap = active_ap
     ? gutils->invertPointerM(orig_ap, Builder2)
     : nullptr;
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_ap, Value *d_x, Value *d_beta, Value *d_y  ) {
      Value *dres = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument  within spmv of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_ap, d_x, d_beta, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "spmv" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_ap = active_ap
     ? lookup(gutils->invertPointerM(orig_ap, Builder2), Builder2)
     : nullptr;
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_ap && need_ap)
      arg_ap = lookup(arg_ap, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_ap) {
      rt_inactive_ap = lookup(rt_inactive_ap, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_ap, Value *d_x, Value *d_y) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_y) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_y0 = Builder2.CreateMul(len, len);
    CallInst * malloccall_mat_y0 = nullptr;
    Value * true_mat_y0 = CreateAllocation(Builder2, fpType, size_y0, "mat_y0", &malloccall_mat_y0);
    MDNode *ident_mat_y0 = nullptr;
    if (malloccall_mat_y0) {
      ident_mat_y0 = MDNode::getDistinct(malloccall_mat_y0->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_y0->getContext()))});
      malloccall_mat_y0->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_y0->getContext(), {ident_mat_y0}));
    }
    Value * mat_y0 = true_mat_y0;
    if (type_vec_like->isIntegerTy()) {
      mat_y0 = Builder2.CreatePtrToInt(mat_y0, type_vec_like);
    } else if (mat_y0->getType() != type_vec_like){
      mat_y0 = Builder2.CreatePointerCast(mat_y0, type_vec_like);
    }
        {
      // BlasCall spmv
        std::vector<Value *>alpha_0;
        if (cblas) alpha_0.push_back(arg_layout);
        if (cublas) alpha_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) alpha_0.push_back(item);
        for (auto item : {arg_n}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) alpha_0.push_back(item);
        for (auto item : {arg_ap}) alpha_0.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) alpha_0.push_back(item);
        for (auto item : {mat_y0}) alpha_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_0.push_back(item);
        if (byRef) {
    auto tmpF_spmv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "spmv" + blas.suffix));
           alpha_0.push_back(ConstantInt::get((tmpF_spmv && tmpF_spmv->getFunctionType()->getNumParams() > alpha_0.size() ) ? tmpF_spmv->getFunctionType()->getParamType(alpha_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTspmv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_spmv = blas.prefix + blas.floatType + "spmv" + blas.suffix;
    auto derivcall_spmv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_spmv), FTspmv);
    if (auto F = dyn_cast<Function>(derivcall_spmv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_spmv);
      auto newF = attribute_spmv(blas, F);
      derivcall_spmv = FunctionCallee(derivcall_spmv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_spmv, alpha_0, Defs));
        }
        {
      // BlasCall dot
        std::vector<Value *>alpha_1;
        if (cublas) alpha_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) alpha_1.push_back(item);
        for (auto item : {d_y, arg_incy}) alpha_1.push_back(item);
        for (auto item : {mat_y0}) alpha_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) alpha_1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, alpha_1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, alpha_1[alpha_1.size()-1]);
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_y0);
      if (freecall && ident_mat_y0) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_y0}));
      }
    }
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_ap && d_ap && d_y) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_ap = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".ap.active");
          nextBlock_ap = gutils->addReverseBlock(activeBlock, bb_name + ".ap.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_ap, nextBlock_ap, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
        {
      // BlasCall spr2
        std::vector<Value *>ap_0;
        if (cblas) ap_0.push_back(arg_layout);
        if (cublas) ap_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) ap_0.push_back(item);
        for (auto item : {arg_n}) ap_0.push_back(item);
        for (auto item : {arg_alpha}) ap_0.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) ap_0.push_back(item);
        for (auto item : {d_y, arg_incy}) ap_0.push_back(item);
        for (auto item : {d_ap}) ap_0.push_back(item);
        if (byRef) {
    auto tmpF_spr2 = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "spr2" + blas.suffix));
           ap_0.push_back(ConstantInt::get((tmpF_spr2 && tmpF_spr2->getFunctionType()->getNumParams() > ap_0.size() ) ? tmpF_spr2->getFunctionType()->getParamType(ap_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : ap_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTspr2 = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_spr2 = blas.prefix + blas.floatType + "spr2" + blas.suffix;
    auto derivcall_spr2 = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_spr2), FTspr2);
    if (auto F = dyn_cast<Function>(derivcall_spr2.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_spr2);
      auto newF = attribute_spr2(blas, F);
      derivcall_spr2 = FunctionCallee(derivcall_spr2.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_spr2, ap_0, Defs));
        }
        {
      // DiagUpdateSPMV
        std::vector<Value *>ap_1;
        if (cublas) ap_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) ap_1.push_back(item);
        for (auto item : {arg_n}) ap_1.push_back(item);
        for (auto item : {arg_alpha}) ap_1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) ap_1.push_back(item);
        for (auto item : {d_y, arg_incy}) ap_1.push_back(item);
        for (auto item : {d_ap}) ap_1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
callSPMVDiagUpdate(Builder2, *gutils->oldFunc->getParent(), blas, intType, blasCharType, blasFPType, type_vec_like, type_n, fpType, ArrayRef<Value *>(ap_1), Defs, byRef, julia_decl);
        }
        if (nextBlock_ap && byRefFloat) {
          Builder2.CreateBr(nextBlock_ap);
          Builder2.SetInsertPoint(nextBlock_ap);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_ap);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_ap);
      }
        }
        }
      }
      if (active_x && d_x && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall spmv
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {arg_ap}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
    auto tmpF_spmv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "spmv" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_spmv && tmpF_spmv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_spmv->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTspmv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_spmv = blas.prefix + blas.floatType + "spmv" + blas.suffix;
    auto derivcall_spmv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_spmv), FTspmv);
    if (auto F = dyn_cast<Function>(derivcall_spmv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_spmv);
      auto newF = attribute_spmv(blas, F);
      derivcall_spmv = FunctionCallee(derivcall_spmv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_spmv, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
      if (active_beta && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_beta = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".beta.active");
          nextBlock_beta = gutils->addReverseBlock(activeBlock, bb_name + ".beta.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_beta, nextBlock_beta, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall dot
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {input_y, (cache_y ? const_one : arg_incy)}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, args1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, args1[args1.size()-1]);
        if (nextBlock_beta && byRefFloat) {
          Builder2.CreateBr(nextBlock_beta);
          Builder2.SetInsertPoint(nextBlock_beta);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_beta);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_beta);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_y && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_y = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".y.active");
          nextBlock_y = gutils->addReverseBlock(activeBlock, bb_name + ".y.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_y, nextBlock_y, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall scal
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, args1, Defs));
        if (nextBlock_y) {
          Builder2.CreateBr(nextBlock_y);
          Builder2.SetInsertPoint(nextBlock_y);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_y);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_y);
      }
        }
        }
      }
    },
    d_ap, d_x, d_y  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_ap) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_ap);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_symm(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_side = 0 + offset;
  const auto orig_side = call.getArgOperand(pos_side);
  auto arg_side = gutils->getNewFromOriginal(orig_side);
  const auto type_side = arg_side->getType();
  const bool overwritten_side = (cacheMode ? overwritten_args[pos_side] : false);

  const int pos_uplo = 1 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_m = 2 + offset;
  const auto orig_m = call.getArgOperand(pos_m);
  auto arg_m = gutils->getNewFromOriginal(orig_m);
  const auto type_m = arg_m->getType();
  const bool overwritten_m = (cacheMode ? overwritten_args[pos_m] : false);

  const int pos_n = 3 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 4 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 5 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 6 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 7 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 8 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);

  const int pos_beta = 9 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_C = 10 + offset;
  const auto orig_C = call.getArgOperand(pos_C);
  auto arg_C = gutils->getNewFromOriginal(orig_C);
  const auto type_C = arg_C->getType();
  const bool overwritten_C = (cacheMode ? overwritten_args[pos_C] : false);
  bool active_C = !gutils->isConstantValue(orig_C);
  Value *rt_inactive_C = nullptr;

  const int pos_ldc = 11 + offset;
  const auto orig_ldc = call.getArgOperand(pos_ldc);
  auto arg_ldc = gutils->getNewFromOriginal(orig_ldc);
  const auto type_ldc = arg_ldc->getType();
  const bool overwritten_ldc = (cacheMode ? overwritten_args[pos_ldc] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      auto shadow_C = gutils->invertPointerM(orig_C, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_C = BuilderZ.CreateICmpEQ(shadow_C, arg_C, "rt.tmp.inactive." "C");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_C_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_C, i), arg_C, "rt.tmp.inactive." "C." + std::to_string(i));
          if (i == 0) rt_inactive_C = rt_inactive_C_tmp;
          else rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_C_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    Value *rt_inactive_out = nullptr;
    if (active_C) {
      rt_inactive_out = rt_inactive_C;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_out, "rt.inactive." "C");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "symm" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_side;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_side;
  Type* blasIntType = type_m;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_side = active_A || active_B;
  bool need_uplo = active_A || active_B;
  bool need_m = active_A || active_C || active_B;
  bool need_n = active_A || active_C || active_B;
  bool need_alpha = active_A || active_B;
  bool need_A = active_B;
  bool need_lda = active_A || active_B;
  bool need_B = active_A;
  bool need_ldb = active_A || active_B;
  bool need_beta = active_C;
  bool need_C = false;
  bool need_ldc = active_A || active_C || active_B;
  bool cache_side = cacheMode && byRef && overwritten_side && need_side;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_C = cacheMode && overwritten_C && need_C;
  bool cache_ldc = cacheMode && byRef && overwritten_ldc && need_ldc;
  if (cache_side)
    cacheTypes.push_back(charType);
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_m)
    cacheTypes.push_back(intType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_ldc)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_C)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_m, cache_m, intType, cacheValues, BuilderZ, "m");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_ldc, cache_ldc, intType, cacheValues, BuilderZ, "ldc");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      Value *normal = is_left(BuilderZ, arg_side, byRef, cublas);
      M = N = BuilderZ.CreateSelect(normal, arg_m, arg_n);
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_C) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.C", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[11] = ValueType::Primal;
      if (byRef) valueTypes[12] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_C, arg_ldc, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldc, byRef);
        Value *args[5] = {malins, arg_C, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  Value *true_ldc = arg_ldc;
  Value *ldc = true_ldc;
  Value *free_C = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_m) {
        arg_m = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.m");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.m");
        Builder2.CreateStore(arg_m, alloc);
        arg_m = Builder2.CreatePointerCast(
            alloc, type_m, "cast.m");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldc) {
        arg_ldc = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldc");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldc");
        Builder2.CreateStore(arg_ldc, alloc);
        arg_ldc = Builder2.CreatePointerCast(
            alloc, type_ldc, "cast.ldc");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
    if (cache_C) {
      arg_C = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.C");
      free_C = arg_C;
      if (type_C->isIntegerTy()) {
        arg_C = Builder2.CreatePtrToInt(arg_C, type_C);
      } else if (arg_C->getType() != type_C){
        arg_C = Builder2.CreatePointerCast(arg_C, type_C);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_C = active_C
     ? gutils->invertPointerM(orig_C, Builder2)
     : nullptr;
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_B, Value *d_beta, Value *d_C  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_C && d_beta) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item :             ({std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument within symm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
            ArrayRef<Value*>(); })) _0.push_back(item);
        for (auto item : {d_beta}) _0.push_back(item);
        for (auto item : {arg_C}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_C && d_B) {
        {
      // BlasCall symm
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_side}) _1.push_back(item);
        for (auto item : {arg_uplo}) _1.push_back(item);
        for (auto item : {arg_m}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_alpha}) _1.push_back(item);
        for (auto item : {arg_A}) _1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; }), arg_lda, arg_n, arg_m, cache_A, byRef, cublas)}) _1.push_back(item);
        for (auto item : {d_B, arg_ldb}) _1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _1.push_back(item);
        for (auto item : {d_C, arg_ldc}) _1.push_back(item);
        if (byRef) {
    auto tmpF_symm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symm" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_symm->getFunctionType()->getParamType(_1.size()) : intType, 1));
           _1.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_symm->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symm = blas.prefix + blas.floatType + "symm" + blas.suffix;
    auto derivcall_symm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symm), FTsymm);
    if (auto F = dyn_cast<Function>(derivcall_symm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symm);
      auto newF = attribute_symm(blas, F);
      derivcall_symm = FunctionCallee(derivcall_symm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symm, _1, Defs));
        }
        }
      if (d_C && d_A) {
        {
      // BlasCall symm
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_side}) _2.push_back(item);
        for (auto item : {arg_uplo}) _2.push_back(item);
        for (auto item : {arg_m}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_alpha}) _2.push_back(item);
        for (auto item : {d_A, arg_lda}) _2.push_back(item);
        for (auto item : {arg_B}) _2.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_ldb, arg_m, arg_m, cache_B, byRef, cublas)}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _2.push_back(item);
        for (auto item : {d_C, arg_ldc}) _2.push_back(item);
        if (byRef) {
    auto tmpF_symm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symm" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_symm->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_symm->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symm = blas.prefix + blas.floatType + "symm" + blas.suffix;
    auto derivcall_symm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symm), FTsymm);
    if (auto F = dyn_cast<Function>(derivcall_symm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symm);
      auto newF = attribute_symm(blas, F);
      derivcall_symm = FunctionCallee(derivcall_symm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symm, _2, Defs));
        }
        }
      if (d_C && d_alpha) {
        {
      // BlasCall symm
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : {arg_side}) _3.push_back(item);
        for (auto item : {arg_uplo}) _3.push_back(item);
        for (auto item : {arg_m}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {d_alpha}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; }), arg_lda, arg_n, arg_m, cache_A, byRef, cublas)}) _3.push_back(item);
        for (auto item : {arg_B}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_ldb, arg_m, arg_m, cache_B, byRef, cublas)}) _3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _3.push_back(item);
        for (auto item : {d_C, arg_ldc}) _3.push_back(item);
        if (byRef) {
    auto tmpF_symm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symm" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_symm->getFunctionType()->getParamType(_3.size()) : intType, 1));
           _3.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_symm->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symm = blas.prefix + blas.floatType + "symm" + blas.suffix;
    auto derivcall_symm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symm), FTsymm);
    if (auto F = dyn_cast<Function>(derivcall_symm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symm);
      auto newF = attribute_symm(blas, F);
      derivcall_symm = FunctionCallee(derivcall_symm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symm, _3, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_C) {
        {
      // BlasCall lascl
        std::vector<Value *>_0;
        if (cblas) _0.push_back(arg_layout);
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {to_blas_callconv(Builder2, valueG, byRef, cublas, nullptr, allocationBuilder, "constant.char.G")}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _0.push_back(item);
        for (auto item : {arg_beta}) _0.push_back(item);
        for (auto item : {arg_m}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) _0.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           _0.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > _0.size() ) ? tmpF_lascl->getFunctionType()->getParamType(_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_B, d_beta, d_C);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "symm" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_C = active_C
     ? lookup(gutils->invertPointerM(orig_C, Builder2), Builder2)
     : nullptr;
    if (!cache_side && need_side)
      arg_side = lookup(arg_side, Builder2);
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_m && need_m)
      arg_m = lookup(arg_m, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_C && need_C)
      arg_C = lookup(arg_C, Builder2);
    if (!cache_ldc && need_ldc)
      arg_ldc = lookup(arg_ldc, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_C) {
      rt_inactive_C = lookup(rt_inactive_C, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B, Value *d_C) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha) {
        Value *toadd = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument alpha within symm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_C && d_A) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len1 = load_if_ref(Builder2, intType,arg_m, byRef);
    Value *len2 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tmp = Builder2.CreateSelect(is_left(Builder2, arg_side, byRef, cublas), len1, len2);
    CallInst * malloccall_mat_tmp = nullptr;
    Value * true_mat_tmp = CreateAllocation(Builder2, fpType, size_tmp, "mat_tmp", &malloccall_mat_tmp);
    MDNode *ident_mat_tmp = nullptr;
    if (malloccall_mat_tmp) {
      ident_mat_tmp = MDNode::getDistinct(malloccall_mat_tmp->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tmp->getContext()))});
      malloccall_mat_tmp->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tmp->getContext(), {ident_mat_tmp}));
    }
    Value * mat_tmp = true_mat_tmp;
    if (type_vec_like->isIntegerTy()) {
      mat_tmp = Builder2.CreatePtrToInt(mat_tmp, type_vec_like);
    } else if (mat_tmp->getType() != type_vec_like){
      mat_tmp = Builder2.CreatePointerCast(mat_tmp, type_vec_like);
    }
        {
      // BlasCall copy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {mat_tmp}) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_0, Defs));
        }
        {
      // BlasCall syr2k
        std::vector<Value *>A_1;
        if (cblas) A_1.push_back(arg_layout);
        if (cublas) A_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_1.push_back(item);
        for (auto item : ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; })) A_1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_n} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_m} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_1.push_back(item);
        for (auto item : {arg_alpha}) A_1.push_back(item);
        for (auto item : {arg_B}) A_1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_ldb, arg_m, arg_m, cache_B, byRef, cublas)}) A_1.push_back(item);
        for (auto item : {d_C, arg_ldc}) A_1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) A_1.push_back(item);
        for (auto item : {d_A, arg_lda}) A_1.push_back(item);
        if (byRef) {
    auto tmpF_syr2k = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "syr2k" + blas.suffix));
           A_1.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(A_1.size()) : intType, 1));
           A_1.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(A_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsyr2k = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_syr2k = blas.prefix + blas.floatType + "syr2k" + blas.suffix;
    auto derivcall_syr2k = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_syr2k), FTsyr2k);
    if (auto F = dyn_cast<Function>(derivcall_syr2k.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_syr2k);
      auto newF = attribute_syr2k(blas, F);
      derivcall_syr2k = FunctionCallee(derivcall_syr2k.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_syr2k, A_1, Defs));
        }
        {
      // BlasCall axpy
        std::vector<Value *>A_2;
        if (cublas) A_2.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_2.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, -1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.-1")}) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_2.push_back(item);
        for (auto item : {mat_tmp}) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_2.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_2) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_2, Defs));
        }
        {
      // BlasCall axpy
        std::vector<Value *>A_3;
        if (cublas) A_3.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_3.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.5), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.5")}) A_3.push_back(item);
        for (auto item : {mat_tmp}) A_3.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_3.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_3) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_3, Defs));
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tmp);
      if (freecall && ident_mat_tmp) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tmp}));
      }
    }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_B && d_C && d_B) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall symm
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_side}) args1.push_back(item);
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; }), arg_lda, arg_n, arg_m, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_C, arg_ldc}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        if (byRef) {
    auto tmpF_symm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symm" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_symm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_symm->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symm = blas.prefix + blas.floatType + "symm" + blas.suffix;
    auto derivcall_symm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symm), FTsymm);
    if (auto F = dyn_cast<Function>(derivcall_symm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symm);
      auto newF = attribute_symm(blas, F);
      derivcall_symm = FunctionCallee(derivcall_symm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symm, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
      if (active_beta) {
        Value *toadd = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument beta within symm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_C && d_C) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_C = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".C.active");
          nextBlock_C = gutils->addReverseBlock(activeBlock, bb_name + ".C.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_C, nextBlock_C, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall lascl
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {to_blas_callconv(Builder2, valueG, byRef, cublas, nullptr, allocationBuilder, "constant.char.G")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_C, arg_ldc}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_lascl->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, args1, Defs));
        if (nextBlock_C) {
          Builder2.CreateBr(nextBlock_C);
          Builder2.SetInsertPoint(nextBlock_C);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_C);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_C);
      }
        }
        }
      }
    },
    d_A, d_B, d_C  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_C) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_C);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_symv(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_n = 1 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 2 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 3 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 4 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_x = 5 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 6 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);

  const int pos_beta = 7 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_y = 8 + offset;
  const auto orig_y = call.getArgOperand(pos_y);
  auto arg_y = gutils->getNewFromOriginal(orig_y);
  const auto type_y = arg_y->getType();
  const bool overwritten_y = (cacheMode ? overwritten_args[pos_y] : false);
  bool active_y = !gutils->isConstantValue(orig_y);
  Value *rt_inactive_y = nullptr;

  const int pos_incy = 9 + offset;
  const auto orig_incy = call.getArgOperand(pos_incy);
  auto arg_incy = gutils->getNewFromOriginal(orig_incy);
  const auto type_incy = arg_incy->getType();
  const bool overwritten_incy = (cacheMode ? overwritten_args[pos_incy] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      auto shadow_y = gutils->invertPointerM(orig_y, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_y = BuilderZ.CreateICmpEQ(shadow_y, arg_y, "rt.tmp.inactive." "y");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_y_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_y, i), arg_y, "rt.tmp.inactive." "y." + std::to_string(i));
          if (i == 0) rt_inactive_y = rt_inactive_y_tmp;
          else rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_y_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    Value *rt_inactive_out = nullptr;
    if (active_y) {
      rt_inactive_out = rt_inactive_y;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_y) {
      rt_inactive_y = BuilderZ.CreateOr(rt_inactive_y, rt_inactive_out, "rt.inactive." "y");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_y) : rt_inactive_y;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "symv" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_x || active_A;
  bool need_n = active_x || active_A || active_beta || active_alpha || active_y;
  bool need_alpha = active_x || active_A;
  bool need_A = active_x || active_alpha;
  bool need_lda = active_x || active_A || active_alpha;
  bool need_x = active_A || active_alpha;
  bool need_incx = active_x || active_A || active_alpha;
  bool need_beta = active_y;
  bool need_y = false;
  bool need_incy = active_x || active_A || active_beta || active_alpha || active_y;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  // we cache the following matrix,
  // since one rule uses input<y>
  if (active_beta) {
    need_y = true;
    cache_y = true;
  }
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_incy)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_y)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_incy, cache_incy, intType, cacheValues, BuilderZ, "incy");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = arg_uplo;
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_y) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.y", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[10] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_y, arg_incy, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_y, arg_incy, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incy, byRef);
        Value *args[4] = {malins, arg_y, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *true_incy = arg_incy;
  Value *free_y = nullptr;
  Value *input_y = nullptr;
  Value *free_input_y = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incy) {
        arg_incy = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incy");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incy");
        Builder2.CreateStore(arg_incy, alloc);
        arg_incy = Builder2.CreatePointerCast(
            alloc, type_incy, "cast.incy");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
    if (active_beta) {
      input_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_input_y = input_y;
      if (type_y->isIntegerTy()) {
        input_y = Builder2.CreatePtrToInt(input_y, type_y);
      } else if (input_y->getType() != type_y){
        input_y = Builder2.CreatePointerCast(input_y, type_y);
      }
    }
    if (cache_y) {
      arg_y = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.y");
      free_y = arg_y;
      if (type_y->isIntegerTy()) {
        arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
      } else if (arg_y->getType() != type_y){
        arg_y = Builder2.CreatePointerCast(arg_y, type_y);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
    if (type_y->isIntegerTy())
      arg_y = Builder2.CreatePtrToInt(arg_y, type_y);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_y = active_y
     ? gutils->invertPointerM(orig_y, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_x, Value *d_beta, Value *d_y  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_beta && d_y) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_beta}) _0.push_back(item);
        for (auto item : {arg_y, (cache_y ? const_one : arg_incy)}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_x && d_y) {
        {
      // BlasCall symv
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_uplo}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_alpha}) _1.push_back(item);
        for (auto item : {arg_A}) _1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) _1.push_back(item);
        for (auto item : {d_x, arg_incx}) _1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _1.push_back(item);
        for (auto item : {d_y, arg_incy}) _1.push_back(item);
        if (byRef) {
    auto tmpF_symv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symv" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_symv && tmpF_symv->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_symv->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symv = blas.prefix + blas.floatType + "symv" + blas.suffix;
    auto derivcall_symv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symv), FTsymv);
    if (auto F = dyn_cast<Function>(derivcall_symv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symv);
      auto newF = attribute_symv(blas, F);
      derivcall_symv = FunctionCallee(derivcall_symv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symv, _1, Defs));
        }
        }
      if (d_A && d_y) {
        {
      // BlasCall symv
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_uplo}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_alpha}) _2.push_back(item);
        for (auto item : {d_A, arg_lda}) _2.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _2.push_back(item);
        for (auto item : {d_y, arg_incy}) _2.push_back(item);
        if (byRef) {
    auto tmpF_symv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symv" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_symv && tmpF_symv->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_symv->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symv = blas.prefix + blas.floatType + "symv" + blas.suffix;
    auto derivcall_symv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symv), FTsymv);
    if (auto F = dyn_cast<Function>(derivcall_symv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symv);
      auto newF = attribute_symv(blas, F);
      derivcall_symv = FunctionCallee(derivcall_symv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symv, _2, Defs));
        }
        }
      if (d_y && d_alpha) {
        {
      // BlasCall symv
        std::vector<Value *>_3;
        if (cblas) _3.push_back(arg_layout);
        if (cublas) _3.push_back(arg_handle);
        for (auto item : {arg_uplo}) _3.push_back(item);
        for (auto item : {arg_n}) _3.push_back(item);
        for (auto item : {d_alpha}) _3.push_back(item);
        for (auto item : {arg_A}) _3.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) _3.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) _3.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _3.push_back(item);
        for (auto item : {d_y, arg_incy}) _3.push_back(item);
        if (byRef) {
    auto tmpF_symv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symv" + blas.suffix));
           _3.push_back(ConstantInt::get((tmpF_symv && tmpF_symv->getFunctionType()->getNumParams() > _3.size() ) ? tmpF_symv->getFunctionType()->getParamType(_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _3) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symv = blas.prefix + blas.floatType + "symv" + blas.suffix;
    auto derivcall_symv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symv), FTsymv);
    if (auto F = dyn_cast<Function>(derivcall_symv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symv);
      auto newF = attribute_symv(blas, F);
      derivcall_symv = FunctionCallee(derivcall_symv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symv, _3, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_y) {
        {
      // BlasCall scal
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_beta}) _0.push_back(item);
        for (auto item : {d_y, arg_incy}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_x, d_beta, d_y);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "symv" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_y = active_y
     ? lookup(gutils->invertPointerM(orig_y, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_y && need_y)
      arg_y = lookup(arg_y, Builder2);
    if (!cache_incy && need_incy)
      arg_incy = lookup(arg_incy, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_y) {
      rt_inactive_y = lookup(rt_inactive_y, Builder2);
    }
  }
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_x, Value *d_y) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha && d_y) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_alpha = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".alpha.active");
          nextBlock_alpha = gutils->addReverseBlock(activeBlock, bb_name + ".alpha.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_alpha, nextBlock_alpha, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len1 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_Ax = len1;
    CallInst * malloccall_mat_Ax = nullptr;
    Value * true_mat_Ax = CreateAllocation(Builder2, fpType, size_Ax, "mat_Ax", &malloccall_mat_Ax);
    MDNode *ident_mat_Ax = nullptr;
    if (malloccall_mat_Ax) {
      ident_mat_Ax = MDNode::getDistinct(malloccall_mat_Ax->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_Ax->getContext()))});
      malloccall_mat_Ax->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_Ax->getContext(), {ident_mat_Ax}));
    }
    Value * mat_Ax = true_mat_Ax;
    if (type_vec_like->isIntegerTy()) {
      mat_Ax = Builder2.CreatePtrToInt(mat_Ax, type_vec_like);
    } else if (mat_Ax->getType() != type_vec_like){
      mat_Ax = Builder2.CreatePointerCast(mat_Ax, type_vec_like);
    }
        {
      // BlasCall symv
        std::vector<Value *>alpha_0;
        if (cblas) alpha_0.push_back(arg_layout);
        if (cublas) alpha_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) alpha_0.push_back(item);
        for (auto item : {arg_A}) alpha_0.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) alpha_0.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) alpha_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 0.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.0.0")}) alpha_0.push_back(item);
        for (auto item : {mat_Ax}) alpha_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_0.push_back(item);
        if (byRef) {
    auto tmpF_symv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symv" + blas.suffix));
           alpha_0.push_back(ConstantInt::get((tmpF_symv && tmpF_symv->getFunctionType()->getNumParams() > alpha_0.size() ) ? tmpF_symv->getFunctionType()->getParamType(alpha_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symv = blas.prefix + blas.floatType + "symv" + blas.suffix;
    auto derivcall_symv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symv), FTsymv);
    if (auto F = dyn_cast<Function>(derivcall_symv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symv);
      auto newF = attribute_symv(blas, F);
      derivcall_symv = FunctionCallee(derivcall_symv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symv, alpha_0, Defs));
        }
        {
      // BlasCall dot
        std::vector<Value *>alpha_1;
        if (cublas) alpha_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) alpha_1.push_back(item);
        for (auto item : {d_y, arg_incy}) alpha_1.push_back(item);
        for (auto item : {mat_Ax}) alpha_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) alpha_1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) alpha_1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : alpha_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, alpha_1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, alpha_1[alpha_1.size()-1]);
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_Ax);
      if (freecall && ident_mat_Ax) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_Ax}));
      }
    }
        if (nextBlock_alpha && byRefFloat) {
          Builder2.CreateBr(nextBlock_alpha);
          Builder2.SetInsertPoint(nextBlock_alpha);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_alpha);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_alpha);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_A && d_y) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len1 = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tmp = len1;
    CallInst * malloccall_mat_tmp = nullptr;
    Value * true_mat_tmp = CreateAllocation(Builder2, fpType, size_tmp, "mat_tmp", &malloccall_mat_tmp);
    MDNode *ident_mat_tmp = nullptr;
    if (malloccall_mat_tmp) {
      ident_mat_tmp = MDNode::getDistinct(malloccall_mat_tmp->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tmp->getContext()))});
      malloccall_mat_tmp->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tmp->getContext(), {ident_mat_tmp}));
    }
    Value * mat_tmp = true_mat_tmp;
    if (type_vec_like->isIntegerTy()) {
      mat_tmp = Builder2.CreatePtrToInt(mat_tmp, type_vec_like);
    } else if (mat_tmp->getType() != type_vec_like){
      mat_tmp = Builder2.CreatePointerCast(mat_tmp, type_vec_like);
    }
        {
      // BlasCall copy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {mat_tmp}) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_0, Defs));
        }
        {
      // BlasCall syr2
        std::vector<Value *>A_1;
        if (cblas) A_1.push_back(arg_layout);
        if (cublas) A_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {arg_alpha}) A_1.push_back(item);
        for (auto item : {arg_x, (cache_x ? const_one : arg_incx)}) A_1.push_back(item);
        for (auto item : {d_y, arg_incy}) A_1.push_back(item);
        for (auto item : {d_A, arg_lda}) A_1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsyr2 = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_syr2 = blas.prefix + blas.floatType + "syr2" + blas.suffix;
    auto derivcall_syr2 = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_syr2), FTsyr2);
    if (auto F = dyn_cast<Function>(derivcall_syr2.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_syr2);
      auto newF = attribute_syr2(blas, F);
      derivcall_syr2 = FunctionCallee(derivcall_syr2.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_syr2, A_1, Defs));
        }
        {
      // BlasCall copy
        std::vector<Value *>A_2;
        if (cublas) A_2.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) A_2.push_back(item);
        for (auto item : {mat_tmp}) A_2.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_2.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Primal : ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_2) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_2, Defs));
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tmp);
      if (freecall && ident_mat_tmp) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tmp}));
      }
    }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_x && d_x && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall symv
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
    auto tmpF_symv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symv" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_symv && tmpF_symv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_symv->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symv = blas.prefix + blas.floatType + "symv" + blas.suffix;
    auto derivcall_symv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symv), FTsymv);
    if (auto F = dyn_cast<Function>(derivcall_symv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symv);
      auto newF = attribute_symv(blas, F);
      derivcall_symv = FunctionCallee(derivcall_symv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symv, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
      if (active_beta && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_beta = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".beta.active");
          nextBlock_beta = gutils->addReverseBlock(activeBlock, bb_name + ".beta.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_beta, nextBlock_beta, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall dot
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        for (auto item : {input_y, (cache_y ? const_one : arg_incy)}) args1.push_back(item);
        if (byRef) {
        }
           if (cublasv2) args1.push_back(Builder2.CreateAlloca(fpType));
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTdot = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : fpType, tys, false);
    auto str_dot = blas.prefix + blas.floatType + "dot" + blas.suffix;
    auto derivcall_dot = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_dot), FTdot);
    if (auto F = dyn_cast<Function>(derivcall_dot.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_dot);
      auto newF = attribute_dot(blas, F);
      derivcall_dot = FunctionCallee(derivcall_dot.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_dot, args1, Defs));
        toadd = cubcall;
         if (cublasv2) toadd = Builder2.CreateLoad(fpType, args1[args1.size()-1]);
        if (nextBlock_beta && byRefFloat) {
          Builder2.CreateBr(nextBlock_beta);
          Builder2.SetInsertPoint(nextBlock_beta);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_beta);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_beta);
      }
        }
        }
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_y && d_y) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_y = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".y.active");
          nextBlock_y = gutils->addReverseBlock(activeBlock, bb_name + ".y.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_y, nextBlock_y, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall scal
        std::vector<Value *>args1;
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {d_y, arg_incy}) args1.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, cache_x ? ValueType::Primal : ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTscal = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_scal = blas.prefix + blas.floatType + "scal" + blas.suffix;
    auto derivcall_scal = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_scal), FTscal);
    if (auto F = dyn_cast<Function>(derivcall_scal.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_scal);
      auto newF = attribute_scal(blas, F);
      derivcall_scal = FunctionCallee(derivcall_scal.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_scal, args1, Defs));
        if (nextBlock_y) {
          Builder2.CreateBr(nextBlock_y);
          Builder2.SetInsertPoint(nextBlock_y);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_y);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_y);
      }
        }
        }
      }
    },
    d_A, d_x, d_y  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_y) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_y);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_syrk(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_trans = 1 + offset;
  const auto orig_trans = call.getArgOperand(pos_trans);
  auto arg_trans = gutils->getNewFromOriginal(orig_trans);
  const auto type_trans = arg_trans->getType();
  const bool overwritten_trans = (cacheMode ? overwritten_args[pos_trans] : false);

  const int pos_n = 2 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_k = 3 + offset;
  const auto orig_k = call.getArgOperand(pos_k);
  auto arg_k = gutils->getNewFromOriginal(orig_k);
  const auto type_k = arg_k->getType();
  const bool overwritten_k = (cacheMode ? overwritten_args[pos_k] : false);

  const int pos_alpha = 4 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 5 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 6 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_beta = 7 + offset;
  const auto orig_beta = call.getArgOperand(pos_beta);
  auto arg_beta = gutils->getNewFromOriginal(orig_beta);
  const auto type_beta = arg_beta->getType();
  const bool overwritten_beta = (cacheMode ? overwritten_args[pos_beta] : false);
  bool active_beta = !gutils->isConstantValue(orig_beta);
  Value *rt_inactive_beta = nullptr;

  const int pos_C = 8 + offset;
  const auto orig_C = call.getArgOperand(pos_C);
  auto arg_C = gutils->getNewFromOriginal(orig_C);
  const auto type_C = arg_C->getType();
  const bool overwritten_C = (cacheMode ? overwritten_args[pos_C] : false);
  bool active_C = !gutils->isConstantValue(orig_C);
  Value *rt_inactive_C = nullptr;

  const int pos_ldc = 9 + offset;
  const auto orig_ldc = call.getArgOperand(pos_ldc);
  auto arg_ldc = gutils->getNewFromOriginal(orig_ldc);
  const auto type_ldc = arg_ldc->getType();
  const bool overwritten_ldc = (cacheMode ? overwritten_args[pos_ldc] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (byRefFloat && active_beta) {
      auto shadow_beta = gutils->invertPointerM(orig_beta, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_beta = BuilderZ.CreateICmpEQ(shadow_beta, arg_beta, "rt.tmp.inactive." "beta");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_beta_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_beta, i), arg_beta, "rt.tmp.inactive." "beta." + std::to_string(i));
          if (i == 0) rt_inactive_beta = rt_inactive_beta_tmp;
          else rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_beta_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      auto shadow_C = gutils->invertPointerM(orig_C, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_C = BuilderZ.CreateICmpEQ(shadow_C, arg_C, "rt.tmp.inactive." "C");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_C_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_C, i), arg_C, "rt.tmp.inactive." "C." + std::to_string(i));
          if (i == 0) rt_inactive_C = rt_inactive_C_tmp;
          else rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_C_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    Value *rt_inactive_out = nullptr;
    if (active_C) {
      rt_inactive_out = rt_inactive_C;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (byRefFloat && active_beta) {
      rt_inactive_beta = BuilderZ.CreateOr(rt_inactive_beta, rt_inactive_out, "rt.inactive." "beta");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_beta) : rt_inactive_beta;
    }
    if (active_C) {
      rt_inactive_C = BuilderZ.CreateOr(rt_inactive_C, rt_inactive_out, "rt.inactive." "C");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_C) : rt_inactive_C;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "syrk" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_A || active_C;
  bool need_trans = active_A;
  bool need_n = active_A || active_C;
  bool need_k = active_A;
  bool need_alpha = active_A;
  bool need_A = active_A;
  bool need_lda = active_A;
  bool need_beta = active_C;
  bool need_C = false;
  bool need_ldc = active_A || active_C;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_trans = cacheMode && byRef && overwritten_trans && need_trans;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_k = cacheMode && byRef && overwritten_k && need_k;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_beta = cacheMode && byRef && overwritten_beta && need_beta;
  bool cache_C = cacheMode && overwritten_C && need_C;
  bool cache_ldc = cacheMode && byRef && overwritten_ldc && need_ldc;
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_trans)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_k)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_beta)
    cacheTypes.push_back(fpType);
  if (cache_ldc)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_C)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_trans, cache_trans, charType, cacheValues, BuilderZ, "trans");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_k, cache_k, intType, cacheValues, BuilderZ, "k");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_beta, cache_beta, fpType, cacheValues, BuilderZ, "beta");
        addValueToCache(arg_ldc, cache_ldc, intType, cacheValues, BuilderZ, "ldc");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      Value *normal = is_normal(BuilderZ, arg_trans, byRef, cublas);
      M = BuilderZ.CreateSelect(normal, arg_n, arg_k);
      N = BuilderZ.CreateSelect(normal, arg_k, arg_n);
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[2] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_C) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.C", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[10] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (byRef) valueTypes[3] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_C, arg_ldc, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldc, byRef);
        Value *args[5] = {malins, arg_C, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldc = arg_ldc;
  Value *ldc = true_ldc;
  Value *free_C = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_trans) {
        arg_trans = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.trans");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.trans");
        Builder2.CreateStore(arg_trans, alloc);
        arg_trans = Builder2.CreatePointerCast(
            alloc, type_trans, "cast.trans");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_k) {
        arg_k = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.k");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.k");
        Builder2.CreateStore(arg_k, alloc);
        arg_k = Builder2.CreatePointerCast(
            alloc, type_k, "cast.k");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_beta) {
        arg_beta = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.beta");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.beta");
        Builder2.CreateStore(arg_beta, alloc);
        arg_beta = Builder2.CreatePointerCast(
            alloc, type_beta, "cast.beta");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldc) {
        arg_ldc = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldc");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldc");
        Builder2.CreateStore(arg_ldc, alloc);
        arg_ldc = Builder2.CreatePointerCast(
            alloc, type_ldc, "cast.ldc");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_C) {
      arg_C = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.C");
      free_C = arg_C;
      if (type_C->isIntegerTy()) {
        arg_C = Builder2.CreatePtrToInt(arg_C, type_C);
      } else if (arg_C->getType() != type_C){
        arg_C = Builder2.CreatePointerCast(arg_C, type_C);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_beta = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_beta = active_beta
     ? gutils->invertPointerM(orig_beta, Builder2)
     : nullptr;
    }
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_C = active_C
     ? gutils->invertPointerM(orig_C, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_beta, Value *d_C  ) {
      Value *dres = nullptr;
        {
      // Seq
     Value *first_use_beta1 = Builder2.getTrue();
      if (d_C && d_beta) {
        {
      // BlasCall axpy
        std::vector<Value *>_0;
        if (cublas) _0.push_back(arg_handle);
        for (auto item :             ({std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument within syrk of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
            ArrayRef<Value*>(); })) _0.push_back(item);
        for (auto item : {d_beta}) _0.push_back(item);
        for (auto item : {arg_C}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, _0, Defs));
        }
        }
      if (d_C && d_A) {
        {
      // BlasCall syr2k
        std::vector<Value *>_1;
        if (cblas) _1.push_back(arg_layout);
        if (cublas) _1.push_back(arg_handle);
        for (auto item : {arg_uplo}) _1.push_back(item);
        for (auto item : {arg_trans}) _1.push_back(item);
        for (auto item : {arg_n}) _1.push_back(item);
        for (auto item : {arg_k}) _1.push_back(item);
        for (auto item : {arg_alpha}) _1.push_back(item);
        for (auto item : {arg_A}) _1.push_back(item);
        for (auto item : {arg_lda}) _1.push_back(item);
        for (auto item : {d_A, arg_lda}) _1.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _1.push_back(item);
        for (auto item : {d_C, arg_ldc}) _1.push_back(item);
        if (byRef) {
    auto tmpF_syr2k = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "syr2k" + blas.suffix));
           _1.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(_1.size()) : intType, 1));
           _1.push_back(ConstantInt::get((tmpF_syr2k && tmpF_syr2k->getFunctionType()->getNumParams() > _1.size() ) ? tmpF_syr2k->getFunctionType()->getParamType(_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _1) tys.push_back(arg->getType());
    llvm::FunctionType *FTsyr2k = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_syr2k = blas.prefix + blas.floatType + "syr2k" + blas.suffix;
    auto derivcall_syr2k = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_syr2k), FTsyr2k);
    if (auto F = dyn_cast<Function>(derivcall_syr2k.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_syr2k);
      auto newF = attribute_syr2k(blas, F);
      derivcall_syr2k = FunctionCallee(derivcall_syr2k.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_syr2k, _1, Defs));
        }
        }
      if (d_C && d_alpha) {
        {
      // BlasCall syrk
        std::vector<Value *>_2;
        if (cblas) _2.push_back(arg_layout);
        if (cublas) _2.push_back(arg_handle);
        for (auto item : {arg_uplo}) _2.push_back(item);
        for (auto item : {arg_trans}) _2.push_back(item);
        for (auto item : {arg_n}) _2.push_back(item);
        for (auto item : {arg_k}) _2.push_back(item);
        for (auto item : {d_alpha}) _2.push_back(item);
        for (auto item : {arg_A}) _2.push_back(item);
        for (auto item : {arg_lda}) _2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> farg_0;
 for (auto tmp : {arg_beta} ) farg_0.push_back(tmp);
SmallVector<Value*, 1> farg_1;
 for (auto tmp : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")} ) farg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for (size_t i=0; i<farg_0.size(); i++) 
  vals.push_back(CreateSelect(Builder2, first_use_beta1, farg_0[i], farg_1[i]));
first_use_beta1 = Builder2.getFalse();
 vals; })) _2.push_back(item);
        for (auto item : {d_C, arg_ldc}) _2.push_back(item);
        if (byRef) {
    auto tmpF_syrk = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "syrk" + blas.suffix));
           _2.push_back(ConstantInt::get((tmpF_syrk && tmpF_syrk->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_syrk->getFunctionType()->getParamType(_2.size()) : intType, 1));
           _2.push_back(ConstantInt::get((tmpF_syrk && tmpF_syrk->getFunctionType()->getNumParams() > _2.size() ) ? tmpF_syrk->getFunctionType()->getParamType(_2.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _2) tys.push_back(arg->getType());
    llvm::FunctionType *FTsyrk = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_syrk = blas.prefix + blas.floatType + "syrk" + blas.suffix;
    auto derivcall_syrk = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_syrk), FTsyrk);
    if (auto F = dyn_cast<Function>(derivcall_syrk.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_syrk);
      auto newF = attribute_syrk(blas, F);
      derivcall_syrk = FunctionCallee(derivcall_syrk.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_syrk, _2, Defs));
        }
        }
        {
      // FirstUse
          auto CI = cast<ConstantInt>(first_use_beta1);
        if (CI->isOne()) {
      if (d_C) {
        {
      // BlasCall lascl
        std::vector<Value *>_0;
        if (cblas) _0.push_back(arg_layout);
        if (cublas) _0.push_back(arg_handle);
        for (auto item : {arg_uplo}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) _0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) _0.push_back(item);
        for (auto item : {arg_beta}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {arg_n}) _0.push_back(item);
        for (auto item : {d_C, arg_ldc}) _0.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) _0.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           _0.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > _0.size() ) ? tmpF_lascl->getFunctionType()->getParamType(_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 0);
    SmallVector<Type*, 1> tys; for (auto arg : _0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, _0, Defs));
        }
        }
        }
        }
        }
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_beta, d_C);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "syrk" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_beta = UndefValue::get(fpType);
    Value *d_C = active_C
     ? lookup(gutils->invertPointerM(orig_C, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_trans && need_trans)
      arg_trans = lookup(arg_trans, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_k && need_k)
      arg_k = lookup(arg_k, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_beta && need_beta)
      arg_beta = lookup(arg_beta, Builder2);
    if (!cache_C && need_C)
      arg_C = lookup(arg_C, Builder2);
    if (!cache_ldc && need_ldc)
      arg_ldc = lookup(arg_ldc, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (byRef && active_beta) {
      rt_inactive_beta = lookup(rt_inactive_beta, Builder2);
    }
    if (active_C) {
      rt_inactive_C = lookup(rt_inactive_C, Builder2);
    }
  }
    llvm::Value* arg_transposed_trans = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_C) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha) {
        Value *toadd = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument alpha within syrk of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_C && d_A) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
        {
      // BlasCall symm
        std::vector<Value *>A_0;
        if (cblas) A_0.push_back(arg_layout);
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({auto brow_2 = {to_blas_callconv(Builder2, valuer, byRef, cublas, nullptr, allocationBuilder, "constant.char.r")}; auto brow_1 = {to_blas_callconv(Builder2, valuel, byRef, cublas, nullptr, allocationBuilder, "constant.char.l")}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_0.push_back(item);
        for (auto item : {arg_uplo}) A_0.push_back(item);
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_k}; auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_1 = ({auto concat_0 = {arg_n}; auto concat_1 = {arg_k}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_0.push_back(item);
        for (auto item : {arg_alpha}) A_0.push_back(item);
        for (auto item : {d_C, arg_ldc}) A_0.push_back(item);
        for (auto item : {arg_A}) A_0.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {arg_trans}, arg_lda, arg_k, arg_n, cache_A, byRef, cublas)}) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")}) A_0.push_back(item);
        for (auto item : {d_A, arg_lda}) A_0.push_back(item);
        if (byRef) {
    auto tmpF_symm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "symm" + blas.suffix));
           A_0.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_symm->getFunctionType()->getParamType(A_0.size()) : intType, 1));
           A_0.push_back(ConstantInt::get((tmpF_symm && tmpF_symm->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_symm->getFunctionType()->getParamType(A_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTsymm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_symm = blas.prefix + blas.floatType + "symm" + blas.suffix;
    auto derivcall_symm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_symm), FTsymm);
    if (auto F = dyn_cast<Function>(derivcall_symm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_symm);
      auto newF = attribute_symm(blas, F);
      derivcall_symm = FunctionCallee(derivcall_symm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_symm, A_0, Defs));
        }
        {
      // For
      auto lim_ar = {arg_n};
      Value *lim = (*lim_ar.begin());
      lim = load_if_ref(Builder2, intType, lim, byRef);
      BasicBlock *current = Builder2.GetInsertBlock();
      auto loopBlock = gutils->addReverseBlock(current,current->getName() + "_loop");
      auto endBlock = gutils->addReverseBlock(loopBlock,current->getName() + "_end", /*fork*/true, /*push*/false);
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, ConstantInt::get(lim->getType(), 0)), endBlock, loopBlock);
      Builder2.SetInsertPoint(loopBlock);
      auto phi_i = Builder2.CreatePHI(lim->getType(), 2);
      phi_i->addIncoming(ConstantInt::get(lim->getType(), 0), current);
      auto phi_i_inc = Builder2.CreateAdd(phi_i, ConstantInt::get(lim->getType(), 1), "", true, true);
      auto phi_b_i = to_blas_callconv(Builder2, phi_i, byRef, cublas, julia_decl_type, allocationBuilder, "for.i");
      Value *for_res = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_k}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_alpha} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : {d_C, arg_ldc} ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : {phi_b_i} ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : {phi_b_i} ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  if (!byRefFloat) ptr = Builder2.CreateLoad(fpType, ptr);
  SmallVector<Value*, 1> vals = { ptr };
vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, fpType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, fpType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateFMul(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "FMul" ));
 }
 vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto concat_0 = {arg_A}; auto concat_1 = {get_cached_mat_width(Builder2, {arg_trans}, arg_lda, arg_k, arg_n, cache_A, byRef, cublas)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({auto brow_2 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto brow_1 = {phi_b_i}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({auto brow_2 = {phi_b_i}; auto brow_1 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({auto brow_2 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}; auto brow_1 = {get_cached_mat_width(Builder2, {arg_trans}, arg_lda, arg_k, arg_n, cache_A, byRef, cublas)}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : {d_A, arg_lda} ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({auto brow_2 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto brow_1 = {phi_b_i}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({auto brow_2 = {phi_b_i}; auto brow_1 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({auto brow_2 = {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}; auto brow_1 = {arg_lda}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_0, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      phi_i->addIncoming(phi_i_inc, Builder2.GetInsertBlock());
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, phi_i_inc), endBlock, loopBlock);
      Builder2.SetInsertPoint(endBlock);
      {
        auto found = gutils->reverseBlockToPrimal.find(endBlock);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(endBlock);
      }
        }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
      if (active_beta) {
        Value *toadd = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument beta within syrk of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_beta, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_beta);
          }
        }
      }
      if (active_C && d_C) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_C = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".C.active");
          nextBlock_C = gutils->addReverseBlock(activeBlock, bb_name + ".C.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_C, nextBlock_C, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall lascl
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")}) args1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1.0), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1.0")}) args1.push_back(item);
        for (auto item : {arg_beta}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {d_C, arg_ldc}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_lascl = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lascl" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_lascl && tmpF_lascl->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_lascl->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTlascl = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lascl = blas.prefix + blas.floatType + "lascl" + blas.suffix;
    auto derivcall_lascl = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lascl), FTlascl);
    if (auto F = dyn_cast<Function>(derivcall_lascl.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lascl);
      auto newF = attribute_lascl(blas, F);
      derivcall_lascl = FunctionCallee(derivcall_lascl.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lascl, args1, Defs));
        if (nextBlock_C) {
          Builder2.CreateBr(nextBlock_C);
          Builder2.SetInsertPoint(nextBlock_C);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_C);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_C);
      }
        }
        }
      }
    },
    d_A, d_C  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_C) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_C);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_trmm(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_side = 0 + offset;
  const auto orig_side = call.getArgOperand(pos_side);
  auto arg_side = gutils->getNewFromOriginal(orig_side);
  const auto type_side = arg_side->getType();
  const bool overwritten_side = (cacheMode ? overwritten_args[pos_side] : false);

  const int pos_uplo = 1 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_transa = 2 + offset;
  const auto orig_transa = call.getArgOperand(pos_transa);
  auto arg_transa = gutils->getNewFromOriginal(orig_transa);
  const auto type_transa = arg_transa->getType();
  const bool overwritten_transa = (cacheMode ? overwritten_args[pos_transa] : false);

  const int pos_diag = 3 + offset;
  const auto orig_diag = call.getArgOperand(pos_diag);
  auto arg_diag = gutils->getNewFromOriginal(orig_diag);
  const auto type_diag = arg_diag->getType();
  const bool overwritten_diag = (cacheMode ? overwritten_args[pos_diag] : false);

  const int pos_m = 4 + offset;
  const auto orig_m = call.getArgOperand(pos_m);
  auto arg_m = gutils->getNewFromOriginal(orig_m);
  const auto type_m = arg_m->getType();
  const bool overwritten_m = (cacheMode ? overwritten_args[pos_m] : false);

  const int pos_n = 5 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_alpha = 6 + offset;
  const auto orig_alpha = call.getArgOperand(pos_alpha);
  auto arg_alpha = gutils->getNewFromOriginal(orig_alpha);
  const auto type_alpha = arg_alpha->getType();
  const bool overwritten_alpha = (cacheMode ? overwritten_args[pos_alpha] : false);
  bool active_alpha = !gutils->isConstantValue(orig_alpha);
  Value *rt_inactive_alpha = nullptr;

  const int pos_A = 7 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 8 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 9 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 10 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (byRefFloat && active_alpha) {
      auto shadow_alpha = gutils->invertPointerM(orig_alpha, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_alpha = BuilderZ.CreateICmpEQ(shadow_alpha, arg_alpha, "rt.tmp.inactive." "alpha");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_alpha_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_alpha, i), arg_alpha, "rt.tmp.inactive." "alpha." + std::to_string(i));
          if (i == 0) rt_inactive_alpha = rt_inactive_alpha_tmp;
          else rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_alpha_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    Value *rt_inactive_out = nullptr;
    if (active_B) {
      rt_inactive_out = rt_inactive_B;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (byRefFloat && active_alpha) {
      rt_inactive_alpha = BuilderZ.CreateOr(rt_inactive_alpha, rt_inactive_out, "rt.inactive." "alpha");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_alpha) : rt_inactive_alpha;
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trmm" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = type_alpha;
  Type* blasCharType = type_side;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_side;
  Type* blasIntType = type_m;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_side = active_A || active_B;
  bool need_uplo = active_A || active_B;
  bool need_transa = active_A || active_B;
  bool need_diag = active_A || active_B;
  bool need_m = active_A || active_B;
  bool need_n = active_A || active_B;
  bool need_alpha = active_A || active_B;
  bool need_A = active_B;
  bool need_lda = active_A || active_B;
  bool need_B = false;
  bool need_ldb = active_A || active_B;
  bool cache_side = cacheMode && byRef && overwritten_side && need_side;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_transa = cacheMode && byRef && overwritten_transa && need_transa;
  bool cache_diag = cacheMode && byRef && overwritten_diag && need_diag;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  // we cache the following matrix,
  // since one rule uses input<B>
  if (active_A) {
    need_B = true;
    cache_B = true;
  }
  if (cache_side)
    cacheTypes.push_back(charType);
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_transa)
    cacheTypes.push_back(charType);
  if (cache_diag)
    cacheTypes.push_back(charType);
  if (cache_m)
    cacheTypes.push_back(intType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_alpha)
    cacheTypes.push_back(fpType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_transa, cache_transa, charType, cacheValues, BuilderZ, "transa");
        addValueToCache(arg_m, cache_m, intType, cacheValues, BuilderZ, "m");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_alpha, cache_alpha, fpType, cacheValues, BuilderZ, "alpha");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      Value *normal = is_left(BuilderZ, arg_side, byRef, cublas);
      M = N = BuilderZ.CreateSelect(normal, arg_m, arg_n);
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[1] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[6] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_m;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[10] = ValueType::Primal;
      if (byRef) valueTypes[11] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[6] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  Value *input_B = nullptr;
  Value *free_input_B = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_transa) {
        arg_transa = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.transa");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.transa");
        Builder2.CreateStore(arg_transa, alloc);
        arg_transa = Builder2.CreatePointerCast(
            alloc, type_transa, "cast.transa");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_m) {
        arg_m = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.m");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.m");
        Builder2.CreateStore(arg_m, alloc);
        arg_m = Builder2.CreatePointerCast(
            alloc, type_m, "cast.m");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRefFloat) {
      if (cache_alpha) {
        arg_alpha = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.alpha");
        auto alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "byref.alpha");
        Builder2.CreateStore(arg_alpha, alloc);
        arg_alpha = Builder2.CreatePointerCast(
            alloc, type_alpha, "cast.alpha");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (active_A) {
      input_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_input_B = input_B;
      if (type_B->isIntegerTy()) {
        input_B = Builder2.CreatePtrToInt(input_B, type_B);
      } else if (input_B->getType() != type_B){
        input_B = Builder2.CreatePointerCast(input_B, type_B);
      }
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_alpha = Constant::getNullValue(gutils->getShadowType(fpType));
    if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) {
      d_alpha = active_alpha
     ? gutils->invertPointerM(orig_alpha, Builder2)
     : nullptr;
    }
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_alpha, Value *d_A, Value *d_B  ) {
      Value *dres = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument  within trmm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_alpha, d_A, d_B);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trmm" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_alpha = UndefValue::get(fpType);
    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    if (!cache_side && need_side)
      arg_side = lookup(arg_side, Builder2);
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_transa && need_transa)
      arg_transa = lookup(arg_transa, Builder2);
    if (!cache_diag && need_diag)
      arg_diag = lookup(arg_diag, Builder2);
    if (!cache_m && need_m)
      arg_m = lookup(arg_m, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_alpha && need_alpha)
      arg_alpha = lookup(arg_alpha, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (byRef && active_alpha) {
      rt_inactive_alpha = lookup(rt_inactive_alpha, Builder2);
    }
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
  }
    llvm::Value* arg_transposed_transa = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_alpha) {
        Value *toadd = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument alpha within trmm of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
        if (toadd) {
          IRBuilder <>Builder3(&call);
          Builder3.setFastMathFlags(getFast());
          if (auto I = dyn_cast<Instruction>(toadd)) Builder3.SetInsertPoint(I->getNextNode() ? I->getNextNode() : I);
          if (byRefFloat) {
            ((DiffeGradientUtils *)gutils)->addToInvertedPtrDiffe(&call, nullptr, fpType, 0, (called->getParent()->getDataLayout().getTypeSizeInBits(fpType)/8), orig_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2);
          } else {
            addToDiffe(arg_alpha, toadd, (isa<Instruction>(toadd) && cast<Instruction>(toadd)->getNextNode()) ? Builder3 : Builder2, type_alpha);
          }
        }
      }
      if (active_A && d_B && d_A) {
        Value *toadd = nullptr;
        {
      // For
      auto lim_ar = ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; });
      Value *lim = (*lim_ar.begin());
      lim = load_if_ref(Builder2, intType, lim, byRef);
      BasicBlock *current = Builder2.GetInsertBlock();
      auto loopBlock = gutils->addReverseBlock(current,current->getName() + "_loop");
      auto endBlock = gutils->addReverseBlock(loopBlock,current->getName() + "_end", /*fork*/true, /*push*/false);
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, ConstantInt::get(lim->getType(), 0)), endBlock, loopBlock);
      Builder2.SetInsertPoint(loopBlock);
      auto phi_i = Builder2.CreatePHI(lim->getType(), 2);
      phi_i->addIncoming(ConstantInt::get(lim->getType(), 0), current);
      auto phi_i_inc = Builder2.CreateAdd(phi_i, ConstantInt::get(lim->getType(), 1), "", true, true);
      auto phi_b_i = to_blas_callconv(Builder2, phi_i_inc, byRef, cublas, julia_decl_type, allocationBuilder, "for.i");
      Value *for_res = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall gemv
        std::vector<Value *>A_0;
        if (cblas) A_0.push_back(arg_layout);
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({auto concat_0 = ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_m} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }); auto concat_1 = {arg_n}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({auto concat_0 = {arg_m}; auto concat_1 = ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }); concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {arg_alpha}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({auto brow_0 = {arg_transa}; SmallVector<Value*, 1> vals = {to_blas_callconv(Builder2, get_blas_row(Builder2, brow_0, byRef, cublas)[0], byRef, cublas, julia_decl_type, allocationBuilder, "")}; vals;}) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_0.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateNot(subarg_0), byRef, cublas, julia_decl_type, allocationBuilder, "Not" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateXor(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Xor" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {d_B, arg_ldb} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({auto concat_0 = {input_B}; auto concat_1 = {arg_m}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAnd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "And" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_0.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateNot(subarg_0), byRef, cublas, julia_decl_type, allocationBuilder, "Not" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAnd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "And" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({auto brow_0 = {arg_transa}; SmallVector<Value*, 1> vals = {to_blas_callconv(Builder2, get_blas_row(Builder2, brow_0, byRef, cublas)[0], byRef, cublas, julia_decl_type, allocationBuilder, "")}; vals;}) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_0.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateNot(subarg_0), byRef, cublas, julia_decl_type, allocationBuilder, "Not" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateXor(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Xor" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({auto concat_0 = {input_B}; auto concat_1 = {arg_m}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {d_B, arg_ldb} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_left(Builder2, arg_side, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({auto brow_0 = {arg_transa}; SmallVector<Value*, 1> vals = {to_blas_callconv(Builder2, get_blas_row(Builder2, brow_0, byRef, cublas)[0], byRef, cublas, julia_decl_type, allocationBuilder, "")}; vals;}) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_0.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateNot(subarg_0), byRef, cublas, julia_decl_type, allocationBuilder, "Not" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateXor(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Xor" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {arg_m} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_ldb} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_0.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")}) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : {d_A, arg_lda} ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_0.push_back(item);
        if (byRef) {
    auto tmpF_gemv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemv" + blas.suffix));
           A_0.push_back(ConstantInt::get((tmpF_gemv && tmpF_gemv->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_gemv->getFunctionType()->getParamType(A_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemv = blas.prefix + blas.floatType + "gemv" + blas.suffix;
    auto derivcall_gemv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemv), FTgemv);
    if (auto F = dyn_cast<Function>(derivcall_gemv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemv);
      auto newF = attribute_gemv(blas, F);
      derivcall_gemv = FunctionCallee(derivcall_gemv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemv, A_0, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      phi_i->addIncoming(phi_i_inc, Builder2.GetInsertBlock());
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, phi_i_inc), endBlock, loopBlock);
      Builder2.SetInsertPoint(endBlock);
      {
        auto found = gutils->reverseBlockToPrimal.find(endBlock);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(endBlock);
      }
        }
      }
      if (active_B && d_B) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall trmm
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_side}) args1.push_back(item);
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {(arg_transposed_transa = arg_transposed_transa ? arg_transposed_transa : transpose(blas.floatType, Builder2, arg_transa, byRef, cublas, charType, allocationBuilder, "transa"))}) args1.push_back(item);
        for (auto item : {arg_diag}) args1.push_back(item);
        for (auto item : {arg_m}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_alpha}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, ({    auto V = arg_side;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.side");
    Value *res = ConstantInt::get(charType, 'n');
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'L')), ConstantInt::get(res->getType(), 'N'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'R')), ConstantInt::get(res->getType(), 'T'), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'r')), ConstantInt::get(res->getType(), 't'), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "side_to_trans.side") }; vs; }), arg_lda, arg_n, arg_m, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        if (byRef) {
    auto tmpF_trmm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trmm" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trmm && tmpF_trmm->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmm->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrmm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trmm = blas.prefix + blas.floatType + "trmm" + blas.suffix;
    auto derivcall_trmm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trmm), FTtrmm);
    if (auto F = dyn_cast<Function>(derivcall_trmm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trmm);
      auto newF = attribute_trmm(blas, F);
      derivcall_trmm = FunctionCallee(derivcall_trmm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trmm, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
    },
    d_A, d_B  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_trmv(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_trans = 1 + offset;
  const auto orig_trans = call.getArgOperand(pos_trans);
  auto arg_trans = gutils->getNewFromOriginal(orig_trans);
  const auto type_trans = arg_trans->getType();
  const bool overwritten_trans = (cacheMode ? overwritten_args[pos_trans] : false);

  const int pos_diag = 2 + offset;
  const auto orig_diag = call.getArgOperand(pos_diag);
  auto arg_diag = gutils->getNewFromOriginal(orig_diag);
  const auto type_diag = arg_diag->getType();
  const bool overwritten_diag = (cacheMode ? overwritten_args[pos_diag] : false);

  const int pos_n = 3 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_A = 4 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 5 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_x = 6 + offset;
  const auto orig_x = call.getArgOperand(pos_x);
  auto arg_x = gutils->getNewFromOriginal(orig_x);
  const auto type_x = arg_x->getType();
  const bool overwritten_x = (cacheMode ? overwritten_args[pos_x] : false);
  bool active_x = !gutils->isConstantValue(orig_x);
  Value *rt_inactive_x = nullptr;

  const int pos_incx = 7 + offset;
  const auto orig_incx = call.getArgOperand(pos_incx);
  auto arg_incx = gutils->getNewFromOriginal(orig_incx);
  const auto type_incx = arg_incx->getType();
  const bool overwritten_incx = (cacheMode ? overwritten_args[pos_incx] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      auto shadow_x = gutils->invertPointerM(orig_x, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_x = BuilderZ.CreateICmpEQ(shadow_x, arg_x, "rt.tmp.inactive." "x");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_x_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_x, i), arg_x, "rt.tmp.inactive." "x." + std::to_string(i));
          if (i == 0) rt_inactive_x = rt_inactive_x_tmp;
          else rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_x_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    Value *rt_inactive_out = nullptr;
    if (active_x) {
      rt_inactive_out = rt_inactive_x;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_x) {
      rt_inactive_x = BuilderZ.CreateOr(rt_inactive_x, rt_inactive_out, "rt.inactive." "x");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_x) : rt_inactive_x;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trmv" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_A || active_x;
  bool need_trans = active_A || active_x;
  bool need_diag = active_A || active_x;
  bool need_n = active_A || active_x;
  bool need_A = active_x;
  bool need_lda = active_A || active_x;
  bool need_x = false;
  bool need_incx = active_A || active_x;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_trans = cacheMode && byRef && overwritten_trans && need_trans;
  bool cache_diag = cacheMode && byRef && overwritten_diag && need_diag;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  // we cache the following matrix,
  // since one rule uses input<x>
  if (active_A) {
    need_x = true;
    cache_x = true;
  }
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_trans)
    cacheTypes.push_back(charType);
  if (cache_diag)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_incx)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_x)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
    if (byRef) {
        addValueToCache(arg_trans, cache_trans, charType, cacheValues, BuilderZ, "trans");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_incx, cache_incx, intType, cacheValues, BuilderZ, "incx");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[5] = ValueType::Primal;
      if (byRef) valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = arg_uplo;
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_x) {
      Value *malloc_size;
      // arg_malloc_size will keep the original type
      Value *arg_malloc_size;
      malloc_size = arg_n;
      arg_malloc_size = malloc_size;
      malloc_size = load_if_ref(BuilderZ, intType, malloc_size, byRef);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, malloc_size, "cache.x", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      ValueType valueTypes[] = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
      valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (cublas) {
          Value *args[6] = {arg_handle, arg_malloc_size, arg_x, arg_incx, malins, ConstantInt::get(intType, 1)};
          callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, cublas_retty, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
        } else if (EnzymeBlasCopy) {
        Value *args[5] = {arg_malloc_size, arg_x, arg_incx, malins, to_blas_callconv(BuilderZ, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder)};
        callMemcpyStridedBlas(BuilderZ, *gutils->oldFunc->getParent(), blas, args, Type::getVoidTy(call.getContext()), gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
       auto dmemcpy = getOrInsertMemcpyStrided(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *inc = load_if_ref(BuilderZ, intType, arg_incx, byRef);
        Value *args[4] = {malins, arg_x, malloc_size, inc};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_incx = arg_incx;
  Value *free_x = nullptr;
  Value *input_x = nullptr;
  Value *free_input_x = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_trans) {
        arg_trans = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.trans");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.trans");
        Builder2.CreateStore(arg_trans, alloc);
        arg_trans = Builder2.CreatePointerCast(
            alloc, type_trans, "cast.trans");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_incx) {
        arg_incx = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.incx");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.incx");
        Builder2.CreateStore(arg_incx, alloc);
        arg_incx = Builder2.CreatePointerCast(
            alloc, type_incx, "cast.incx");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (active_A) {
      input_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_input_x = input_x;
      if (type_x->isIntegerTy()) {
        input_x = Builder2.CreatePtrToInt(input_x, type_x);
      } else if (input_x->getType() != type_x){
        input_x = Builder2.CreatePointerCast(input_x, type_x);
      }
    }
    if (cache_x) {
      arg_x = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.x");
      free_x = arg_x;
      if (type_x->isIntegerTy()) {
        arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
      } else if (arg_x->getType() != type_x){
        arg_x = Builder2.CreatePointerCast(arg_x, type_x);
      }
      cacheidx++;
    }
  } else {

    if (type_x->isIntegerTy())
      arg_x = Builder2.CreatePtrToInt(arg_x, type_x);
  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_x = active_x
     ? gutils->invertPointerM(orig_x, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_A, Value *d_x  ) {
      Value *dres = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument  within trmv of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_A, d_x);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trmv" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_x = active_x
     ? lookup(gutils->invertPointerM(orig_x, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_trans && need_trans)
      arg_trans = lookup(arg_trans, Builder2);
    if (!cache_diag && need_diag)
      arg_diag = lookup(arg_diag, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_x && need_x)
      arg_x = lookup(arg_x, Builder2);
    if (!cache_incx && need_incx)
      arg_incx = lookup(arg_incx, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_x) {
      rt_inactive_x = lookup(rt_inactive_x, Builder2);
    }
  }
    llvm::Value* arg_transposed_trans = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_x) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_A && d_x && d_A) {
        Value *toadd = nullptr;
        {
      // For
      auto lim_ar = ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; });
      Value *lim = (*lim_ar.begin());
      lim = load_if_ref(Builder2, intType, lim, byRef);
      BasicBlock *current = Builder2.GetInsertBlock();
      auto loopBlock = gutils->addReverseBlock(current,current->getName() + "_loop");
      auto endBlock = gutils->addReverseBlock(loopBlock,current->getName() + "_end", /*fork*/true, /*push*/false);
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, ConstantInt::get(lim->getType(), 0)), endBlock, loopBlock);
      Builder2.SetInsertPoint(loopBlock);
      auto phi_i = Builder2.CreatePHI(lim->getType(), 2);
      phi_i->addIncoming(ConstantInt::get(lim->getType(), 0), current);
      auto phi_i_inc = Builder2.CreateAdd(phi_i, ConstantInt::get(lim->getType(), 1), "", true, true);
      auto phi_b_i = to_blas_callconv(Builder2, phi_i_inc, byRef, cublas, julia_decl_type, allocationBuilder, "for.i");
      Value *for_res = nullptr;
        {
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall axpy
        std::vector<Value *>A_0;
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {phi_b_i} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto brow_2 = {d_x, arg_incx}; auto brow_1 = {input_x, (cache_x ? const_one : arg_incx)}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) larg_2.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  if (!byRefFloat) ptr = Builder2.CreateLoad(fpType, ptr);
  SmallVector<Value*, 1> vals = { ptr };
vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : ({auto brow_2 = {input_x, (cache_x ? const_one : arg_incx)}; auto brow_1 = {d_x, arg_incx}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);}) ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = nullptr;
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; })) A_0.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : ({SmallVector<Value*, 1> larg_0;
 for (auto tmp : {arg_layout} ) larg_0.push_back(tmp);
SmallVector<Value*, 1> larg_1;
 for (auto tmp : {d_A, arg_lda} ) larg_1.push_back(tmp);
SmallVector<Value*, 1> larg_2;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_lower(Builder2, arg_uplo, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isleft")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : ({    auto V = arg_diag;
    if (byRef) V = Builder2.CreateLoad(charType, V, "ld.diag");
    Value *res = ConstantInt::get(intType, 1);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'U')), ConstantInt::get(res->getType(), 1), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'N')), ConstantInt::get(res->getType(), 0), res);
    res = CreateSelect(Builder2, Builder2.CreateICmpEQ(V, ConstantInt::get(V->getType(), 'n')), ConstantInt::get(res->getType(), 0), res);
SmallVector<Value *, 1>vs = { to_blas_callconv(Builder2, res, byRef, cublas, julia_decl_type, allocationBuilder, "is_diag_int.diag") }; vs; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; }) ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; }) ) larg_2.push_back(tmp);
SmallVector<Value*, 1> larg_3;
 for (auto tmp : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {phi_b_i} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateSub(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Sub" ));
 }
 vals; }) ) larg_3.push_back(tmp);
 Value *ptr = larg_1[0];
 Value *ld_lookup = load_if_ref(Builder2, intType, larg_1[1], byRef);
 Value *layoutptr = cblas ? load_if_ref(Builder2, charType, larg_0[0], byRef) : nullptr;
 Value *row = load_if_ref(Builder2, intType, larg_2[0], byRef);
 Value *col = load_if_ref(Builder2, intType, larg_3[0], byRef);
 ptr = lookup_with_layout(Builder2, fpType, layoutptr, ptr, ld_lookup, row, col);
  SmallVector<Value*, 1> vals = { ptr, larg_1[1] };
vals; }) ) { sarg.push_back(tmp); break; }
 sarg; })) A_0.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")}) A_0.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, cache_x ? ValueType::Both : ValueType::Both}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTaxpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_axpy = blas.prefix + blas.floatType + "axpy" + blas.suffix;
    auto derivcall_axpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_axpy), FTaxpy);
    if (auto F = dyn_cast<Function>(derivcall_axpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_axpy);
      auto newF = attribute_axpy(blas, F);
      derivcall_axpy = FunctionCallee(derivcall_axpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_axpy, A_0, Defs));
        if (nextBlock_A) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      phi_i->addIncoming(phi_i_inc, Builder2.GetInsertBlock());
      Builder2.CreateCondBr(Builder2.CreateICmpEQ(lim, phi_i_inc), endBlock, loopBlock);
      Builder2.SetInsertPoint(endBlock);
      {
        auto found = gutils->reverseBlockToPrimal.find(endBlock);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(endBlock);
      }
        }
      }
      if (active_x && d_x) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_x = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".x.active");
          nextBlock_x = gutils->addReverseBlock(activeBlock, bb_name + ".x.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_x, nextBlock_x, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall trmv
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {(arg_transposed_trans = arg_transposed_trans ? arg_transposed_trans : transpose(blas.floatType, Builder2, arg_trans, byRef, cublas, charType, allocationBuilder, "trans"))}) args1.push_back(item);
        for (auto item : {arg_diag}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_x, arg_incx}) args1.push_back(item);
        if (byRef) {
    auto tmpF_trmv = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trmv" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_trmv && tmpF_trmv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmv->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trmv && tmpF_trmv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmv->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trmv && tmpF_trmv->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trmv->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrmv = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trmv = blas.prefix + blas.floatType + "trmv" + blas.suffix;
    auto derivcall_trmv = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trmv), FTtrmv);
    if (auto F = dyn_cast<Function>(derivcall_trmv.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trmv);
      auto newF = attribute_trmv(blas, F);
      derivcall_trmv = FunctionCallee(derivcall_trmv.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trmv, args1, Defs));
        if (nextBlock_x) {
          Builder2.CreateBr(nextBlock_x);
          Builder2.SetInsertPoint(nextBlock_x);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_x);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_x);
      }
        }
        }
      }
    },
    d_A, d_x  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_x) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_x);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}


bool handle_trtrs(BlasInfo blas, llvm::CallInst &call, llvm::Function *called,
    const std::vector<bool> &overwritten_args, llvm::Type *fpType) {
  
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif
  using namespace llvm;
  CallInst *const newCall = cast<CallInst>(gutils->getNewFromOriginal(&call));
  IRBuilder<> BuilderZ(newCall);
  BuilderZ.setFastMathFlags(getFast());
  IRBuilder<> allocationBuilder(gutils->inversionAllocs);
  allocationBuilder.setFastMathFlags(getFast());
  // never cache in Fwd Mode
  const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
  Value *cacheval = nullptr;

  const int offset = ((cblas || cublas) ? 1 : 0);
// Next ones shall only be called in the cublas case,
// they have incorrect meaning otherwise
  const int pos_handle = 0;
  Value *orig_handle = nullptr;
  Value *arg_handle = nullptr;
  Type *type_handle = nullptr;
  bool overwritten_handle = true;
  if (cublas) {
    orig_handle = call.getArgOperand(pos_handle);
    arg_handle = gutils->getNewFromOriginal(orig_handle);
    type_handle = arg_handle->getType();
    overwritten_handle = (cacheMode ? overwritten_args[pos_handle] : false);

  }

// Next ones shall only be called in the cblas case,
// they have incorrect meaning otherwise
  const int pos_layout = 0;
  Value *const orig_layout = cblas ? call.getArgOperand(pos_layout) : nullptr;
  Value * arg_layout = cblas ? gutils->getNewFromOriginal(orig_layout) : nullptr;
  const auto type_layout = cblas ? arg_layout->getType() : nullptr;
  const bool overwritten_layout = ((cacheMode && cblas) ? overwritten_args[pos_layout] : false);

  const int pos_uplo = 0 + offset;
  const auto orig_uplo = call.getArgOperand(pos_uplo);
  auto arg_uplo = gutils->getNewFromOriginal(orig_uplo);
  const auto type_uplo = arg_uplo->getType();
  const bool overwritten_uplo = (cacheMode ? overwritten_args[pos_uplo] : false);

  const int pos_trans = 1 + offset;
  const auto orig_trans = call.getArgOperand(pos_trans);
  auto arg_trans = gutils->getNewFromOriginal(orig_trans);
  const auto type_trans = arg_trans->getType();
  const bool overwritten_trans = (cacheMode ? overwritten_args[pos_trans] : false);

  const int pos_diag = 2 + offset;
  const auto orig_diag = call.getArgOperand(pos_diag);
  auto arg_diag = gutils->getNewFromOriginal(orig_diag);
  const auto type_diag = arg_diag->getType();
  const bool overwritten_diag = (cacheMode ? overwritten_args[pos_diag] : false);

  const int pos_n = 3 + offset;
  const auto orig_n = call.getArgOperand(pos_n);
  auto arg_n = gutils->getNewFromOriginal(orig_n);
  const auto type_n = arg_n->getType();
  const bool overwritten_n = (cacheMode ? overwritten_args[pos_n] : false);

  const int pos_nrhs = 4 + offset;
  const auto orig_nrhs = call.getArgOperand(pos_nrhs);
  auto arg_nrhs = gutils->getNewFromOriginal(orig_nrhs);
  const auto type_nrhs = arg_nrhs->getType();
  const bool overwritten_nrhs = (cacheMode ? overwritten_args[pos_nrhs] : false);

  const int pos_A = 5 + offset;
  const auto orig_A = call.getArgOperand(pos_A);
  auto arg_A = gutils->getNewFromOriginal(orig_A);
  const auto type_A = arg_A->getType();
  const bool overwritten_A = (cacheMode ? overwritten_args[pos_A] : false);
  bool active_A = !gutils->isConstantValue(orig_A);
  Value *rt_inactive_A = nullptr;

  const int pos_lda = 6 + offset;
  const auto orig_lda = call.getArgOperand(pos_lda);
  auto arg_lda = gutils->getNewFromOriginal(orig_lda);
  const auto type_lda = arg_lda->getType();
  const bool overwritten_lda = (cacheMode ? overwritten_args[pos_lda] : false);

  const int pos_B = 7 + offset;
  const auto orig_B = call.getArgOperand(pos_B);
  auto arg_B = gutils->getNewFromOriginal(orig_B);
  const auto type_B = arg_B->getType();
  const bool overwritten_B = (cacheMode ? overwritten_args[pos_B] : false);
  bool active_B = !gutils->isConstantValue(orig_B);
  Value *rt_inactive_B = nullptr;

  const int pos_ldb = 8 + offset;
  const auto orig_ldb = call.getArgOperand(pos_ldb);
  auto arg_ldb = gutils->getNewFromOriginal(orig_ldb);
  const auto type_ldb = arg_ldb->getType();
  const bool overwritten_ldb = (cacheMode ? overwritten_args[pos_ldb] : false);

  const int pos_info = 9 + offset;
  const auto orig_info = call.getArgOperand(pos_info);
  auto arg_info = gutils->getNewFromOriginal(orig_info);
  const auto type_info = arg_info->getType();
  const bool overwritten_info = (cacheMode ? overwritten_args[pos_info] : false);


  // <X> is inactive either if gutils->isConstantValue(<X>)
  // returns true, or if runtimeActivity is on and the
  // shadow points to the primal arg.
  if(gutils->runtimeActivity) {
    Value *anyRuntimeActivity = nullptr;
    if (active_A) {
      auto shadow_A = gutils->invertPointerM(orig_A, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_A = BuilderZ.CreateICmpEQ(shadow_A, arg_A, "rt.tmp.inactive." "A");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_A_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_A, i), arg_A, "rt.tmp.inactive." "A." + std::to_string(i));
          if (i == 0) rt_inactive_A = rt_inactive_A_tmp;
          else rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_A_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      auto shadow_B = gutils->invertPointerM(orig_B, BuilderZ);
      if (gutils->getWidth() == 1)
        rt_inactive_B = BuilderZ.CreateICmpEQ(shadow_B, arg_B, "rt.tmp.inactive." "B");
      else {
        for (size_t i=0; i<gutils->getWidth(); i++) {
          auto rt_inactive_B_tmp = BuilderZ.CreateICmpEQ(gutils->extractMeta(BuilderZ, shadow_B, i), arg_B, "rt.tmp.inactive." "B." + std::to_string(i));
          if (i == 0) rt_inactive_B = rt_inactive_B_tmp;
          else rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_B_tmp);
        }
      }
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    Value *rt_inactive_out = nullptr;
    if (active_B) {
      rt_inactive_out = rt_inactive_B;
    } else {
      rt_inactive_out = ConstantInt::getTrue(BuilderZ.getContext());
    }
    if (active_A) {
      rt_inactive_A = BuilderZ.CreateOr(rt_inactive_A, rt_inactive_out, "rt.inactive." "A");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_A) : rt_inactive_A;
    }
    if (active_B) {
      rt_inactive_B = BuilderZ.CreateOr(rt_inactive_B, rt_inactive_out, "rt.inactive." "B");
      if (Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) anyRuntimeActivity = anyRuntimeActivity ? BuilderZ.CreateOr(anyRuntimeActivity, rt_inactive_B) : rt_inactive_B;
    }
    if ((Mode == DerivativeMode::ForwardMode || Mode == DerivativeMode::ForwardModeSplit) && anyRuntimeActivity) {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trtrs" << "\n";
      ss << "Runtime Activity not yet implemented for Forward-Mode BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, BuilderZ, anyRuntimeActivity);
    }
  }
  Type* blasFPType = byRefFloat ? (Type*)getUnqual(fpType) : (Type*)fpType;
  Type* blasCharType = type_uplo;
  Type *cublasEnumType = nullptr;
  if (cublas) cublasEnumType = type_uplo;
  Type* blasIntType = type_n;
  Type* cublas_retty = nullptr;
  Value* cublas_handle = nullptr;
  if (cublas) {
    cublas_retty = call.getFunctionType()->getReturnType();
    cublas_handle = call.getArgOperand(0);
  }
  const bool julia_decl = !type_A->isPointerTy();
  Type* type_vec_like = type_A;
  // fpType already given by blas type (s, d, c, z) 
  IntegerType *intType = blas.intType(call.getContext());
  IntegerType *charType = IntegerType::get(intType->getContext(), 8);

  IntegerType *julia_decl_type = nullptr;
  if (julia_decl)
    julia_decl_type = intType;
  Value *valueN = nullptr;
  Value *valueT = nullptr;
  Value *valueC = nullptr;
  Value *valueG = nullptr;
  Value *valuer = nullptr;
  Value *valuel = nullptr;
  Value *valueR = nullptr;
  Value *valueL = nullptr;
  Value *valueU = nullptr;
  if (cublas) {
    valueN = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_N);
    valueT = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_T);
    valueC = ConstantInt::get(cublasEnumType, cublasOperation_t::CUBLAS_OP_C);
    valuel = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valuer = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueL = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_LEFT);
    valueR = ConstantInt::get(cublasEnumType, cublasSideMode_t::CUBLAS_SIDE_RIGHT);
    valueU = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_UPPER);
    valueG = ConstantInt::get(cublasEnumType, cublasFillMode_t::CUBLAS_FILL_MODE_FULL);
  } else {
    valueN = ConstantInt::get(charType, 'N');
    valueT = ConstantInt::get(charType, 'T');
    valueC = ConstantInt::get(charType, 'C');
    valueG = ConstantInt::get(charType, 'G');
    valuer = ConstantInt::get(charType, 'r');
    valuel = ConstantInt::get(charType, 'l');
    valueR = ConstantInt::get(charType, 'R');
    valueL = ConstantInt::get(charType, 'L');
    valueU = ConstantInt::get(charType, 'U');
  }

  SmallVector<Type *, 2> cacheTypes;

  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = active_A || active_B;
  bool need_trans = active_A || active_B;
  bool need_diag = active_A || active_B;
  bool need_n = active_A || active_B;
  bool need_nrhs = active_A || active_B;
  bool need_A = active_B;
  bool need_lda = active_A || active_B;
  bool need_B = active_A;
  bool need_ldb = active_A || active_B;
  bool need_info = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_trans = cacheMode && byRef && overwritten_trans && need_trans;
  bool cache_diag = cacheMode && byRef && overwritten_diag && need_diag;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_nrhs = cacheMode && byRef && overwritten_nrhs && need_nrhs;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  bool cache_B = cacheMode && overwritten_B && need_B;
  bool cache_ldb = cacheMode && byRef && overwritten_ldb && need_ldb;
  bool cache_info = cacheMode && byRef && overwritten_info && need_info;
  if (cache_uplo)
    cacheTypes.push_back(charType);
  if (cache_trans)
    cacheTypes.push_back(charType);
  if (cache_diag)
    cacheTypes.push_back(charType);
  if (cache_n)
    cacheTypes.push_back(intType);
  if (cache_nrhs)
    cacheTypes.push_back(intType);
  if (cache_lda)
    cacheTypes.push_back(intType);
  if (cache_ldb)
    cacheTypes.push_back(intType);
  if (cache_A)
    cacheTypes.push_back(getUnqual(fpType));
  if (cache_B)
    cacheTypes.push_back(getUnqual(fpType));
  Type *cachetype = nullptr;
  switch (cacheTypes.size()) {
  case 0:
    break;
  case 1:
    cachetype = cacheTypes[0];
    break;
  default:
    cachetype = StructType::get(call.getContext(), cacheTypes);
    break;
  }

  if ((Mode == DerivativeMode::ReverseModeCombined ||
       Mode == DerivativeMode::ReverseModePrimal) && cachetype) {
    SmallVector<Value *, 2> cacheValues;
BuilderZ.SetInsertPoint(gutils->getNewFromOriginal(&call)->getNextNode());
    if (byRef) {
        addValueToCache(arg_trans, cache_trans, charType, cacheValues, BuilderZ, "trans");
        addValueToCache(arg_n, cache_n, intType, cacheValues, BuilderZ, "n");
        addValueToCache(arg_nrhs, cache_nrhs, intType, cacheValues, BuilderZ, "nrhs");
        addValueToCache(arg_lda, cache_lda, intType, cacheValues, BuilderZ, "lda");
        addValueToCache(arg_ldb, cache_ldb, intType, cacheValues, BuilderZ, "ldb");
    }
    if (cache_A) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_n;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.A", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[6] = ValueType::Primal;
      if (byRef) valueTypes[7] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = arg_uplo;
        SmallVector<Value *, 7> args = {uplo, M, N, arg_A, arg_lda, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_lda, byRef);
        Value *args[5] = {malins, arg_A, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cache_B) {
      auto charTy = IntegerType::get(intType->getContext(), 8);
      Value *M, *N;
      M = arg_n;
      N = arg_nrhs;
      auto *len1 = load_if_ref(BuilderZ, intType, M, byRef);
      auto *len2 = load_if_ref(BuilderZ, intType, N, byRef);
      auto *matSize = BuilderZ.CreateMul(len1, len2);
      Instruction *SubZero = nullptr;
      CallInst *malloccall = nullptr;
      auto malins = CreateAllocation(BuilderZ, fpType, matSize, "cache.B", &malloccall);
      if (malloccall) {
        auto ident = MDNode::getDistinct(malloccall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(malloccall->getContext()))});
        malloccall->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall->getContext(), {ident}));
      }
      SmallVector<ValueType, 7> valueTypes = {ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None, ValueType::None};
       valueTypes[8] = ValueType::Primal;
      if (byRef) valueTypes[9] = ValueType::Primal;
      if (byRef) valueTypes[4] = ValueType::Primal;
      if (byRef) valueTypes[5] = ValueType::Primal;
      if (EnzymeLapackCopy) {
        Value *uplo = llvm::ConstantInt::get(charTy, 0);
        uplo = to_blas_callconv(BuilderZ, uplo, byRef, cublas, nullptr, allocationBuilder, "copy.garbage");
        SmallVector<Value *, 7> args = {uplo, M, N, arg_B, arg_ldb, malins, M};
        if (!byRef) {
           args.insert(args.begin(), arg_layout); valueTypes.insert(valueTypes.begin(), ValueType::Primal); }
        else
           args.push_back(ConstantInt::get(intType, 1));
        callMemcpyStridedLapack(BuilderZ, *gutils->oldFunc->getParent(), blas, args, gutils->getInvertedBundles(&call, valueTypes, BuilderZ, /*lookup*/false));
      } else {
        auto dmemcpy = getOrInsertMemcpyMat(*gutils->oldFunc->getParent(), fpType, cast<PointerType>(malins->getType()), intType, 0, 0);
        Value *len_lda = load_if_ref(BuilderZ, intType, arg_ldb, byRef);
        Value *args[5] = {malins, arg_B, len1, len2, len_lda};
        if (args[1]->getType()->isIntegerTy())
          args[1] = BuilderZ.CreateIntToPtr(args[1], malins->getType());
        else if (args[1]->getType() != malins->getType())
          args[1] = BuilderZ.CreatePointerCast(args[1], malins->getType());
        BuilderZ.CreateCall(dmemcpy, args,
            gutils->getInvertedBundles(&call, valueTypes,
            BuilderZ, /*lookup*/ false));
      }
      cacheValues.push_back(malins);
    }
    if (cacheValues.size() == 1) {
      cacheval = cacheValues[0];
    } else {
      cacheval = UndefValue::get(cachetype);
      for (auto&& tup : llvm::enumerate(cacheValues))
        cacheval = BuilderZ.CreateInsertValue(cacheval, tup.value(), tup.index());
    }
    gutils->cacheForReverse(BuilderZ, cacheval,
                            getIndex(&call, CacheType::Tape, BuilderZ));
  }
  unsigned cacheidx = 0;
  Value *true_lda = arg_lda;
  Value *lda = true_lda;
  Value *free_A = nullptr;
  Value *true_ldb = arg_ldb;
  Value *ldb = true_ldb;
  Value *free_B = nullptr;
  IRBuilder<> Builder2(&call);
  switch (Mode) {
    case DerivativeMode::ReverseModeCombined:
    case DerivativeMode::ReverseModeGradient:
      getReverseBuilder(Builder2);
      break;
    case DerivativeMode::ForwardModeError:
      llvm_unreachable("blas forward error rules not enabled");
    case DerivativeMode::ForwardMode:
    case DerivativeMode::ForwardModeSplit:
      Builder2.SetInsertPoint(BuilderZ.GetInsertBlock(),
                              BuilderZ.GetInsertPoint());
      Builder2.setFastMathFlags(getFast());
      break;
    case DerivativeMode::ReverseModePrimal:
      break;
  }

  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {

    if (cachetype) {
      if (Mode != DerivativeMode::ReverseModeCombined) {
        cacheval = BuilderZ.CreatePHI(cachetype, 0);
      }
      cacheval = gutils->cacheForReverse(
          BuilderZ, cacheval, getIndex(&call, CacheType::Tape, BuilderZ));
      if (Mode != DerivativeMode::ForwardModeSplit)
        cacheval = lookup(cacheval, Builder2);
    }

    if (byRef) {
      if (cache_trans) {
        arg_trans = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.trans");
        auto alloc = allocationBuilder.CreateAlloca(charType, nullptr, "byref.trans");
        Builder2.CreateStore(arg_trans, alloc);
        arg_trans = Builder2.CreatePointerCast(
            alloc, type_trans, "cast.trans");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_n) {
        arg_n = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.n");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.n");
        Builder2.CreateStore(arg_n, alloc);
        arg_n = Builder2.CreatePointerCast(
            alloc, type_n, "cast.n");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_nrhs) {
        arg_nrhs = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.nrhs");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.nrhs");
        Builder2.CreateStore(arg_nrhs, alloc);
        arg_nrhs = Builder2.CreatePointerCast(
            alloc, type_nrhs, "cast.nrhs");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_lda) {
        arg_lda = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.lda");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.lda");
        Builder2.CreateStore(arg_lda, alloc);
        arg_lda = Builder2.CreatePointerCast(
            alloc, type_lda, "cast.lda");
        cacheidx++;
      }

    }
    if (byRef) {
      if (cache_ldb) {
        arg_ldb = (cacheTypes.size() == 1)
                    ? cacheval
                    : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.ldb");
        auto alloc = allocationBuilder.CreateAlloca(intType, nullptr, "byref.ldb");
        Builder2.CreateStore(arg_ldb, alloc);
        arg_ldb = Builder2.CreatePointerCast(
            alloc, type_ldb, "cast.ldb");
        cacheidx++;
      }

    }
    if (cache_A) {
      arg_A = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.A");
      free_A = arg_A;
      if (type_A->isIntegerTy()) {
        arg_A = Builder2.CreatePtrToInt(arg_A, type_A);
      } else if (arg_A->getType() != type_A){
        arg_A = Builder2.CreatePointerCast(arg_A, type_A);
      }
      cacheidx++;
    }
    if (cache_B) {
      arg_B = (cacheTypes.size() == 1)
                  ? cacheval
                  : Builder2.CreateExtractValue(cacheval, {cacheidx}, "tape.ext.B");
      free_B = arg_B;
      if (type_B->isIntegerTy()) {
        arg_B = Builder2.CreatePtrToInt(arg_B, type_B);
      } else if (arg_B->getType() != type_B){
        arg_B = Builder2.CreatePointerCast(arg_B, type_B);
      }
      cacheidx++;
    }
  } else {

  }
  /* fwd-rewrite */                                 
  if (Mode == DerivativeMode::ForwardMode ||        
      Mode == DerivativeMode::ForwardModeSplit) {   
                                                    
    auto callval = call.getCalledOperand();       

Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
    Value *d_A = active_A
     ? gutils->invertPointerM(orig_A, Builder2)
     : nullptr;
    Value *d_B = active_B
     ? gutils->invertPointerM(orig_B, Builder2)
     : nullptr;
    Value *dres = applyChainRule(
        call.getType(), Builder2,
        [&](Value *d_A, Value *d_B  ) {
      Value *dres = nullptr;
            std::string s;
            llvm::raw_string_ostream ss(s);
            ss << "in Mode: " << to_string(Mode) << "\n";
            ss << "cannot handle blas argument  within trtrs of " << call;
            EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
      if (!dres && !call.getType()->isVoidTy()) dres = Constant::getNullValue(call.getType());
      return dres;
    },
    d_A, d_B);
    if (!gutils->isConstantValue(&call))
      setDiffe(&call, dres, Builder2);
  }
  /* rev-rewrite */                                 
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient) {
    if (blas.floatType == "c" || blas.floatType == "C" || blas.floatType == "z" || blas.floatType == "Z") {
      std::string s;
      llvm::raw_string_ostream ss(s);
      ss << "trtrs" << "\n";
      ss << "Complex inputs not yet supported in reverse mode for BLAS calls" << "\n";
      EmitNoDerivativeError(ss.str(), call, gutils, Builder2);
    }
    Value *alloc = nullptr;
    if (byRef && !cublas) {
      alloc = allocationBuilder.CreateAlloca(fpType, nullptr, "ret");
    }

    Value *d_A = active_A
     ? lookup(gutils->invertPointerM(orig_A, Builder2), Builder2)
     : nullptr;
    Value *d_B = active_B
     ? lookup(gutils->invertPointerM(orig_B, Builder2), Builder2)
     : nullptr;
    if (!cache_uplo && need_uplo)
      arg_uplo = lookup(arg_uplo, Builder2);
    if (!cache_trans && need_trans)
      arg_trans = lookup(arg_trans, Builder2);
    if (!cache_diag && need_diag)
      arg_diag = lookup(arg_diag, Builder2);
    if (!cache_n && need_n)
      arg_n = lookup(arg_n, Builder2);
    if (!cache_nrhs && need_nrhs)
      arg_nrhs = lookup(arg_nrhs, Builder2);
    if (!cache_A && need_A)
      arg_A = lookup(arg_A, Builder2);
    if (!cache_lda && need_lda)
      arg_lda = lookup(arg_lda, Builder2);
    if (!cache_B && need_B)
      arg_B = lookup(arg_B, Builder2);
    if (!cache_ldb && need_ldb)
      arg_ldb = lookup(arg_ldb, Builder2);
    if (!cache_info && need_info)
      arg_info = lookup(arg_info, Builder2);
  if(gutils->runtimeActivity && cacheMode) {
    if (active_A) {
      rt_inactive_A = lookup(rt_inactive_A, Builder2);
    }
    if (active_B) {
      rt_inactive_B = lookup(rt_inactive_B, Builder2);
    }
  }
    llvm::Value* arg_transposed_trans = nullptr;
    applyChainRule(
      Builder2,
      [&](Value *d_A, Value *d_B) {
Value * const_one = to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "int.one");
      auto bb_name = Builder2.GetInsertBlock()->getName();
      if (active_B && d_B) {
        Value *toadd = nullptr;
        {
        BasicBlock *nextBlock_B = nullptr;
        if (gutils->runtimeActivity && cacheMode) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".B.active");
          nextBlock_B = gutils->addReverseBlock(activeBlock, bb_name + ".B.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_B, nextBlock_B, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
      // BlasCall trtrs
        std::vector<Value *>args1;
        if (cblas) args1.push_back(arg_layout);
        if (cublas) args1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) args1.push_back(item);
        for (auto item : {(arg_transposed_trans = arg_transposed_trans ? arg_transposed_trans : transpose(blas.floatType, Builder2, arg_trans, byRef, cublas, charType, allocationBuilder, "trans"))}) args1.push_back(item);
        for (auto item : {arg_diag}) args1.push_back(item);
        for (auto item : {arg_n}) args1.push_back(item);
        for (auto item : {arg_nrhs}) args1.push_back(item);
        for (auto item : {arg_A}) args1.push_back(item);
        for (auto item : {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_lda, arg_n, arg_n, cache_A, byRef, cublas)}) args1.push_back(item);
        for (auto item : {d_B, arg_ldb}) args1.push_back(item);
        for (auto item : {allocationBuilder.CreateAlloca(intType)}) args1.push_back(item);
        if (byRef) {
    auto tmpF_trtrs = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "trtrs" + blas.suffix));
           args1.push_back(ConstantInt::get((tmpF_trtrs && tmpF_trtrs->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trtrs->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trtrs && tmpF_trtrs->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trtrs->getFunctionType()->getParamType(args1.size()) : intType, 1));
           args1.push_back(ConstantInt::get((tmpF_trtrs && tmpF_trtrs->getFunctionType()->getNumParams() > args1.size() ) ? tmpF_trtrs->getFunctionType()->getParamType(args1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : args1) tys.push_back(arg->getType());
    llvm::FunctionType *FTtrtrs = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_trtrs = blas.prefix + blas.floatType + "trtrs" + blas.suffix;
    auto derivcall_trtrs = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_trtrs), FTtrtrs);
    if (auto F = dyn_cast<Function>(derivcall_trtrs.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_trtrs);
      auto newF = attribute_trtrs(blas, F);
      derivcall_trtrs = FunctionCallee(derivcall_trtrs.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_trtrs, args1, Defs));
        if (nextBlock_B) {
          Builder2.CreateBr(nextBlock_B);
          Builder2.SetInsertPoint(nextBlock_B);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_B);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_B);
      }
        }
        }
      }
      if (active_A && d_B && d_A) {
        Value *toadd = nullptr;
        {
      // Seq
        BasicBlock *nextBlock_A = nullptr;
        if (gutils->runtimeActivity && cacheMode && byRefFloat) {
          BasicBlock *current = Builder2.GetInsertBlock();
          auto activeBlock = gutils->addReverseBlock(current,bb_name + ".A.active");
          nextBlock_A = gutils->addReverseBlock(activeBlock, bb_name + ".A.done", /*fork*/true, /*push*/false);
          Builder2.CreateCondBr(rt_inactive_A, nextBlock_A, activeBlock);
          Builder2.SetInsertPoint(activeBlock);
        }
    Value *len = load_if_ref(Builder2, intType,arg_n, byRef);
    Value *size_tri = Builder2.CreateMul(len, len);
    CallInst * malloccall_mat_tri = nullptr;
    Value * true_mat_tri = CreateAllocation(Builder2, fpType, size_tri, "mat_tri", &malloccall_mat_tri);
    MDNode *ident_mat_tri = nullptr;
    if (malloccall_mat_tri) {
      ident_mat_tri = MDNode::getDistinct(malloccall_mat_tri->getContext(), {ConstantAsMetadata::get(ConstantInt::getTrue(malloccall_mat_tri->getContext()))});
      malloccall_mat_tri->setMetadata("enzyme_cache_alloc", MDNode::get(malloccall_mat_tri->getContext(), {ident_mat_tri}));
    }
    Value * mat_tri = true_mat_tri;
    if (type_vec_like->isIntegerTy()) {
      mat_tri = Builder2.CreatePtrToInt(mat_tri, type_vec_like);
    } else if (mat_tri->getType() != type_vec_like){
      mat_tri = Builder2.CreatePointerCast(mat_tri, type_vec_like);
    }
        {
      // BlasCall lacpy
        std::vector<Value *>A_0;
        if (cblas) A_0.push_back(arg_layout);
        if (cublas) A_0.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        for (auto item : {d_A, arg_lda}) A_0.push_back(item);
        for (auto item : {mat_tri}) A_0.push_back(item);
        for (auto item : {arg_n}) A_0.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           A_0.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > A_0.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(A_0.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_0) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, A_0, Defs));
        }
        {
      // BlasCall gemm
        std::vector<Value *>A_1;
        if (cblas) A_1.push_back(arg_layout);
        if (cublas) A_1.push_back(lookup(arg_handle, Builder2));
        for (auto item : {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}) A_1.push_back(item);
        for (auto item : {to_blas_callconv(Builder2, (blas.floatType == "z" || blas.floatType == "c") ? valueC : valueT, byRef, cublas, nullptr, allocationBuilder, "constant.char.C")}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        for (auto item : {arg_nrhs}) A_1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, -1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.-1")}) A_1.push_back(item);
        for (auto item : ({auto brow_2 = ({auto concat_0 = {arg_B}; auto concat_1 = {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_ldb, arg_n, arg_n, cache_B, byRef, cublas)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_1 = {d_B, arg_ldb}; auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_1.push_back(item);
        for (auto item : ({auto brow_2 = {d_B, arg_ldb}; auto brow_1 = ({auto concat_0 = {arg_B}; auto concat_1 = {get_cached_mat_width(Builder2, {to_blas_callconv(Builder2, valueN, byRef, cublas, nullptr, allocationBuilder, "constant.char.N")}, arg_ldb, arg_n, arg_n, cache_B, byRef, cublas)}; concat_values<ArrayRef<Value*>, ArrayRef<Value*>>(concat_0, concat_1); }); auto brow_0 = {arg_trans}; get_blas_row(Builder2, brow_0, brow_1, brow_2, byRef, cublas);})) A_1.push_back(item);
        for (auto item : {to_blas_fp_callconv(Builder2, ConstantFP::get(fpType, 1), byRefFloat, blasFPType, allocationBuilder, "constant.fp.1")}) A_1.push_back(item);
        for (auto item : {mat_tri}) A_1.push_back(item);
        for (auto item : {arg_n}) A_1.push_back(item);
        if (byRef) {
    auto tmpF_gemm = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "gemm" + blas.suffix));
           A_1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
           A_1.push_back(ConstantInt::get((tmpF_gemm && tmpF_gemm->getFunctionType()->getNumParams() > A_1.size() ) ? tmpF_gemm->getFunctionType()->getParamType(A_1.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_1) tys.push_back(arg->getType());
    llvm::FunctionType *FTgemm = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_gemm = blas.prefix + blas.floatType + "gemm" + blas.suffix;
    auto derivcall_gemm = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_gemm), FTgemm);
    if (auto F = dyn_cast<Function>(derivcall_gemm.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_gemm);
      auto newF = attribute_gemm(blas, F);
      derivcall_gemm = FunctionCallee(derivcall_gemm.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_gemm, A_1, Defs));
        }
        {
      // BlasCall copy
        std::vector<Value *>A_2;
        if (cublas) A_2.push_back(lookup(arg_handle, Builder2));
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {to_blas_callconv(Builder2, is_nonunit(Builder2, arg_diag, byRef, cublas), byRef, cublas, julia_decl_type, allocationBuilder, "isnonunit")} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 0), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.0")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> marg_2;
 for (auto tmp : {arg_n} ) marg_2.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_2.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, Builder2.getInt1Ty(), marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = marg_1[marg_1.size() == 1 ? 0 : i];
  auto subarg_2 = Builder2.CreateBitCast(marg_2[marg_2.size() == 1 ? 0 : i], marg_1[marg_1.size() == 1 ? 0 : i]->getType());
  vals.push_back( CreateSelect(Builder2, subarg_0, subarg_1, subarg_2));
 }
 vals; })) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> sarg;
 for (auto tmp : {d_A, arg_lda} ) { sarg.push_back(tmp); break; }
 sarg; })) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_lda} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_2.push_back(item);
        for (auto item : {mat_tri}) A_2.push_back(item);
        for (auto item : ({SmallVector<Value*, 1> marg_0;
 for (auto tmp : {arg_n} ) marg_0.push_back(tmp);
SmallVector<Value*, 1> marg_1;
 for (auto tmp : {to_blas_callconv(Builder2, ConstantInt::get(intType, 1), byRef, cublas, julia_decl_type, allocationBuilder, "constant.int.1")} ) marg_1.push_back(tmp);
SmallVector<Value*, 1> vals;
for(size_t i=0; i<marg_1.size(); i++) {
  auto subarg_0 = load_if_ref(Builder2, intType, marg_0[marg_0.size() == 1 ? 0 : i], byRef);
  auto subarg_1 = load_if_ref(Builder2, intType, marg_1[marg_1.size() == 1 ? 0 : i], byRef);
  vals.push_back(to_blas_callconv(Builder2, Builder2.CreateAdd(subarg_0, subarg_1), byRef, cublas, julia_decl_type, allocationBuilder, "Add" ));
 }
 vals; })) A_2.push_back(item);
        if (byRef) {
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_2) tys.push_back(arg->getType());
    llvm::FunctionType *FTcopy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_copy = blas.prefix + blas.floatType + "copy" + (cublasv2 ? "" : blas.suffix);
    auto derivcall_copy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_copy), FTcopy);
    if (auto F = dyn_cast<Function>(derivcall_copy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_copy);
      auto newF = attribute_copy(blas, F);
      derivcall_copy = FunctionCallee(derivcall_copy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_copy, A_2, Defs));
        }
        {
      // BlasCall lacpy
        std::vector<Value *>A_3;
        if (cblas) A_3.push_back(arg_layout);
        if (cublas) A_3.push_back(lookup(arg_handle, Builder2));
        for (auto item : {arg_uplo}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {mat_tri}) A_3.push_back(item);
        for (auto item : {arg_n}) A_3.push_back(item);
        for (auto item : {d_A, arg_lda}) A_3.push_back(item);
        if (byRef) {
    auto tmpF_lacpy = gutils->oldFunc->getParent()->getFunction(
  getRenamedPerCallingConv(called->getName(), blas.prefix + blas.floatType + "lacpy" + blas.suffix));
           A_3.push_back(ConstantInt::get((tmpF_lacpy && tmpF_lacpy->getFunctionType()->getNumParams() > A_3.size() ) ? tmpF_lacpy->getFunctionType()->getParamType(A_3.size()) : intType, 1));
        }
        const auto Defs = gutils->getInvertedBundles(&call, {ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Primal}, Builder2, /* lookup */ 1);
    SmallVector<Type*, 1> tys; for (auto arg : A_3) tys.push_back(arg->getType());
    llvm::FunctionType *FTlacpy = FunctionType::get(cublasv2 ? Type::getVoidTy(fpType->getContext()) : Builder2.getVoidTy(), tys, false);
    auto str_lacpy = blas.prefix + blas.floatType + "lacpy" + blas.suffix;
    auto derivcall_lacpy = gutils->oldFunc->getParent()->getOrInsertFunction(
  getRenamedPerCallingConv(called->getName(), str_lacpy), FTlacpy);
    if (auto F = dyn_cast<Function>(derivcall_lacpy.getCallee()))
    {
      F->addFnAttr("enzyme_math", str_lacpy);
      auto newF = attribute_lacpy(blas, F);
      derivcall_lacpy = FunctionCallee(derivcall_lacpy.getFunctionType(), newF);
    }

    auto cubcall = cast<CallInst>(Builder2.CreateCall(derivcall_lacpy, A_3, Defs));
        }
    {
      CallInst *freecall = CreateDealloc(Builder2, true_mat_tri);
      if (freecall && ident_mat_tri) {
        freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident_mat_tri}));
      }
    }
        if (nextBlock_A && byRefFloat) {
          Builder2.CreateBr(nextBlock_A);
          Builder2.SetInsertPoint(nextBlock_A);
      {
        auto found = gutils->reverseBlockToPrimal.find(nextBlock_A);
        assert(found != gutils->reverseBlockToPrimal.end());
        SmallVector<BasicBlock *, 4> &vec =
          gutils->reverseBlocks[found->second];
        assert(vec.size());
        vec.push_back(nextBlock_A);
      }
        }
        }
      }
    },
    d_A, d_B  );
  }
  if (Mode == DerivativeMode::ReverseModeCombined ||
      Mode == DerivativeMode::ReverseModeGradient ||
      Mode == DerivativeMode::ForwardModeSplit) {
    if (shouldFree()) {
      if (cache_A) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_A);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
      if (cache_B) {
        {
          CallInst *freecall = CreateDealloc(Builder2, free_B);
          if (freecall) {
            auto ident = MDNode::getDistinct(freecall->getContext(), {ConstantAsMetadata::get(ConstantInt::getFalse(freecall->getContext()))});
            freecall->setMetadata("enzyme_cache_free", MDNode::get(freecall->getContext(), {ident}));
          }
        }
      }
    }
  }
                                                                   
  if (cublas && Mode == DerivativeMode::ReverseModeGradient && call.getType()->isIntegerTy()) {        
     gutils->replaceAWithB(gutils->getNewFromOriginal(&call), Constant::getNullValue(call.getType()));
  }
  bool shouldErase = true;
  if (gutils->knownRecomputeHeuristic.find(&call) !=
    gutils->knownRecomputeHeuristic.end()) {
    if (!gutils->knownRecomputeHeuristic[&call]) {
     auto cv = gutils->cacheForReverse(BuilderZ, newCall,
     getIndex(&call, CacheType::Self, BuilderZ));
     shouldErase = false;
    }
  }
  if (shouldErase) {
    if (Mode == DerivativeMode::ReverseModeGradient) {        
      eraseIfUnused(call, /*erase*/ true, /*check*/ false);        
    } else {                                                       
      eraseIfUnused(call);                                         
    }                                                              
  }
  return true;
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
}

