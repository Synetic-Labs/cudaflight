// GPU flight harness: device-side mirror of sitl_lockstep_main.c.
//
// One thread == one firmware instance. The control schedule (settle,
// arm, fly profile) and the FNV-1a motor hash replicate the CPU harness
// step for step, so the GPU run is judged by the same oracle: all
// unperturbed instances bit-identical, a perturbed one diverging alone.
//
// Compiled for nvptx64 and linked AFTER the instancer pass — this file
// must hold no mutable globals of its own (per-instance flight state
// lives in an explicit device buffer) and may touch firmware state only
// through real firmware functions, never macros or static-inline PG
// accessors (they would read the template image).

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sitl_lockstep.h"
#include "sitl_lockstep_physics.h"

// firmware entry points (fc/init.h pulls in platform headers; declare directly)
void systemInit(void);
void initPhase1(void);
void initPhase2(void);
void initPhase3(void);
void printfSerialInit(void);
float sin_approx(float x);

// instancer layout tables
extern char __bf_image[];
extern const uint64_t __bf_image_size;
typedef struct { uint64_t loc, target; } bfReloc_t;
extern const bfReloc_t __bf_relocs[];
extern const uint64_t __bf_reloc_count;

// set by the host before any launch (defined in delta_gpu.c)
extern char *__bf_inst_base;
extern uint64_t __bf_inst_stride;
extern uint32_t __bf_inst_count;

#define KERNEL __attribute__((nvptx_kernel))

#define CONTROL_STEP_US 1000
#define SETTLE_MS       6000    // gyro calibration + 5s arming boot grace
#define ARM_MS          1000
#define HOVER_THROTTLE  1680
#define M_PIf           3.14159265358979323846f
#define FNV_BASIS       0xcbf29ce484222325ULL
#define FNV_PRIME       0x100000001b3ULL

typedef struct {
    quadSim_t sim;
    uint16_t rc[BFL_MAX_RC_CHANNELS];
    uint64_t hash;
    uint32_t ms;        // absolute control step count
    uint8_t perturbed;
} bfFlight_t;

// host reads this to size the flight-state buffer
const uint64_t __bf_state_size = sizeof(bfFlight_t);

static inline unsigned self(void)
{
    return (unsigned)__nvvm_read_ptx_sreg_ctaid_x() * (unsigned)__nvvm_read_ptx_sreg_ntid_x()
         + (unsigned)__nvvm_read_ptx_sreg_tid_x();
}

static uint64_t fnv1a64(uint64_t hash, const void *data, unsigned len)
{
    const uint8_t *p = data;
    for (unsigned i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= FNV_PRIME;
    }
    return hash;
}

static void controlStep(bfFlight_t *s)
{
    // physics turns the previous step's motor outputs into this step's
    // sensor readings, then the firmware advances against them
    quadSimStep(&s->sim, CONTROL_STEP_US * 1e-6);
    bflSetRc(s->rc, BFL_MAX_RC_CHANNELS);
    bflStepUs(CONTROL_STEP_US);

    float motors[4];
    bflGetMotorsPwm(motors, 4);
    s->hash = fnv1a64(s->hash, motors, sizeof(motors));
}

// Copy the pristine template into this thread's instance blob and rebase
// the recorded pointer slots — the GPU twin of bflInstancesCreate().
KERNEL void bfInstanceInit(bfFlight_t *st, uint32_t perturbIdx)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    char *blob = __bf_inst_base + (uint64_t)k * __bf_inst_stride;
    memcpy(blob, __bf_image, __bf_image_size);
    for (uint64_t r = 0; r < __bf_reloc_count; r++) {
        *(uint64_t *)(blob + __bf_relocs[r].loc) = (uint64_t)(blob + __bf_relocs[r].target);
    }

    bfFlight_t *s = &st[k];
    memset(s, 0, sizeof(*s));
    s->hash = FNV_BASIS;
    s->perturbed = (k == perturbIdx);
}

// Boot the instance through the real firmware init path.
KERNEL void bfBoot(bfFlight_t *st)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    bfFlight_t *s = &st[k];

    printfSerialInit();
    systemInit();
    initPhase1();
    initPhase2();
    initPhase3();

    // Map ARM to AUX1 high (firmware-side helper, see header note)
    bflConfigureArmSwitch();

    // RC defaults: sticks centred, throttle low, all aux low (AETR map)
    for (int i = 0; i < BFL_MAX_RC_CHANNELS; i++) {
        s->rc[i] = 1500;
    }
    s->rc[2] = 1000; // throttle
    for (int i = 4; i < BFL_MAX_RC_CHANNELS; i++) {
        s->rc[i] = 1000;
    }

    quadSimInit(&s->sim);
}

// Advance every instance by msCount control steps. Chunked so a long
// flight survives display-GPU kernel watchdogs; s->ms carries the
// absolute schedule across launches.
KERNEL void bfRun(bfFlight_t *st, uint32_t msCount)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    bfFlight_t *s = &st[k];

    for (uint32_t n = 0; n < msCount; n++, s->ms++) {
        const uint32_t ms = s->ms;
        uint16_t *rc = s->rc;

        if (ms == SETTLE_MS) {
            rc[4] = 1800; // AUX1 high: arm
        }
        if (ms >= SETTLE_MS + ARM_MS) {
            // fly: identical profile (and float arithmetic) to the CPU
            // harness; a perturbed instance gets a different roll amplitude
            const float t = (ms - (SETTLE_MS + ARM_MS)) * 0.001f;
            const float rollAmp = s->perturbed ? 130.0f : 100.0f;

            rc[2] = (t < 2.0f) ? (uint16_t)(1000 + (HOVER_THROTTLE - 1000) * 0.5f * t) : HOVER_THROTTLE;
            if (t >= 3.0f) {
                rc[0] = (uint16_t)(1500 + rollAmp * sin_approx(2.0f * M_PIf * 0.5f * t));
                rc[1] = (uint16_t)(1500 + 80 * sin_approx(2.0f * M_PIf * 0.3f * t + 1.0f));
                rc[3] = (uint16_t)(1500 + 60 * sin_approx(2.0f * M_PIf * 0.2f * t + 2.0f));
            } else {
                rc[0] = rc[1] = rc[3] = 1500;
            }
        }

        controlStep(s);
    }
}

// Capture a per-instance snapshot: the firmware blob plus the flight
// state, verbatim. Per-instance (not one golden image) because the booted
// firmware holds runtime-written pointers into its own blob (scheduler
// queue, currentPidProfile, ...) that are not in the static reloc table —
// a snapshot is only valid restored into the SAME blob address.
KERNEL void bfSnapshot(bfFlight_t *st, bfFlight_t *snapSt, char *snapBase)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    memcpy(snapBase + (uint64_t)k * __bf_inst_stride,
           __bf_inst_base + (uint64_t)k * __bf_inst_stride, __bf_image_size);
    snapSt[k] = st[k];
}

// Restore flagged instances to their snapshot. Pure memcpy — no pointer
// rebasing needed since source and destination are the same blob — so an
// RL episode reset costs ~56KB of bandwidth per instance. Restored
// instances replay bit-identically given identical inputs.
KERNEL void bfReset(bfFlight_t *st, bfFlight_t *snapSt, char *snapBase, const uint8_t *flags)
{
    const unsigned k = self();
    if (k >= __bf_inst_count || !flags[k]) {
        return;
    }
    memcpy(__bf_inst_base + (uint64_t)k * __bf_inst_stride,
           snapBase + (uint64_t)k * __bf_inst_stride, __bf_image_size);
    st[k] = snapSt[k];
}

// Collect the verdict inputs.
KERNEL void bfFinish(bfFlight_t *st, uint64_t *hashOut, float *altOut, uint8_t *armedOut)
{
    const unsigned k = self();
    if (k >= __bf_inst_count) {
        return;
    }
    hashOut[k] = st[k].hash;
    altOut[k] = (float)quadSimAltitude(&st[k].sim);
    armedOut[k] = bflIsArmed() ? 1 : 0;
}
