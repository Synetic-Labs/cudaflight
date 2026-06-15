// Differentiable control core + Enzyme reverse-mode gradient kernel.
//
// Linked into the firmware module BEFORE the instancer pass, so Enzyme runs
// while the firmware's per-instance state (pidRuntime, currentPidProfile, ...)
// is still distinct global symbols it can mark inactive — NOT the single
// rebased blob the instancer later packs them into (which crashes Enzyme's
// pointer inversion). After Enzyme, the instancer packs both the primal AND
// Enzyme's shadow globals per-instance and rebases every access, so each
// thread differentiates its own instance.
//
// fwCore: action [-1,1] AETR -> motors, via the REAL Betaflight stick->motor
// pipeline (bflRateCore: updateRcCommands -> processRcCommand -> pidController
// -> mixTable). bfFwStepGrad: reverse-mode VJP d(loss)/d(action) given the
// motor cotangent seed. The frozen state globals are marked inactive below.

#include <stdint.h>

#include "sitl_lockstep.h"   // bflRateCore, bflGetMotorsNormalised

#define KERNEL __attribute__((nvptx_kernel))

extern uint32_t __bf_inst_count;

static inline unsigned self(void)
{
    return (unsigned)__nvvm_read_ptx_sreg_ctaid_x() * (unsigned)__nvvm_read_ptx_sreg_ntid_x()
         + (unsigned)__nvvm_read_ptx_sreg_tid_x();
}

// ---- Enzyme markers -------------------------------------------------------
extern void __enzyme_autodiff(void *, ...);
int enzyme_dup;

// Freeze the stateful config/runtime globals: their pointer-laden structs are
// what crash Enzyme's shadow-pointer inversion, and we want the truncated
// (state-frozen) within-step gradient anyway. One global per target (the
// PreserveNVVM walk follows only operand 0). Declared opaque — only the
// symbol address matters for the marking.
#define ENZYME_INACTIVE_GLOBAL(sym) \
    extern char sym; void *__enzyme_inactive_global_##sym = (void *)&sym
ENZYME_INACTIVE_GLOBAL(currentPidProfile);
ENZYME_INACTIVE_GLOBAL(currentControlRateProfile);
ENZYME_INACTIVE_GLOBAL(pidRuntime);
ENZYME_INACTIVE_GLOBAL(mixerRuntime);
ENZYME_INACTIVE_GLOBAL(gyro);

// ---- differentiable core --------------------------------------------------
__attribute__((noinline)) static void fwCore(const float *action, float *motors)
{
    float rc[4];
    for (int i = 0; i < 4; i++) {
        rc[i] = 1500.0f + 500.0f * action[i];   // AETR [-1,1] -> [1000,2000] us
    }
    bflRateCore(rc);
    bflGetMotorsNormalised(motors, 4);
}

// Reverse-mode VJP: dActions[k] = d(loss)/d(action) given seedMotors[k] =
// d(loss)/d(motor). State frozen (truncated through-time gradient).
KERNEL void bfFwStepGrad(const float *actions, const float *seedMotors, float *dActions)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    const float *a = &actions[(uint64_t)k * 4];

    float motors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float dmotors[4];
    float da[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < 4; i++) {
        dmotors[i] = seedMotors[(uint64_t)k * 4 + i];
    }

    __enzyme_autodiff((void *)fwCore,
                      enzyme_dup, a, da,
                      enzyme_dup, motors, dmotors);

    for (int i = 0; i < 4; i++) {
        dActions[(uint64_t)k * 4 + i] = da[i];
    }
}
