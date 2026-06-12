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
// Boots the firmware against the virtual clock, arms it, flies a
// deterministic input profile and emits an FNV-1a hash of every motor
// output sample. Two runs of the same binary must print the same hash;
// any future port (multi-instance, GPU) must reproduce it bit-exactly.
//
// The loop is closed: a quad rigid-body model (sitl_lockstep_physics.c)
// turns the firmware's motor outputs into the gyro/acc/baro readings it
// sees on the next step, so the PID loop flies a real (simulated)
// airframe rather than a canned sensor profile.

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
#include "sitl_lockstep_physics.h"

#define CONTROL_STEP_US     1000    // inject sensors/RC and sample motors at 1kHz
#define SETTLE_SECONDS      6       // gyro calibration + 5s arming boot grace period
#define ARM_SECONDS         1
#define DEFAULT_FLY_SECONDS 10

#define HOVER_THROTTLE      1680    // slightly above hover for the model in sitl_lockstep_physics.c

static uint64_t fnv1a64(uint64_t hash, const void *data, size_t len)
{
    const uint8_t *p = data;
    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= 0x100000001b3ULL;
    }
    return hash;
}

static uint16_t rc[BFL_MAX_RC_CHANNELS];
static uint64_t traceHash = 0xcbf29ce484222325ULL; // FNV-1a offset basis
static uint64_t samples = 0;
static quadSim_t sim;

static void controlStep(void)
{
    // physics turns the previous step's motor outputs into this step's
    // sensor readings, then the firmware advances against them
    quadSimStep(&sim, CONTROL_STEP_US * 1e-6);
    bflSetRc(rc, BFL_MAX_RC_CHANNELS);
    bflStepUs(CONTROL_STEP_US);

    float motors[4];
    bflGetMotorsPwm(motors, 4);
    traceHash = fnv1a64(traceHash, motors, sizeof(motors));
    samples++;
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

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            flySeconds = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--eeprom") == 0 && i + 1 < argc) {
            bflSetEepromPath(argv[++i]);
        } else if (strcmp(argv[i], "--trace") == 0) {
            trace = true;
        } else {
            fprintf(stderr, "usage: %s [--seconds N] [--eeprom FILE] [--trace]\n", argv[0]);
            return 1;
        }
    }

    setvbuf(stdout, NULL, _IOLBF, 0);

#if SERIAL_PORT_COUNT > 0
    printfSerialInit();
#endif

    systemInit();
    initPhase1();
    initPhase2();
    initPhase3();

    printf("[harness] init complete at t=%uus\n", (unsigned)bflMicros());

    // Map ARM to AUX1 high (equivalent of CLI: aux 0 0 0 1700 2100 0 0)
    modeActivationConditionsMutable(0)->modeId = BOXARM;
    modeActivationConditionsMutable(0)->auxChannelIndex = 0;
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(0)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);
    analyzeModeActivationConditions();

    // RC defaults: sticks centred, throttle low, all aux low (AETR map)
    for (int i = 0; i < BFL_MAX_RC_CHANNELS; i++) {
        rc[i] = 1500;
    }
    rc[2] = 1000; // throttle
    for (int i = 4; i < BFL_MAX_RC_CHANNELS; i++) {
        rc[i] = 1000;
    }

    // Settle: on the ground, level, motionless; gyro calibration completes
    quadSimInit(&sim);
    for (int ms = 0; ms < SETTLE_SECONDS * 1000; ms++) {
        controlStep();
    }
    printArmingDisableFlags();

    // Arm
    rc[4] = 1800; // AUX1 high
    for (int ms = 0; ms < ARM_SECONDS * 1000; ms++) {
        controlStep();
    }

    if (!ARMING_FLAG(ARMED)) {
        fprintf(stderr, "[harness] FAILED to arm\n");
        printArmingDisableFlags();
        return 2;
    }
    printf("[harness] armed at t=%ums, motors=%u, motorUpdates=%llu\n",
           (unsigned)(bflMicros() / 1000), bflGetMotorCount(),
           (unsigned long long)bflGetMotorUpdateCount());

    // Fly: take off, then gentle stick wiggles. The sticks are the only
    // open-loop input now; rates, attitude and altitude come from the
    // physics responding to the firmware's motor outputs.
    for (int ms = 0; ms < flySeconds * 1000; ms++) {
        const float t = ms * 0.001f;

        // throttle ramps to just above hover over 2s, then holds
        rc[2] = (t < 2.0f) ? (uint16_t)(1000 + (HOVER_THROTTLE - 1000) * 0.5f * t) : HOVER_THROTTLE;
        // gentle stick wiggles after the climb is established
        if (t >= 3.0f) {
            rc[0] = (uint16_t)(1500 + 100 * sin_approx(2.0f * M_PIf * 0.5f * t));
            rc[1] = (uint16_t)(1500 + 80 * sin_approx(2.0f * M_PIf * 0.3f * t + 1.0f));
            rc[3] = (uint16_t)(1500 + 60 * sin_approx(2.0f * M_PIf * 0.2f * t + 2.0f));
        } else {
            rc[0] = rc[1] = rc[3] = 1500;
        }

        controlStep();

        if (trace || (ms % 1000) == 0) {
            float m[4];
            double roll, pitch, yaw;
            bflGetMotorsPwm(m, 4);
            quadSimEulerDeg(&sim, &roll, &pitch, &yaw);
            printf("[trace] t=%5.2fs thr=%u alt=%6.2fm rpy=%6.1f %6.1f %6.1f rates=%6.1f %6.1f %6.1f motors= %7.2f %7.2f %7.2f %7.2f\n",
                   (double)t, rc[2], quadSimAltitude(&sim), roll, pitch, yaw,
                   sim.omega[0] * 180.0 / 3.14159265358979, sim.omega[1] * 180.0 / 3.14159265358979, sim.omega[2] * 180.0 / 3.14159265358979,
                   (double)m[0], (double)m[1], (double)m[2], (double)m[3]);
        }
    }

    const bool stillArmed = ARMING_FLAG(ARMED);
    const bool airborne = quadSimAltitude(&sim) > 1.0;
    printf("[harness] done: t=%ums samples=%llu motorUpdates=%llu armed=%d alt=%.2fm airborne=%d\n",
           (unsigned)(bflMicros() / 1000), (unsigned long long)samples,
           (unsigned long long)bflGetMotorUpdateCount(), stillArmed,
           quadSimAltitude(&sim), airborne);
    printf("TRACE_HASH: %016llx\n", (unsigned long long)traceHash);

    return (stillArmed && airborne) ? 0 : 3;
}
