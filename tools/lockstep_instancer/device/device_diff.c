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
#include <string.h>          // memcpy (state save/restore)

#include "sitl_lockstep.h"   // bflRateCore, bflGetMotorsRaw

#define KERNEL __attribute__((nvptx_kernel))

extern uint32_t __bf_inst_count;
extern char *__bf_inst_base;
extern uint64_t __bf_inst_stride;
extern const uint64_t __bf_image_size;
// Complete relocation table (flat {loc, targetOff} pairs), host-set in
// delta_gpu.c. Used by the value-threaded gradient kernel to rebase-on-entry
// (the twin of device_flight.c's bfRebaseBlob — duplicated here because that TU
// is linked only AFTER the instancer, so its static helpers aren't visible).
extern const uint64_t *__bf_full_relocs;
extern uint64_t __bf_full_reloc_count;

static inline unsigned self(void)
{
    return (unsigned)__nvvm_read_ptx_sreg_ctaid_x() * (unsigned)__nvvm_read_ptx_sreg_ntid_x()
         + (unsigned)__nvvm_read_ptx_sreg_tid_x();
}

// ---- Enzyme markers -------------------------------------------------------
extern void __enzyme_autodiff(void *, ...);   // reverse mode (taped)
extern void __enzyme_fwddiff(void *, ...);    // forward mode (no tape)
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
    // Raw float mixer output (motor[]), NOT bflGetMotorsNormalised: the latter
    // reads motorsNormalised[], which writeMotors() produces via an int16
    // quantization. Differentiating through that integer truncation yields a
    // zero gradient almost everywhere — Enzyme correctly returns 0. The raw
    // pre-quantization signal is the smooth one the gradient must read (the
    // same source the finite-difference Jacobian in device_flight.c uses).
    bflGetMotorsRaw(motors, 4);
}

// Reverse-mode VJP: dActions[k] = d(loss)/d(action) given seedMotors[k] =
// d(loss)/d(motor). State frozen (truncated through-time gradient).
// scratch: [N x __bf_inst_stride] device scratch (same buffer the FD Jacobian
// uses). The autodiff's FORWARD sweep runs the real control law, which mutates
// the per-instance blob (stateful pt1/pt2/pt3 filters in pidRuntime, rcData,
// motor, AND Enzyme's shadow globals). Save the blob first and restore it after
// so every call evaluates the gradient at the SAME frozen state — otherwise the
// filters advance each call and the gradient drifts (and won't match the FD
// Jacobian, which save/restores the same way). The restore also re-zeros the
// shadow globals (they live in the blob and start at zero post-boot), so Enzyme
// never sees stale adjoint residue from a previous launch.
KERNEL void bfFwStepGrad(const float *actions, const float *seedMotors,
                         float *dActions, char *scratch)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    char *blob = __bf_inst_base + (uint64_t)k * __bf_inst_stride;
    char *sc = scratch + (uint64_t)k * __bf_inst_stride;
    memcpy(sc, blob, __bf_image_size);   // freeze current instance state

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

    memcpy(blob, sc, __bf_image_size);   // restore state + shadow globals

    for (int i = 0; i < 4; i++) {
        dActions[(uint64_t)k * 4 + i] = da[i];
    }
}

// Rebase-aware full-Jacobian twin of bfFwStepGrad for the value-threaded ("pure")
// path. FORWARD-mode Enzyme counterpart of bfFwStepJacFDPure (device_flight.c):
// rebases the threaded blob on entry, then fills jacOut[k][i][d] = d(motor_i)/
// d(action_d) by one FORWARD sweep per action (seed = e_d recovers COLUMN d,
// since a forward sweep returns dmotors = J . seed). The custom_vjp stores J in
// the forward and contracts J^T . cotangent in a pure backward — so the backward
// never re-enters the firmware at a drifted state. The blob is save/restored
// around each sweep (re-zeroing the shadow globals too). A bfSetBase launch must
// point __bf_inst_base at this blob ahead of the call (FFI handler).
//
// Forward mode (NOT reverse): for the R^4->R^4 control Jacobian a forward sweep
// per action column costs exactly the same 4 evaluations as a reverse sweep per
// motor row, but carries NO value tape — reverse-mode Enzyme heap-allocates its
// tape via device malloc (hundreds of small allocs per thread), which exhausts
// the device malloc heap once fleet x horizon is large (a null malloc -> illegal
// address; see bfgym.cpp). Forward mode has no tape, so it scales to any fleet.
KERNEL void bfFwStepJacGradPure(const float *actions, float *jacOut, char *scratch)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    char *blob = __bf_inst_base + (uint64_t)k * __bf_inst_stride;
    // rebase-on-entry (unconditional): fix this instance's self-pointers for the
    // address XLA placed the threaded blob at, before bflRateCore dereferences them.
    for (uint64_t r = 0; r < __bf_full_reloc_count; r++) {
        const uint64_t loc = __bf_full_relocs[2 * r];
        const uint64_t targetOff = __bf_full_relocs[2 * r + 1];
        *(uint64_t *)(blob + loc) = (uint64_t)blob + targetOff;
    }
    char *sc = scratch + (uint64_t)k * __bf_inst_stride;
    memcpy(sc, blob, __bf_image_size);   // freeze current instance state

    const float *a = &actions[(uint64_t)k * 4];
    float *J = &jacOut[(uint64_t)k * 16];

    for (int d = 0; d < 4; d++) {
        float motors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        float dmotors[4] = { 0.0f, 0.0f, 0.0f, 0.0f };   // forward sweep result
        float da[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        da[d] = 1.0f;   // unit action tangent e_d -> dmotors = column d of J

        __enzyme_fwddiff((void *)fwCore,
                         enzyme_dup, a, da,
                         enzyme_dup, motors, dmotors);

        memcpy(blob, sc, __bf_image_size);   // restore state + shadow globals
        for (int i = 0; i < 4; i++) {
            J[i * 4 + d] = dmotors[i];   // J[i][d] = d(motor_i)/d(action_d)
        }
    }
}
