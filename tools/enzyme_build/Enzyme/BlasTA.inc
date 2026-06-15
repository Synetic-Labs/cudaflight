/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
const bool cblas = blas.prefix == "cblas_";
const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
const bool cublasv2 = blas.prefix == "cublas" && StringRef(blas.suffix).contains("v2");
TypeTree ttFloat;
llvm::Type *floatType = blas.fpType(call.getContext(), true); 
if (byRefFloat) {
  ttFloat.insert({-1},BaseType::Pointer);
  ttFloat.insert({-1,0},floatType);
} else { 
  ttFloat.insert({-1},floatType);
}
TypeTree ttFloatRet;
ttFloatRet.insert({-1},floatType);
TypeTree ttCuBlasRet;
ttCuBlasRet.insert({-1},BaseType::Integer);
TypeTree ttPtrInt;
ttPtrInt.insert({-1},BaseType::Pointer);
ttPtrInt.insert({-1, -1},BaseType::Integer);
TypeTree ttInt;
if (byRef) {
  ttInt.insert({-1},BaseType::Pointer);
  ttInt.insert({-1,0},BaseType::Integer);
  ttInt.insert({-1,1},BaseType::Integer);
  ttInt.insert({-1,2},BaseType::Integer);
  ttInt.insert({-1,3},BaseType::Integer);
  if (blas.suffix == "_64_" || blas.suffix == "64_") {
    ttInt.insert({-1,4},BaseType::Integer);
    ttInt.insert({-1,5},BaseType::Integer);
    ttInt.insert({-1,6},BaseType::Integer);
    ttInt.insert({-1,7},BaseType::Integer);
  }
} else {
  ttInt.insert({-1},BaseType::Integer);
}
TypeTree ttChar;
if (byRef) {
  ttChar.insert({-1},BaseType::Pointer);
  ttChar.insert({-1,0},BaseType::Integer);
} else {
  ttChar.insert({-1},BaseType::Integer);
}
TypeTree ttCuHandle;
ttCuHandle.insert({-1},BaseType::Pointer);
TypeTree ttPtr;
ttPtr.insert({-1},BaseType::Pointer);
ttPtr.insert({-1,0},floatType);
if (blas.function == "asum") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(1 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(1 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  if (cublasv2) {
    updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
    updateAnalysis(&call, ttCuBlasRet, &call);
  } else {
    assert(call.getType()->isFloatingPointTy());
    updateAnalysis(&call, ttFloatRet, &call);
  }
}
if (blas.function == "axpy") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(1 + offset), ttFloat, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(2 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(2 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(2 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(2 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // vincData y
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(4 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(4 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(4 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(4 + offset), ttPtr, &call);
  }
  // vincInc incy
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "copy") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(1 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(1 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // vincData y
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(3 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(3 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  }
  // vincInc incy
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "dot") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(1 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(1 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // vincData y
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(3 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(3 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  }
  // vincInc incy
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  if (cublasv2) {
    updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
    updateAnalysis(&call, ttCuBlasRet, &call);
  } else {
    assert(call.getType()->isFloatingPointTy());
    updateAnalysis(&call, ttFloatRet, &call);
  }
}
if (blas.function == "gemm") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // trans transa
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // trans transb
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // len m
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // len k
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(5 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(6 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(7 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(8 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(9 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(10 + offset), ttFloat, &call);
  // mldData C
  updateAnalysis(call.getArgOperand(11 + offset), ttPtr, &call);
  // mldLD ldc
  updateAnalysis(call.getArgOperand(12 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "gemv") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // trans transa
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len m
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(3 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(4 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(6 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(7 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(8 + offset), ttFloat, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(9 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(10 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "ger") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // len m
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(2 + offset), ttFloat, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "lacpy") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len m
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "lascl") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo type
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len kl
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // len ku
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // fp cfrom
  updateAnalysis(call.getArgOperand(3 + offset), ttFloat, &call);
  // fp cto
  updateAnalysis(call.getArgOperand(4 + offset), ttFloat, &call);
  // len m
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // info info
  updateAnalysis(call.getArgOperand(9 + offset), ttPtrInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "nrm2") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(1 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(1 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(1 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  if (cublasv2) {
    updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
    updateAnalysis(&call, ttCuBlasRet, &call);
  } else {
    assert(call.getType()->isFloatingPointTy());
    updateAnalysis(&call, ttFloatRet, &call);
  }
}
if (blas.function == "potrf") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(2 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // info info
  updateAnalysis(call.getArgOperand(4 + offset), ttPtrInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "potrs") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // len nrhs
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // info info
  updateAnalysis(call.getArgOperand(7 + offset), ttPtrInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "scal") {
  const int offset = (cublas ? 1 : 0);
  // len n
  updateAnalysis(call.getArgOperand(0 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(1 + offset), ttFloat, &call);
  // vincData x
  if (auto n = dyn_cast<ConstantInt>(call.getArgOperand(0 + offset))) {
    if (auto inc = dyn_cast<ConstantInt>(call.getArgOperand(2 + offset))) {
      assert(!inc->isNegative());
      TypeTree ttData = ttPtr;
      for (size_t i = 1; i < n->getZExtValue(); i++)
          ttData.insert({-1, int(i * inc->getZExtValue())}, floatType);
      updateAnalysis(call.getArgOperand(2 + offset), ttData, &call);
    } else {
      updateAnalysis(call.getArgOperand(2 + offset), ttPtr, &call);
    }
  } else {
    updateAnalysis(call.getArgOperand(2 + offset), ttPtr, &call);
  }
  // vincInc incx
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "spmv") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(2 + offset), ttFloat, &call);
  // ap ap
  // vincData x
  updateAnalysis(call.getArgOperand(4 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(6 + offset), ttFloat, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "spr2") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(2 + offset), ttFloat, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // ap ap
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "symm") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // side side
  // uplo uplo
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // len m
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(4 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(9 + offset), ttFloat, &call);
  // mldData C
  updateAnalysis(call.getArgOperand(10 + offset), ttPtr, &call);
  // mldLD ldc
  updateAnalysis(call.getArgOperand(11 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "symv") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(2 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(7 + offset), ttFloat, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(8 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(9 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "syr2") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(1 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(2 + offset), ttFloat, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(3 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // vincData y
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // vincInc incy
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "syr2k") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // trans trans
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // len k
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(4 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(9 + offset), ttFloat, &call);
  // mldData C
  updateAnalysis(call.getArgOperand(10 + offset), ttPtr, &call);
  // mldLD ldc
  updateAnalysis(call.getArgOperand(11 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "syrk") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // trans trans
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // len n
  updateAnalysis(call.getArgOperand(2 + offset), ttInt, &call);
  // len k
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(4 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // fp beta
  updateAnalysis(call.getArgOperand(7 + offset), ttFloat, &call);
  // mldData C
  updateAnalysis(call.getArgOperand(8 + offset), ttPtr, &call);
  // mldLD ldc
  updateAnalysis(call.getArgOperand(9 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "trmm") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // side side
  // uplo uplo
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // trans transa
  updateAnalysis(call.getArgOperand(2 + offset), ttChar, &call);
  // diag diag
  // len m
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(6 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(9 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(10 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "trmv") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // trans trans
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // diag diag
  // len n
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(4 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // vincData x
  updateAnalysis(call.getArgOperand(6 + offset), ttPtr, &call);
  // vincInc incx
  updateAnalysis(call.getArgOperand(7 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "trsm") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // side side
  // uplo uplo
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // trans transa
  updateAnalysis(call.getArgOperand(2 + offset), ttChar, &call);
  // diag diag
  // len m
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // len n
  updateAnalysis(call.getArgOperand(5 + offset), ttInt, &call);
  // fp alpha
  updateAnalysis(call.getArgOperand(6 + offset), ttFloat, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(9 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(10 + offset), ttInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
if (blas.function == "trtrs") {
  const int offset = ((cblas || cublas) ? 1 : 0);
  // layout layout
  // uplo uplo
  updateAnalysis(call.getArgOperand(0 + offset), ttChar, &call);
  // trans trans
  updateAnalysis(call.getArgOperand(1 + offset), ttChar, &call);
  // diag diag
  // len n
  updateAnalysis(call.getArgOperand(3 + offset), ttInt, &call);
  // len nrhs
  updateAnalysis(call.getArgOperand(4 + offset), ttInt, &call);
  // mldData A
  updateAnalysis(call.getArgOperand(5 + offset), ttPtr, &call);
  // mldLD lda
  updateAnalysis(call.getArgOperand(6 + offset), ttInt, &call);
  // mldData B
  updateAnalysis(call.getArgOperand(7 + offset), ttPtr, &call);
  // mldLD ldb
  updateAnalysis(call.getArgOperand(8 + offset), ttInt, &call);
  // info info
  updateAnalysis(call.getArgOperand(9 + offset), ttPtrInt, &call);
  if (cublas) {
    updateAnalysis(&call, ttCuBlasRet, &call);
  }
}
