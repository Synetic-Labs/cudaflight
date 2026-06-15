/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Rewriters                                                                  *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

switch(user->getOpcode()) {
  case llvm::Instruction::FNeg:{
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FNeg (DiffeRet))
    if (shadow && !gutils->isConstantValue(user->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)user))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  case llvm::Instruction::Freeze:{
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Freeze (DiffeRet))
    if (shadow && !gutils->isConstantValue(user->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)user))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  default: break;
}
if (auto BO = dyn_cast<BinaryOperator>(user)) {
  switch(BO->getOpcode()) {
  case llvm::Instruction::FAdd:{
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(BO->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(BO->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  case llvm::Instruction::FSub:{
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(BO->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FNeg (DiffeRet))
    if (shadow && !gutils->isConstantValue(BO->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  case llvm::Instruction::FMul:{
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (CheckedMul (DiffeRet), ?:$y)
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(BO->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (BO->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition BO->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(BO->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (CheckedMul (DiffeRet), ?:$x)
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(BO->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (BO->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition BO->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(BO->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  case llvm::Instruction::FDiv:{
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (CheckedDiv (DiffeRet), ?:$y)
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(BO->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (BO->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition BO->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(BO->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FNeg (CheckedMul (CheckedDiv (DiffeRet), ?:$y), (FDiv ?:$x, ?:$y)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(BO->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (BO->getOperand(1) == val || BO->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition BO->getOperand(1) == val || BO->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(BO->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  case llvm::Instruction::FRem:{
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(BO->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (CheckedMul (DiffeRet), (FNeg (anonymous_54 (anonymous_55 (anonymous_56 (FDiv ?:$x, ?:$y):$div)), ?:$div)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(BO->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (BO->getOperand(0) == val || BO->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition BO->getOperand(0) == val || BO->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(BO->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)BO))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
    default: break;
  }
}
if (auto CI = dyn_cast<CallInst>(user)) {
  switch(ID) {
    default: break;
    case Intrinsic::tanh:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_136 ?:$x):$c, ?:$c))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::sinh:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (anonymous_136 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::cosh:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (anonymous_139 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::sin:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (anonymous_47 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::cos:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (FNeg (anonymous_142 ?:$x)))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::exp:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (anonymous_128 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::exp2:
    case Intrinsic::nvvm_ex2_approx_ftz_f:
    case Intrinsic::nvvm_ex2_approx_f:
    case Intrinsic::nvvm_ex2_approx_d:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_145 ?:$x), (anonymous_128 ?:$x)))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::log:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), ?:$x)
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::log2:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_145 ?:$x), ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::log10:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_149 ?:$x), ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::copysign:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_128 (anonymous_3 ?:$x), ?:$x), (anonymous_128 (anonymous_3 ?:$y), ?:$y)))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::minnum:
    case Intrinsic::nvvm_fmin_f:
    case Intrinsic::nvvm_fmin_d:
    case Intrinsic::nvvm_fmin_ftz_f:
    case Intrinsic::x86_sse2_min_pd:
    case Intrinsic::minimum:
    case Intrinsic::minimumnum:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (Select (FCmpOLT ?:$x, ?:$y), (DiffeRet), (anonymous_0 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 1 : (Select (FCmpOLT ?:$x, ?:$y), (anonymous_0 ?:$x), (DiffeRet))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::maxnum:
    case Intrinsic::nvvm_fmax_f:
    case Intrinsic::nvvm_fmax_d:
    case Intrinsic::nvvm_fmax_ftz_f:
    case Intrinsic::x86_sse2_max_pd:
    case Intrinsic::maximum:
    case Intrinsic::maximumnum:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (Select (FCmpOLT ?:$x, ?:$y), (anonymous_0 ?:$x), (DiffeRet))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 1 : (Select (FCmpOLT ?:$x, ?:$y), (DiffeRet), (anonymous_0 ?:$x))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::fabs:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), (Select (FCmpOLT ?:$x, (anonymous_0 ?:$x)), (anonymous_155 ?:$x), (anonymous_110 ?:$x)))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::sqrt:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (Select (FCmpUEQ ?:$x, (anonymous_0 ?:$x)), (anonymous_0 ?:$x), (FDiv (DiffeRet), (FMul (anonymous_131 ?:$x), (anonymous_6 ?:$x))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::pow:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (CheckedMul (DiffeRet), (FMul ?:$y, (anonymous_128 ?:$x, (FSub ?:$y, (anonymous_110 ?:$y)))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 1 : (CheckedMul (DiffeRet), (Select (FCmpOEQ ?:$x, (anonymous_0 ?:$x)), (anonymous_0 ?:$x), (FMul (anonymous_128 ?:$x, ?:$y), (anonymous_63 ?:$x))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::powi:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (Select (Or (ICmpEQ ?:$y, (anonymous_162 ?:$y)), (And (anonymous_163), (FCmpOEQ (DiffeRet), (anonymous_0 ?:$x)))), (anonymous_0 ?:$x), (FMul (DiffeRet), (FMul (SIToFP ?:$y, (TypeOf ?:$x)), (anonymous_128 ?:$x, (ISub ?:$y, (anonymous_107 ?:$y))))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::fma:
    case Intrinsic::fmuladd:
    {
      // Rule (Op ?:$x, ?:$y, ?:$z)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FMul (DiffeRet), ?:$y)
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(1) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 1 : (FMul (DiffeRet), ?:$x)
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 2 : (DiffeRet)
      if (shadow && !gutils->isConstantValue(CI->getOperand(2)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::vector_reduce_fadd:
    {
      // Rule (Op ?:$x, ?:$y)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (DiffeRet)
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 1 : (VectorSplat (VectorSize (TypeOf ?:$y)), (DiffeRet))
      if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::acos:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FNeg (FDiv (DiffeRet), (anonymous_31 (FSub (anonymous_3 ?:$x), (FMul ?:$x, ?:$x)))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::atan:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), (FAdd (FMul ?:$x, ?:$x), (anonymous_3 ?:$x)))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
    case Intrinsic::asin:
    {
      // Rule (Op ?:$x)
      if (gutils->mode == DerivativeMode::ForwardModeError) {
        if (!gutils->isConstantValue(const_cast<Value*>(val))) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
          return true;
        }
      }
      // Arg 0 : (FDiv (DiffeRet), (anonymous_31 (FSub (anonymous_3 ?:$x), (FMul ?:$x, ?:$x))))
      if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (CI->getOperand(0) == val) {
          if (EnzymePrintDiffUse)
             llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
          return true;
        }
      }
      if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
        if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
          if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
            return true;
          }
      }
      return false;
    }
  }
  auto funcName = getFuncNameFromCall(CI);
  if ((funcName == "__nv_frcp_rd" ||
      funcName == "__nv_frcp_rn" ||
      funcName == "__nv_frcp_ru" ||
      funcName == "__nv_frcp_rz" ||
      funcName == "__nv_drcp_rd" ||
      funcName == "__nv_drcp_rn" ||
      funcName == "__nv_drcp_ru" ||
      funcName == "__nv_drcp_rz") && CI->arg_size() == 1 ){
    // Rule (Op ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FNeg (CheckedDiv (DiffeRet), (FMul ?:$y, ?:$y)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "atan" ||
      funcName == "atanf" ||
      funcName == "atanl" ||
      funcName == "__fd_atan_1") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (CheckedDiv (DiffeRet), (FAdd (FMul ?:$x, ?:$x), (anonymous_3 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "atan2" ||
      funcName == "atan2f" ||
      funcName == "atan2l" ||
      funcName == "__nv_atan2" ||
      funcName == "__nv_atan2f" ||
      funcName == "__nv_atan2l") && CI->arg_size() == 2 ){
    // Rule (Op ?:$y, ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (FMul (DiffeRet), ?:$x), (FAdd (FMul ?:$x, ?:$x), (FMul ?:$y, ?:$y)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FNeg (FDiv (FMul (DiffeRet), ?:$y), (FAdd (FMul ?:$x, ?:$x), (FMul ?:$y, ?:$y))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cbrt" ||
      funcName == "cbrtf" ||
      funcName == "cbrtl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (FMul (DiffeRet), (anonymous_6 ?:$x)), (FMul (anonymous_7 ?:$x), ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "hypot" ||
      funcName == "hypotf" ||
      funcName == "hypotl" ||
      funcName == "__hypot_finite") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (FMul (DiffeRet), ?:$x), (anonymous_6 ?:$x, ?:$y))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FDiv (FMul (DiffeRet), ?:$y), (anonymous_6 ?:$x, ?:$y))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "tanh") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_11 ?:$x):$c, ?:$c))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "tanhf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_14 ?:$x):$c, ?:$c))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "tanhl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FMul (anonymous_17 ?:$x):$c, ?:$c))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cosh") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_20 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "coshf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_23 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "coshl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_26 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sinh") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_11 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sinhf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_14 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sinhl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_17 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "asinh" ||
      funcName == "asinhf" ||
      funcName == "asinhl" ||
      funcName == "__nv_asinh" ||
      funcName == "__nv_asinhf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (anonymous_31 (FAdd (FMul ?:$x, ?:$x), (anonymous_3 ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "acosh" ||
      funcName == "acoshf" ||
      funcName == "acoshl" ||
      funcName == "__nv_acosh" ||
      funcName == "__nv_acoshf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (anonymous_31 (FSub (FMul ?:$x, ?:$x), (anonymous_3 ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "atanh" ||
      funcName == "atanhf" ||
      funcName == "atanhl" ||
      funcName == "__nv_atanh" ||
      funcName == "__nv_atanhf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FSub (anonymous_3 ?:$x), (FMul ?:$x, ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "__nv_fast_expf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (anonymous_6 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "exp10") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (FMul (DiffeRet), (anonymous_6 ?:$x)), (anonymous_36 ?:$x))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "tan" ||
      funcName == "tanf" ||
      funcName == "tanl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FAdd (anonymous_3 ?:$x), (FMul (anonymous_6 ?:$x):$c, ?:$c)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "remainder") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FMul (FNeg (DiffeRet)), (anonymous_39 (FDiv ?:$x, ?:$y)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "asin" ||
      funcName == "asinf" ||
      funcName == "asinl" ||
      funcName == "__nv_asin" ||
      funcName == "__nv_asinf" ||
      funcName == "__nv_asinl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (anonymous_31 (FSub (anonymous_3 ?:$x), (FMul ?:$x, ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "acos" ||
      funcName == "acosf" ||
      funcName == "acosl" ||
      funcName == "__nv_acos" ||
      funcName == "__nv_acosf" ||
      funcName == "__nv_acosl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FNeg (FDiv (DiffeRet), (anonymous_31 (FSub (anonymous_3 ?:$x), (FMul ?:$x, ?:$x)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "log1p" ||
      funcName == "log1pf" ||
      funcName == "log1pl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FAdd ?:$x, (anonymous_3 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "expm1" ||
      funcName == "expm1f" ||
      funcName == "expm1l") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (anonymous_44 ?:$x), (DiffeRet))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "jl_rem2pi" ||
      funcName == "jl_rem2pif" ||
      funcName == "jl_rem2pil") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sinc" ||
      funcName == "sincf" ||
      funcName == "sincl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Select (FCmpOEQ ?:$x, (anonymous_0 ?:$x)), (anonymous_0 ?:$x), (FMul (DiffeRet), (FDiv (FSub (anonymous_47 ?:$x), (anonymous_6 ?:$x)), ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sincn" ||
      funcName == "sincnf" ||
      funcName == "sincnl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Select (FCmpOEQ ?:$x, (anonymous_0 ?:$x)), (anonymous_0 ?:$x), (FMul (DiffeRet), (FDiv (FSub (anonymous_47 (FMul (anonymous_49 ?:$x), ?:$x)), (anonymous_6 ?:$x)), ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "Faddeeva_erfcx_re") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_51 ?:$x), (FSub (FMul ?:$x, (anonymous_6 ?:$x)), (anonymous_52 ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "fmod" ||
      funcName == "fmodf" ||
      funcName == "fmodl" ||
      funcName == "__nv_fmod" ||
      funcName == "__nv_fmodf" ||
      funcName == "__nv_fmodl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (CheckedMul (DiffeRet), (FNeg (anonymous_54 (anonymous_55 (anonymous_56 (FDiv ?:$x, ?:$y):$div)), ?:$div)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "modf" ||
      funcName == "modff" ||
      funcName == "modfl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$integral_part_ptr)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (DiffeRet)
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "__fd_sincos_1" ||
      funcName == "__fd_sincos_1f" ||
      funcName == "__fd_sincos_1l") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ArrayRet (FMul (anonymous_59 (anonymous_6 ?:$x):$callset), (DiffeRet)), (FNeg (FMul (anonymous_60 ?:$callset), (DiffeRet))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sincospi" ||
      funcName == "sincospif" ||
      funcName == "sincospil") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ArrayRet (FMul (anonymous_49 ?:$x), (FMul (anonymous_59 (anonymous_6 ?:$x):$callset), (DiffeRet))), (FMul (anonymous_49 ?:$x), (FNeg (FMul (anonymous_60 ?:$callset), (DiffeRet)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "xlogy_jl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (anonymous_63 ?:$y), (DiffeRet))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (Select (FCmpOEQ ?:$x, (anonymous_0 ?:$x)), (anonymous_0 ?:$x), (FMul (FDiv ?:$x, ?:$y), (DiffeRet)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "tgamma") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (anonymous_66 ?:$x), (DiffeRet))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "logabsgamma" ||
      funcName == "lgamma") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ArrayRet (FMul (anonymous_66 ?:$x), (DiffeRet)), (InactiveArg))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "logabsgammaf" ||
      funcName == "lgammaf") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ArrayRet (FMul (anonymous_70 ?:$x), (DiffeRet)), (InactiveArg))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "sinpi" ||
      funcName == "sinpif" ||
      funcName == "sinpil" ||
      funcName == "cospi" ||
      funcName == "cospif" ||
      funcName == "cospil") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (anonymous_49 ?:$x), (FMul (anonymous_6 (FAdd ?:$x, (anonymous_72 ?:$x))), (DiffeRet)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "j0") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FNeg (anonymous_75 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "j0f") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FNeg (anonymous_78 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "y0") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FNeg (anonymous_81 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "y0f") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FNeg (anonymous_84 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "j1") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_72 ?:$x), (FSub (anonymous_87 ?:$x), (anonymous_89 (anonymous_90), ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "j1f") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_72 ?:$x), (FSub (anonymous_93 ?:$x), (anonymous_95 (anonymous_90), ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "y1") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_72 ?:$x), (FSub (anonymous_98 ?:$x), (anonymous_100 (anonymous_90), ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "y1f") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_72 ?:$x), (FSub (anonymous_103 ?:$x), (anonymous_105 (anonymous_90), ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "jn" ||
      funcName == "jnf" ||
      funcName == "yn" ||
      funcName == "ynf") && CI->arg_size() == 2 ){
    // Rule (Op ?:$n, ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FMul (DiffeRet), (FMul (anonymous_72 ?:$x), (FSub (anonymous_6 (ISub ?:$n, (anonymous_107 ?:$n)), ?:$x), (anonymous_6 (IAdd ?:$n, (anonymous_107 ?:$n)), ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cmplx_in") && CI->arg_size() == 2 ){
    // Rule (Op ?:$n, ?:$z)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (CFMul (DiffeRet), (Conj (CFMul (anonymous_109 ?:$z), (CFAdd (anonymous_6 (FSub ?:$n, (anonymous_110 ?:$n)), ?:$z), (anonymous_6 (FAdd ?:$n, (anonymous_110 ?:$n)), ?:$z)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cmplx_jn" ||
      funcName == "cmplx_yn") && CI->arg_size() == 2 ){
    // Rule (Op ?:$n, ?:$z)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (CFMul (DiffeRet), (Conj (CFMul (anonymous_109 ?:$z), (CFSub (anonymous_6 (FSub ?:$n, (anonymous_110 ?:$n)), ?:$z), (anonymous_6 (FAdd ?:$n, (anonymous_110 ?:$n)), ?:$z)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "erf" ||
      funcName == "erff" ||
      funcName == "erfl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_113 ?:$x), (anonymous_44 (FNeg (FMul ?:$x, ?:$x)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "erfi") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_113 ?:$x), (anonymous_44 (FMul ?:$x, ?:$x))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "erfc" ||
      funcName == "erfcf" ||
      funcName == "erfcl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FMul (anonymous_116 ?:$x), (anonymous_44 (FNeg (FMul ?:$x, ?:$x)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "erfinv" ||
      funcName == "erfinvf" ||
      funcName == "erfinfvl" ||
      funcName == "__nv_erfinv" ||
      funcName == "__nv_erfinvf" ||
      funcName == "__nv_erfinfvl") && CI->arg_size() == 1 ){
    // Rule (Op ?:$x)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (FMul (anonymous_118 ?:$x), (FMul (anonymous_6 ?:$x):$ei, ?:$ei)), (DiffeRet))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "Faddeeva_erf") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$tbd)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ToStruct2 (Conj (CFMul (Conj (DiffeRet)), (CFMul (anonymous_120 ?:$x), (CFExp (CFNeg (CFMul ?:$x, ?:$x)))))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "Faddeeva_erfi") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$tbd)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ToStruct2 (Conj (CFMul (Conj (DiffeRet)), (CFMul (anonymous_120 ?:$x), (CFExp (CFMul ?:$x, ?:$x))))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "Faddeeva_erfc") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$tbd)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (ToStruct2 (Conj (CFMul (Conj (DiffeRet)), (CFMul (anonymous_123 ?:$x), (CFExp (CFNeg (CFMul ?:$x, ?:$x)))))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "__mulsc3" ||
      funcName == "__muldc3" ||
      funcName == "__multc3" ||
      funcName == "__mulxc3") && CI->arg_size() == 4 ){
    // Rule (Op ?:$xre, ?:$xim, ?:$yre, ?:$yim)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (StructRet (anonymous_60 (CFMul (anonymous_125), (ArrayRet ?:$yre, ?:$yim)):$r0), (anonymous_59 ?:$r0))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(2) == val || CI->getOperand(3) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(2) == val || CI->getOperand(3) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (StructRet (anonymous_60 (CFMul (anonymous_125), (ArrayRet ?:$yim, (FNeg ?:$yre))):$r1), (anonymous_59 ?:$r1))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(3) == val || CI->getOperand(2) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(3) == val || CI->getOperand(2) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 2 : (StructRet (anonymous_60 (CFMul (anonymous_125), (ArrayRet ?:$xre, ?:$xim)):$r2), (anonymous_59 ?:$r2))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(2))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(2)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 3 : (StructRet (anonymous_60 (CFMul (anonymous_125), (ArrayRet ?:$xim, (FNeg ?:$xre))):$r3), (anonymous_59 ?:$r3))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(3))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(3)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "__divsc3" ||
      funcName == "__divdc3" ||
      funcName == "__divtc3" ||
      funcName == "__divxc3") && CI->arg_size() == 4 ){
    // Rule (Op ?:$xre, ?:$xim, ?:$yre, ?:$yim)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (StructRet (anonymous_60 (CFDiv (anonymous_125), (ArrayRet ?:$yre, ?:$yim)):$r0), (anonymous_59 ?:$r0))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(2) == val || CI->getOperand(3) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(2) == val || CI->getOperand(3) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (StructRet (anonymous_60 (CFDiv (anonymous_125), (ArrayRet ?:$yim, (FNeg ?:$yre))):$r1), (anonymous_59 ?:$r1))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(3) == val || CI->getOperand(2) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(3) == val || CI->getOperand(2) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 2 : (StructRet (anonymous_60 (CFNeg (CFDiv (CFMul (anonymous_125), (ArrayRet ?:$xre, ?:$xim)), (CFMul_splat ?:$yre, ?:$yim, ?:$yre, ?:$yim))):$r2), (anonymous_59 ?:$r2))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(2))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val || CI->getOperand(2) == val || CI->getOperand(3) == val || CI->getOperand(2) == val || CI->getOperand(3) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val || CI->getOperand(2) == val || CI->getOperand(3) == val || CI->getOperand(2) == val || CI->getOperand(3) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(2)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 3 : (StructRet (anonymous_60 (CFDiv (CFMul (anonymous_125), (ArrayRet ?:$xim, (FNeg ?:$xim))), (CFMul_splat ?:$yre, ?:$yim, ?:$yre, ?:$yim)):$r2), (anonymous_59 ?:$r2))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(3))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val || CI->getOperand(2) == val || CI->getOperand(3) == val || CI->getOperand(2) == val || CI->getOperand(3) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(2) == val || CI->getOperand(3) == val || CI->getOperand(2) == val || CI->getOperand(3) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(3)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "scalbn" ||
      funcName == "scalbnf" ||
      funcName == "scalbnl" ||
      funcName == "scalbnln" ||
      funcName == "scalbnlnf" ||
      funcName == "scalbnlnl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$n)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (anonymous_128 (DiffeRet), ?:$n)
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "ldexp" ||
      funcName == "ldexpf" ||
      funcName == "ldexpl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$num, ?:$exp)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (anonymous_128 (DiffeRet), ?:$exp)
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "frexp" ||
      funcName == "frexpf" ||
      funcName == "frexpl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$expout)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FDiv (DiffeRet), (FMul (BitCast (And (MantissaMaskOfReturnForFrexp):$mask, (BitCast ?:$x, (TypeOf ?:$mask))), (TypeOf ?:$x)), (anonymous_131 ?:$x)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cabs" ||
      funcName == "cabsf" ||
      funcName == "cabsl") && CI->arg_size() == 2 ){
    // Rule (Op ?:$re, ?:$im)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (FMul (DiffeRet), (FDiv ?:$re, (anonymous_6 ?:$re, ?:$im)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (FMul (DiffeRet), (FDiv ?:$im, (anonymous_6 ?:$re, ?:$im)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(1) == val || CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(1) == val || CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cabs" ||
      funcName == "cabsf" ||
      funcName == "cabsl") && CI->arg_size() == 1 ){
    // Rule (Op (Op ?:$x, ?:$y):$z)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Cabs_helper ?:$z, (DiffeRet), (anonymous_6 ?:$z))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cmplx_inv") && CI->arg_size() == 1 ){
    // Rule (Op (Op ?:$x, ?:$y):$z)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Conj (CFDiv (CFNeg (Conj (DiffeRet))), (CFMul ?:$z, ?:$z)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "fdim" ||
      funcName == "fdimf" ||
      funcName == "fdiml") && CI->arg_size() == 2 ){
    // Rule (Op ?:$x, ?:$y)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Select (FCmpOLT ?:$x, ?:$y), (anonymous_0 ?:$x), (DiffeRet))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 1 : (Select (FCmpOLT ?:$x, ?:$y), (anonymous_0 ?:$y), (FNeg (DiffeRet)))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(1))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val || CI->getOperand(1) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val || CI->getOperand(1) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(1)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
  if ((funcName == "cmplx_sqrt") && CI->arg_size() == 1 ){
    // Rule (Op (Op ?:$x, ?:$y):$z)
    if (gutils->mode == DerivativeMode::ForwardModeError) {
      if (!gutils->isConstantValue(const_cast<Value*>(val))) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need primal of all active operands in error propagation\n";
        return true;
      }
    }
    // Arg 0 : (Select (And (FCmpUEQ ?:$x, (anonymous_0 ?:$x)), (FCmpUEQ ?:$y, (anonymous_0 ?:$y))), (anonymous_158 ?:$z), (Conj (CFDiv (Conj (DiffeRet)), (CFMul (anonymous_159 ?:$z), (anonymous_6 ?:$z)))))
    if (!shadow && !gutils->isConstantValue(const_cast<Value*>(CI->getOperand(0))) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (CI->getOperand(0) == val) {
        if (EnzymePrintDiffUse)
           llvm::errs() << "Need direct primal of " << *val << "in reverse from " << *user << " from condition CI->getOperand(0) == val";
        return true;
      }
    }
    if (shadow && !gutils->isConstantValue(CI->getOperand(0)) && !gutils->isConstantValue(const_cast<Value*>((const Value*)CI))) {
      if (qtype == QueryType::Shadow && (mode == DerivativeMode::ForwardMode || mode == DerivativeMode::ForwardModeSplit)) {
        if (EnzymePrintDiffUse) llvm::errs() << "Need forward shadow of " << *val << " from condition " << *user << "\n";
          return true;
        }
    }
    return false;
  }
    auto blasMetaData = extractBLAS(funcName);
    if (blasMetaData)
    {
      auto Mode = gutils->mode;
      const bool cacheMode = (Mode != DerivativeMode::ForwardMode);
      const std::vector<bool> *overwritten_args_ptr = nullptr;
      if (gutils->overwritten_args_map_ptr) {
        auto found = 
          gutils->overwritten_args_map_ptr->find(const_cast<CallInst *>(CI));
        assert(found != gutils->overwritten_args_map_ptr->end());
        overwritten_args_ptr = &found->second.second;
      }
      BlasInfo blas = *blasMetaData;
if (blas.function == "asum") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_x = 1 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 2 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_n = false;
  bool need_x = false;
  bool need_incx = false;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "axpy") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 1 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_x = 2 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 3 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 4 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 5 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
;  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_x) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "copy") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_x = 1 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 2 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 3 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 4 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_y) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
;  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_y) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "dot") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_x = 1 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 2 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 3 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 4 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
;  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_y) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
;  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  if (cublas) {
    if (!gutils->isConstantValue(CI->getArgOperand(5 + offset)))
      if ((shadow || gutils->runtimeActivity) && val == CI->getArgOperand(5 + offset)) return true;
    if (mode != DerivativeMode::ReverseModeGradient && !shadow && val == CI->getArgOperand(5 + offset)) return true;
  }
  return false;
}
if (blas.function == "gemm") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(12))) return true;
    if (!gutils->isConstantValue(CI->getOperand(9))) return true;
    if (!gutils->isConstantValue(CI->getOperand(7))) return true;
  }
  auto pos_transa = 0 + offset ;
  auto arg_transa = CI->getArgOperand(pos_transa);
  const bool overwritten_transa = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_transa] : true ) : false);

  auto pos_transb = 1 + offset ;
  auto arg_transb = CI->getArgOperand(pos_transb);
  const bool overwritten_transb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_transb] : true ) : false);

  auto pos_m = 2 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 3 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_k = 4 + offset ;
  auto arg_k = CI->getArgOperand(pos_k);
  const bool overwritten_k = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_k] : true ) : false);

  auto pos_alpha = 5 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 6 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 7 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 8 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 9 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  auto pos_beta = 10 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_C = 11 + offset ;
  auto arg_C = CI->getArgOperand(pos_C);
  const bool overwritten_C = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_C] : true ) : false);

  auto pos_ldc = 12 + offset ;
  auto arg_ldc = CI->getArgOperand(pos_ldc);
  const bool overwritten_ldc = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldc] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_C = !gutils->isConstantValue(arg_C);
  if (!shadow && gutils->runtimeActivity && active_C) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_transa || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_transa && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_transa : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_transb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_transb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_transb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_k || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_k && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_k : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_C || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (shadow && active_beta) return true;
    if (shadow && active_C) return true;
    if (!shadow && need_C && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_C : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldc || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldc && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldc : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "gemv") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(4))) return true;
    if (!gutils->isConstantValue(CI->getOperand(5))) return true;
    if (!gutils->isConstantValue(CI->getOperand(7))) return true;
  }
  auto pos_transa = 0 + offset ;
  auto arg_transa = CI->getArgOperand(pos_transa);
  const bool overwritten_transa = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_transa] : true ) : false);

  auto pos_m = 1 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 2 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 3 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 4 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 5 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_x = 6 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 7 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_beta = 8 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_y = 9 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 10 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_transa || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_transa && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_transa : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_A) return true;
    if (shadow && active_x) return true;
    if (shadow && active_beta) return true;
    if (shadow && active_y) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "ger") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
  }
  auto pos_m = 0 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 2 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_x = 3 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 4 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 5 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 6 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  auto pos_A = 7 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 8 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_m = false;
  bool need_n = false;
  bool need_alpha = false;
  bool need_x = false;
  bool need_incx = false;
  bool need_y = false;
  bool need_incy = false;
  bool need_A = false;
  bool need_lda = false;
  bool cache_m = cacheMode && byRef && overwritten_m && need_m;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "lacpy") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(4))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_m = 1 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 2 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_A = 3 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 4 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 5 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 6 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "nrm2") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_x = 1 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 2 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x;
  bool need_x = active_x;
  bool need_incx = active_x;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  if (cublas) {
    if (!gutils->isConstantValue(CI->getArgOperand(3 + offset)))
      if ((shadow || gutils->runtimeActivity) && val == CI->getArgOperand(3 + offset)) return true;
    if (mode != DerivativeMode::ReverseModeGradient && !shadow && val == CI->getArgOperand(3 + offset)) return true;
  }
  return false;
}
if (blas.function == "potrf") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(3))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_A = 2 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 3 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_info = 4 + offset ;
  auto arg_info = CI->getArgOperand(pos_info);
  const bool overwritten_info = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_info] : true ) : false);

  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_info || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_info && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_info : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "potrs") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(4))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_nrhs = 2 + offset ;
  auto arg_nrhs = CI->getArgOperand(pos_nrhs);
  const bool overwritten_nrhs = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_nrhs] : true ) : false);

  auto pos_A = 3 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 4 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 5 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 6 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  auto pos_info = 7 + offset ;
  auto arg_info = CI->getArgOperand(pos_info);
  const bool overwritten_info = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_info] : true ) : false);

  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_nrhs || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_nrhs && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_nrhs : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_info || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_info && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_info : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "scal") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = (cublas ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  auto pos_n = 0 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 1 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_x = 2 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 3 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_n = active_x || active_alpha;
  bool need_alpha = active_x;
  bool need_x = active_alpha;
  bool need_incx = active_x || active_alpha;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
;  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
;  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "spmv") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(4))) return true;
    if (!gutils->isConstantValue(CI->getOperand(5))) return true;
    if (!gutils->isConstantValue(CI->getOperand(3))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 2 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_ap = 3 + offset ;
  auto arg_ap = CI->getArgOperand(pos_ap);
  const bool overwritten_ap = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ap] : true ) : false);

  auto pos_x = 4 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 5 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_beta = 6 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_y = 7 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 8 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_ap = !gutils->isConstantValue(arg_ap);
  if (!shadow && gutils->runtimeActivity && active_ap) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ap || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ap && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ap : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_ap) return true;
    if (shadow && active_x) return true;
    if (shadow && active_beta) return true;
    if (shadow && active_y) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "spr2") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 2 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_x = 3 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 4 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 5 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 6 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  auto pos_ap = 7 + offset ;
  auto arg_ap = CI->getArgOperand(pos_ap);
  const bool overwritten_ap = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ap] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
  bool active_ap = !gutils->isConstantValue(arg_ap);
  if (!shadow && gutils->runtimeActivity && active_ap) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = false;
  bool need_n = false;
  bool need_alpha = false;
  bool need_x = false;
  bool need_incx = false;
  bool need_y = false;
  bool need_incy = false;
  bool need_ap = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  bool cache_ap = cacheMode && overwritten_ap && need_ap;
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ap || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ap && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ap : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "symm") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(11))) return true;
    if (!gutils->isConstantValue(CI->getOperand(8))) return true;
  }
  auto pos_side = 0 + offset ;
  auto arg_side = CI->getArgOperand(pos_side);
  const bool overwritten_side = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_side] : true ) : false);

  auto pos_uplo = 1 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_m = 2 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 3 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 4 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 5 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 6 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 7 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 8 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  auto pos_beta = 9 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_C = 10 + offset ;
  auto arg_C = CI->getArgOperand(pos_C);
  const bool overwritten_C = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_C] : true ) : false);

  auto pos_ldc = 11 + offset ;
  auto arg_ldc = CI->getArgOperand(pos_ldc);
  const bool overwritten_ldc = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldc] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_C = !gutils->isConstantValue(arg_C);
  if (!shadow && gutils->runtimeActivity && active_C) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_side || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_side && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_side : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_C || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (shadow && active_C) return true;
    if (!shadow && need_C && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_C : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldc || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldc && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldc : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "symv") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(4))) return true;
    if (!gutils->isConstantValue(CI->getOperand(3))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 2 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 3 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 4 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_x = 5 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 6 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_beta = 7 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_y = 8 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 9 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_alpha) return true;
    if (shadow && active_A) return true;
    if (shadow && active_x) return true;
    if (shadow && active_beta) return true;
    if (shadow && active_y) return true;
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "syr2") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_n = 1 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 2 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_x = 3 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 4 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  auto pos_y = 5 + offset ;
  auto arg_y = CI->getArgOperand(pos_y);
  const bool overwritten_y = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_y] : true ) : false);

  auto pos_incy = 6 + offset ;
  auto arg_incy = CI->getArgOperand(pos_incy);
  const bool overwritten_incy = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incy] : true ) : false);

  auto pos_A = 7 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 8 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
  bool active_y = !gutils->isConstantValue(arg_y);
  if (!shadow && gutils->runtimeActivity && active_y) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = false;
  bool need_n = false;
  bool need_alpha = false;
  bool need_x = false;
  bool need_incx = false;
  bool need_y = false;
  bool need_incy = false;
  bool need_A = false;
  bool need_lda = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_n = cacheMode && byRef && overwritten_n && need_n;
  bool cache_alpha = cacheMode && byRef && overwritten_alpha && need_alpha;
  bool cache_x = cacheMode && overwritten_x && need_x;
  bool cache_incx = cacheMode && byRef && overwritten_incx && need_incx;
  bool cache_y = cacheMode && overwritten_y && need_y;
  bool cache_incy = cacheMode && byRef && overwritten_incy && need_incy;
  bool cache_A = cacheMode && overwritten_A && need_A;
  bool cache_lda = cacheMode && byRef && overwritten_lda && need_lda;
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_y || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_y && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_y : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incy || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incy && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incy : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, cache_x ? ValueType::Both : ValueType::Both, cache_y ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "syr2k") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_trans = 1 + offset ;
  auto arg_trans = CI->getArgOperand(pos_trans);
  const bool overwritten_trans = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_trans] : true ) : false);

  auto pos_n = 2 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_k = 3 + offset ;
  auto arg_k = CI->getArgOperand(pos_k);
  const bool overwritten_k = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_k] : true ) : false);

  auto pos_alpha = 4 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 5 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 6 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 7 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 8 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  auto pos_beta = 9 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_C = 10 + offset ;
  auto arg_C = CI->getArgOperand(pos_C);
  const bool overwritten_C = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_C] : true ) : false);

  auto pos_ldc = 11 + offset ;
  auto arg_ldc = CI->getArgOperand(pos_ldc);
  const bool overwritten_ldc = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldc] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_C = !gutils->isConstantValue(arg_C);
  if (!shadow && gutils->runtimeActivity && active_C) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_uplo = false;
  bool need_trans = false;
  bool need_n = false;
  bool need_k = false;
  bool need_alpha = false;
  bool need_A = false;
  bool need_lda = false;
  bool need_B = false;
  bool need_ldb = false;
  bool need_beta = false;
  bool need_C = false;
  bool need_ldc = false;
  bool cache_uplo = cacheMode && byRef && overwritten_uplo && need_uplo;
  bool cache_trans = cacheMode && byRef && overwritten_trans && need_trans;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_trans || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_trans && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_trans : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_k || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_k && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_k : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_C || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_C && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_C : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldc || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldc && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldc : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "syrk") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(9))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_trans = 1 + offset ;
  auto arg_trans = CI->getArgOperand(pos_trans);
  const bool overwritten_trans = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_trans] : true ) : false);

  auto pos_n = 2 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_k = 3 + offset ;
  auto arg_k = CI->getArgOperand(pos_k);
  const bool overwritten_k = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_k] : true ) : false);

  auto pos_alpha = 4 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 5 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 6 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_beta = 7 + offset ;
  auto arg_beta = CI->getArgOperand(pos_beta);
  const bool overwritten_beta = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_beta] : true ) : false);

  auto pos_C = 8 + offset ;
  auto arg_C = CI->getArgOperand(pos_C);
  const bool overwritten_C = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_C] : true ) : false);

  auto pos_ldc = 9 + offset ;
  auto arg_ldc = CI->getArgOperand(pos_ldc);
  const bool overwritten_ldc = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldc] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_beta = !gutils->isConstantValue(arg_beta);
  if (!shadow && gutils->runtimeActivity && active_beta) return true;
  bool active_C = !gutils->isConstantValue(arg_C);
  if (!shadow && gutils->runtimeActivity && active_C) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_trans || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_trans && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_trans : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_k || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_k && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_k : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_beta || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_beta) return true;
    if (!shadow && need_beta && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_beta : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_C || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_C) return true;
    if (!shadow && need_C && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_C : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, cache_beta ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldc || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldc && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldc : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "trmm") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(8))) return true;
    if (!gutils->isConstantValue(CI->getOperand(10))) return true;
  }
  auto pos_side = 0 + offset ;
  auto arg_side = CI->getArgOperand(pos_side);
  const bool overwritten_side = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_side] : true ) : false);

  auto pos_uplo = 1 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_transa = 2 + offset ;
  auto arg_transa = CI->getArgOperand(pos_transa);
  const bool overwritten_transa = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_transa] : true ) : false);

  auto pos_diag = 3 + offset ;
  auto arg_diag = CI->getArgOperand(pos_diag);
  const bool overwritten_diag = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_diag] : true ) : false);

  auto pos_m = 4 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 5 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 6 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 7 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 8 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 9 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 10 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_side || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_side && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_side : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_transa || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_transa && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_transa : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_diag || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_diag && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_diag : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "trmv") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(5))) return true;
    if (!gutils->isConstantValue(CI->getOperand(7))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_trans = 1 + offset ;
  auto arg_trans = CI->getArgOperand(pos_trans);
  const bool overwritten_trans = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_trans] : true ) : false);

  auto pos_diag = 2 + offset ;
  auto arg_diag = CI->getArgOperand(pos_diag);
  const bool overwritten_diag = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_diag] : true ) : false);

  auto pos_n = 3 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_A = 4 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 5 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_x = 6 + offset ;
  auto arg_x = CI->getArgOperand(pos_x);
  const bool overwritten_x = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_x] : true ) : false);

  auto pos_incx = 7 + offset ;
  auto arg_incx = CI->getArgOperand(pos_incx);
  const bool overwritten_incx = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_incx] : true ) : false);

  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_x = !gutils->isConstantValue(arg_x);
  if (!shadow && gutils->runtimeActivity && active_x) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_trans || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_trans && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_trans : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_diag || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_diag && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_diag : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_x || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_x) return true;
    if (!shadow && need_x && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_x : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Both, ValueType::Both, cache_x ? ValueType::Both : ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_incx || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_incx && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_incx : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "trsm") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
  }
  auto pos_side = 0 + offset ;
  auto arg_side = CI->getArgOperand(pos_side);
  const bool overwritten_side = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_side] : true ) : false);

  auto pos_uplo = 1 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_transa = 2 + offset ;
  auto arg_transa = CI->getArgOperand(pos_transa);
  const bool overwritten_transa = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_transa] : true ) : false);

  auto pos_diag = 3 + offset ;
  auto arg_diag = CI->getArgOperand(pos_diag);
  const bool overwritten_diag = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_diag] : true ) : false);

  auto pos_m = 4 + offset ;
  auto arg_m = CI->getArgOperand(pos_m);
  const bool overwritten_m = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_m] : true ) : false);

  auto pos_n = 5 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_alpha = 6 + offset ;
  auto arg_alpha = CI->getArgOperand(pos_alpha);
  const bool overwritten_alpha = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_alpha] : true ) : false);

  auto pos_A = 7 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 8 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 9 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 10 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  bool active_alpha = !gutils->isConstantValue(arg_alpha);
  if (!shadow && gutils->runtimeActivity && active_alpha) return true;
  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
  // len, fp, etc. must be preserved if overwritten
  bool need_side = false;
  bool need_uplo = false;
  bool need_transa = false;
  bool need_diag = false;
  bool need_m = false;
  bool need_n = false;
  bool need_alpha = false;
  bool need_A = false;
  bool need_lda = false;
  bool need_B = false;
  bool need_ldb = false;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_side || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_side && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_side : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_transa || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_transa && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_transa : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_diag || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_diag && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_diag : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_m || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_m && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_m : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_alpha || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && byRefFloat && active_alpha) return true;
    if (!shadow && need_alpha && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_alpha : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, cache_alpha ? ValueType::Both : ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  return false;
}
if (blas.function == "trtrs") {
  const bool byRef = blas.prefix == "" || blas.prefix == "cublas_";
const bool byRefFloat = byRef || blas.prefix == "cublas";
(void)byRefFloat;
  const bool cblas = blas.prefix == "cblas_";
  const bool cublas = blas.prefix == "cublas_" || blas.prefix == "cublas";
  const int offset = ((cblas || cublas) ? 1 : 0);
 if (cublas && !shadow && val == CI->getArgOperand(0)) return true;
  if (!byRef && val == CI->getArgOperand(0)) {
    if (!gutils->isConstantValue(CI->getOperand(6))) return true;
    if (!gutils->isConstantValue(CI->getOperand(8))) return true;
  }
  auto pos_uplo = 0 + offset ;
  auto arg_uplo = CI->getArgOperand(pos_uplo);
  const bool overwritten_uplo = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_uplo] : true ) : false);

  auto pos_trans = 1 + offset ;
  auto arg_trans = CI->getArgOperand(pos_trans);
  const bool overwritten_trans = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_trans] : true ) : false);

  auto pos_diag = 2 + offset ;
  auto arg_diag = CI->getArgOperand(pos_diag);
  const bool overwritten_diag = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_diag] : true ) : false);

  auto pos_n = 3 + offset ;
  auto arg_n = CI->getArgOperand(pos_n);
  const bool overwritten_n = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_n] : true ) : false);

  auto pos_nrhs = 4 + offset ;
  auto arg_nrhs = CI->getArgOperand(pos_nrhs);
  const bool overwritten_nrhs = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_nrhs] : true ) : false);

  auto pos_A = 5 + offset ;
  auto arg_A = CI->getArgOperand(pos_A);
  const bool overwritten_A = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_A] : true ) : false);

  auto pos_lda = 6 + offset ;
  auto arg_lda = CI->getArgOperand(pos_lda);
  const bool overwritten_lda = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_lda] : true ) : false);

  auto pos_B = 7 + offset ;
  auto arg_B = CI->getArgOperand(pos_B);
  const bool overwritten_B = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_B] : true ) : false);

  auto pos_ldb = 8 + offset ;
  auto arg_ldb = CI->getArgOperand(pos_ldb);
  const bool overwritten_ldb = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_ldb] : true ) : false);

  auto pos_info = 9 + offset ;
  auto arg_info = CI->getArgOperand(pos_info);
  const bool overwritten_info = (cacheMode ? (overwritten_args_ptr ? (*overwritten_args_ptr)[pos_info] : true ) : false);

  bool active_A = !gutils->isConstantValue(arg_A);
  if (!shadow && gutils->runtimeActivity && active_A) return true;
  bool active_B = !gutils->isConstantValue(arg_B);
  if (!shadow && gutils->runtimeActivity && active_B) return true;
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
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_uplo || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_uplo && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_uplo : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_trans || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_trans && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_trans : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_diag || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_diag && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_diag : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_n || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_n && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_n : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_nrhs || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_nrhs && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_nrhs : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_A || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (!shadow && need_A && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_A : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_lda || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_lda && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_lda : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_B || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (shadow && active_A) return true;
    if (shadow && active_B) return true;
    if (!shadow && need_B && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_B : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_ldb || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_ldb && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_ldb : true ))
      return true;
  }
  }
  {
  SmallVector<ValueType, 1> valTys = {ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Primal, ValueType::Primal, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both, ValueType::Both}
; valTys.insert(valTys.begin(), ValueType::Primal);
  if (val == arg_info || gutils->usedInRooting(CI, valTys, val, shadow)) {
    if (!shadow && need_info && ((cacheMode && overwritten_args_ptr && (mode == DerivativeMode::ReverseModeGradient)) ? !cache_info : true ))
      return true;
  }
  }
  return false;
}
    }
}
