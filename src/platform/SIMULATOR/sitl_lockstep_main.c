/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// Golden-trace harness for the SITL_LOCKSTEP target.
//
// Boots N firmware instances against the virtual clock, arms them, flies
// a deterministic closed-loop profile (quad rigid body in
// sitl_lockstep_physics.c) and emits an FNV-1a hash of every motor output
// sample per instance. All unperturbed instances must produce the same
// hash, bit-identical to a single-instance run; a --perturb'd instance
// must diverge without affecting the others. This is the oracle every
// port (multi-instance CPU, GPU) has to satisfy.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"

#include "common/maths.h"
#include "common/printf_serial.h"

#include "drivers/system.h"

#include "fc/init.h"
#include "fc/rc_modes.h"
#include "fc/runtime_config.h"

#include "sitl_lockstep.h"
#include "sitl_lockstep_instance.h"
#include "sitl_lockstep_physics.h"

#define CONTROL_STEP_US     1000    // inject sensors/RC and sample motors at 1kHz
#define SETTLE_SECONDS      6       // gyro calibration + 5s arming boot grace period
#define ARM_SECONDS         1
#define DEFAULT_FLY_SECONDS 10

#define HOVER_THROTTLE      1680    // slightly above hover for the model in sitl_lockstep_physics.c

#define NO_PERTURB          UINT32_MAX

typedef struct {
    quadSim_t sim;
    uint16_t rc[BFL_MAX_RC_CHANNELS];
    uint64_t hash;
    bool perturbed;
} instance_t;

static instance_t *insts;
static unsigned numInstances = 1;
static uint64_t samples = 0;

static uint64_t fnv1a64(uint64_t hash, const void *data, size_t len)
{
    const uint8_t *p = data;
    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

static void activate(unsigned k)
{
    if (numInstances > 1) {
        bflInstanceActivate(k);
    }
}

static void controlStep(unsigned k)
{
    // physics turns the previous step's motor outputs into this step's
    // sensor readings, then the firmware advances against them
    activate(k);
    quadSimStep(&insts[k].sim, CONTROL_STEP_US * 1e-6);
    bflSetRc(insts[k].rc, BFL_MAX_RC_CHANNELS);
    bflStepUs(CONTROL_STEP_US);

    float motors[4];
    bflGetMotorsPwm(motors, 4);
    insts[k].hash = fnv1a64(insts[k].hash, motors, sizeof(motors));
}

static void printArmingDisableFlags(void)
{
    const armingDisableFlags_e flags = getArmingDisableFlags();
    if (!flags) {
        return;
    }
    printf("[harness] arming disabled:");
    for (unsigned i = 0; i < ARMING_DISABLE_FLAGS_COUNT; i++) {
        const armingDisableFlags_e flag = (1 << i);
        if (flags & flag) {
            printf(" %s", getArmingDisableFlagName(flag));
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int flySeconds = DEFAULT_FLY_SECONDS;
    bool trace = false;
    const char *eepromBase = NULL;
    unsigned perturb = NO_PERTURB;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            flySeconds = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--eeprom") == 0 && i + 1 < argc) {
            eepromBase = argv[++i];
        } else if (strcmp(argv[i], "--instances") == 0 && i + 1 < argc) {
            numInstances = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--perturb") == 0 && i + 1 < argc) {
            perturb = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--trace") == 0) {
            trace = true;
        } else {
            fprintf(stderr,
                    "usage: %s [--seconds N] [--eeprom FILE] [--instances N] [--perturb K] [--trace]\n",
                    argv[0]);
            return 1;
        }
    }

    setvbuf(stdout, NULL, _IOLBF, 0);

    if (numInstances < 1) {
        numInstances = 1;
    }
    if (numInstances > 1 && bflInstancesCreate(numInstances) != 0) {
        return 1;
    }

    insts = calloc(numInstances, sizeof(*insts));

    // Boot every instance through the real init path
    for (unsigned k = 0; k < numInstances; k++) {
        activate(k);

        if (eepromBase) {
            if (k == 0) {
                bflSetEepromPath(eepromBase);
            } else {
                char *path = malloc(strlen(eepromBase) + 16);
                sprintf(path, "%s.%u", eepromBase, k);
                bflSetEepromPath(path);
            }
        }

#if SERIAL_PORT_COUNT > 0
        printfSerialInit();
#endif
        systemInit();
        initPhase1();
        initPhase2();
        initPhase3();

        // Map ARM to AUX1 high (firmware-side helper: PG accessors are
        // static-inline and must not be inlined into this native file)
        bflConfigureArmSwitch();

        // RC defaults: sticks centred, throttle low, all aux low (AETR map)
        for (int i = 0; i < BFL_MAX_RC_CHANNELS; i++) {
            insts[k].rc[i] = 1500;
        }
        insts[k].rc[2] = 1000; // throttle
        for (int i = 4; i < BFL_MAX_RC_CHANNELS; i++) {
            insts[k].rc[i] = 1000;
        }

        quadSimInit(&insts[k].sim);
        insts[k].hash = 0xcbf29ce484222325ULL; // FNV-1a offset basis
        insts[k].perturbed = (k == perturb);
    }
    printf("[harness] %u instance(s) initialised at t=%uus\n",
           numInstances, (unsigned)bflMicros());

    // Settle: on the ground, level, motionless; gyro calibration completes
    for (int ms = 0; ms < SETTLE_SECONDS * 1000; ms++) {
        for (unsigned k = 0; k < numInstances; k++) {
            controlStep(k);
        }
        samples++;
    }

    // Arm
    for (unsigned k = 0; k < numInstances; k++) {
        insts[k].rc[4] = 1800; // AUX1 high
    }
    for (int ms = 0; ms < ARM_SECONDS * 1000; ms++) {
        for (unsigned k = 0; k < numInstances; k++) {
            controlStep(k);
        }
        samples++;
    }

    for (unsigned k = 0; k < numInstances; k++) {
        activate(k);
        if (!bflIsArmed()) {
            fprintf(stderr, "[harness] instance %u FAILED to arm\n", k);
            printArmingDisableFlags();
            bflDebugStatus();
            return 2;
        }
    }
    printf("[harness] all armed at t=%ums, motors=%u, motorUpdates=%llu\n",
           (unsigned)(bflMicros() / 1000), bflGetMotorCount(),
           (unsigned long long)bflGetMotorUpdateCount());

    // Fly: take off, then gentle stick wiggles. The sticks are the only
    // open-loop input; rates, attitude and altitude come from the physics
    // responding to each instance's own motor outputs.
    for (int ms = 0; ms < flySeconds * 1000; ms++) {
        const float t = ms * 0.001f;

        for (unsigned k = 0; k < numInstances; k++) {
            uint16_t *rc = insts[k].rc;
            // a perturbed instance flies a slightly different roll profile;
            // it must diverge while leaving the others bit-identical
            const float rollAmp = insts[k].perturbed ? 130.0f : 100.0f;

            // throttle ramps to just above hover over 2s, then holds
            rc[2] = (t < 2.0f) ? (uint16_t)(1000 + (HOVER_THROTTLE - 1000) * 0.5f * t) : HOVER_THROTTLE;
            // gentle stick wiggles after the climb is established
            if (t >= 3.0f) {
                rc[0] = (uint16_t)(1500 + rollAmp * sin_approx(2.0f * M_PIf * 0.5f * t));
                rc[1] = (uint16_t)(1500 + 80 * sin_approx(2.0f * M_PIf * 0.3f * t + 1.0f));
                rc[3] = (uint16_t)(1500 + 60 * sin_approx(2.0f * M_PIf * 0.2f * t + 2.0f));
            } else {
                rc[0] = rc[1] = rc[3] = 1500;
            }

            controlStep(k);
        }
        samples++;

        if (trace || (ms % 1000) == 0) {
            float m[4];
            double roll, pitch, yaw;
            activate(0);
            bflGetMotorsPwm(m, 4);
            quadSimEulerDeg(&insts[0].sim, &roll, &pitch, &yaw);
            printf("[trace] t=%5.2fs thr=%u alt=%6.2fm rpy=%6.1f %6.1f %6.1f motors= %7.2f %7.2f %7.2f %7.2f\n",
                   (double)t, insts[0].rc[2], quadSimAltitude(&insts[0].sim), roll, pitch, yaw,
                   (double)m[0], (double)m[1], (double)m[2], (double)m[3]);
        }
    }

    // Verdict
    bool allArmedAirborne = true;
    bool unperturbedIdentical = true;
    uint64_t refHash = 0;
    bool haveRef = false;

    for (unsigned k = 0; k < numInstances; k++) {
        activate(k);
        const bool armed = bflIsArmed();
        const bool airborne = quadSimAltitude(&insts[k].sim) > 1.0;
        allArmedAirborne = allArmedAirborne && armed && airborne;

        if (!insts[k].perturbed) {
            if (!haveRef) {
                refHash = insts[k].hash;
                haveRef = true;
            } else if (insts[k].hash != refHash) {
                unperturbedIdentical = false;
            }
        }

        printf("[harness] instance %u: armed=%d alt=%7.2fm hash=%016llx%s\n",
               k, armed, quadSimAltitude(&insts[k].sim),
               (unsigned long long)insts[k].hash,
               insts[k].perturbed ? " (perturbed)" : "");
    }

    printf("[harness] done: t=%ums samples=%llu instances=%u identical=%d\n",
           (unsigned)(bflMicros() / 1000), (unsigned long long)samples,
           numInstances, unperturbedIdentical);
    printf("TRACE_HASH: %016llx\n", (unsigned long long)refHash);

    if (perturb != NO_PERTURB && perturb < numInstances && insts[perturb].hash == refHash) {
        fprintf(stderr, "[harness] PERTURBED instance did not diverge\n");
        return 4;
    }
    if (!unperturbedIdentical) {
        fprintf(stderr, "[harness] HASH MISMATCH between unperturbed instances\n");
        return 4;
    }
    return allArmedAirborne ? 0 : 3;
}
