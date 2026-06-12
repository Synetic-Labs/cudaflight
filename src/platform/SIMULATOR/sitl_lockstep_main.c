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

// --- CLI dump loading -------------------------------------------------------
// Turns a configurator/manufacturer CLI dump ("diff all" text) into EEPROM
// contents: sanitise reboot-class commands out of the text, feed it through
// the real CLI parser, then save without rebooting. The eeprom file written
// via --eeprom is then a boot-ready config for any backend (CPU file mode,
// GPU RAM preload).

static bool lineFirstTokenIs(const char *line, const char *token)
{
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    const size_t n = strlen(token);
    if (strncasecmp(line, token, n) != 0) {
        return false;
    }
    const char c = line[n];
    return c == '\0' || c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '#';
}

static char *sanitizeCliDump(const char *text, size_t len, size_t *outLen)
{
    // worst case: every line is "defaults" gaining " nosave"
    char *out = malloc(len + len / 2 + 64);
    size_t o = 0;

    const char *p = text;
    const char *end = text + len;
    while (p < end) {
        const char *nl = memchr(p, '\n', end - p);
        const size_t lineLen = nl ? (size_t)(nl - p) + 1 : (size_t)(end - p);
        char line[512];
        const size_t n = lineLen < sizeof(line) - 1 ? lineLen : sizeof(line) - 1;
        memcpy(line, p, n);
        line[n] = '\0';
        p += lineLen;

        // reboot-class / batch commands are the harness's business, not
        // the dump's: 'save' and 'exit' reboot (exit(0) on this target);
        // 'batch' would turn any unknown-setting error into a refused
        // save; 'defaults' without nosave reboots too
        if (lineFirstTokenIs(line, "save") || lineFirstTokenIs(line, "exit") ||
            lineFirstTokenIs(line, "batch") || lineFirstTokenIs(line, "bl") ||
            lineFirstTokenIs(line, "msc")) {
            continue;
        }
        if (lineFirstTokenIs(line, "defaults") && strcasestr(line, "nosave") == NULL) {
            o += sprintf(out + o, "defaults nosave\n");
            continue;
        }
        memcpy(out + o, line, n);
        o += n;
    }
    o += sprintf(out + o, "\nsave noreboot\n");
    out[o] = '\0';
    *outLen = o;
    return out;
}

static int runCliDumpConverter(const char *dumpPath)
{
    FILE *f = fopen(dumpPath, "r");
    if (!f) {
        fprintf(stderr, "[harness] cannot open CLI dump '%s'\n", dumpPath);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    const long size = ftell(f);
    rewind(f);
    char *text = malloc(size + 1);
    if (fread(text, 1, size, f) != (size_t)size) {
        fprintf(stderr, "[harness] failed to read '%s'\n", dumpPath);
        return 1;
    }
    fclose(f);
    text[size] = '\0';

    size_t cliLen;
    char *cli = sanitizeCliDump(text, size, &cliLen);
    free(text);

    printf("[harness] applying CLI dump '%s' (%ld bytes)\n", dumpPath, size);
    bflCliExec(cli, (uint32_t)cliLen);
    free(cli);

    // show what the firmware actually accepted — this output is the
    // verification artifact, compare it against the source dump
    printf("[harness] resulting configuration (diff all):\n");
    const char diffCmd[] = "diff all\n";
    bflCliExec(diffCmd, sizeof(diffCmd) - 1);

    return 0;
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

// ASCII view of an instance's OSD character grid. Letters/digits/punctuation
// sit at their ASCII codepoints in Betaflight fonts so text reads directly;
// symbol glyphs (horizon line, battery icon, ...) print as '.'.
static void dumpOsd(unsigned k, const char *when)
{
    activate(k);
    const unsigned rows = bflOsdRows(), cols = bflOsdCols();
    const uint8_t *s = bflOsdScreen();
    printf("[osd] instance %u %s (draws=%u)\n", k, when, (unsigned)bflOsdDrawCount());
    printf("      +%.*s+\n", cols, "------------------------------------------------");
    for (unsigned y = 0; y < rows; y++) {
        char line[64];
        for (unsigned x = 0; x < cols; x++) {
            const uint8_t c = s[y * cols + x];
            line[x] = (c >= 0x20 && c < 0x7f) ? (char)c : '.';
        }
        line[cols] = '\0';
        printf("      |%s|\n", line);
    }
    printf("      +%.*s+\n", cols, "------------------------------------------------");
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
    bool osdDump = false;
    const char *eepromBase = NULL;
    const char *cliDumpPath = NULL;
    unsigned perturb = NO_PERTURB;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            flySeconds = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--eeprom") == 0 && i + 1 < argc) {
            eepromBase = argv[++i];
        } else if (strcmp(argv[i], "--cli-dump") == 0 && i + 1 < argc) {
            cliDumpPath = argv[++i];
        } else if (strcmp(argv[i], "--instances") == 0 && i + 1 < argc) {
            numInstances = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--perturb") == 0 && i + 1 < argc) {
            perturb = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--trace") == 0) {
            trace = true;
        } else if (strcmp(argv[i], "--osd") == 0) {
            osdDump = true;
        } else {
            fprintf(stderr,
                    "usage: %s [--seconds N] [--eeprom FILE] [--instances N] [--perturb K] [--trace] [--osd]\n"
                    "       %s --cli-dump DUMP.txt --eeprom OUT.bin   (convert CLI dump to eeprom image)\n",
                    argv[0], argv[0]);
            return 1;
        }
    }

    if (cliDumpPath) {
        // converter mode: boot one instance, apply the dump, save, exit
        numInstances = 1;
    }

    setvbuf(stdout, NULL, _IOLBF, 0);

    bflInstanceTemplateFixup();

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

        if (cliDumpPath) {
            // before bflConfigureArmSwitch so the saved EEPROM is purely
            // the dump's configuration
            const int rc = runCliDumpConverter(cliDumpPath);
            if (rc == 0) {
                printf("[harness] eeprom written to '%s' (%u bytes)\n",
                       eepromBase ? eepromBase : EEPROM_FILENAME, bflEepromSize());
            }
            return rc;
        }

        // Map ARM to AUX1 high (firmware-side helper: PG accessors are
        // static-inline and must not be inlined into this native file)
        bflConfigureArmSwitch();

        // OSD: with a layout-less config, enable the demo elements and
        // tag each instance by name so a wall of instances is tellable apart
        bflOsdApplyDemoLayoutIfBlank();
        char craftName[26];
        sprintf(craftName, "BETAFLIGHT %04u", k % 10000);
        bflOsdDefaultCraftName(craftName);

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
        if (osdDump && (ms == 1000 || ms == 5000)) {
            char when[32];
            sprintf(when, "settle t=%ds", ms / 1000);
            dumpOsd(0, when); // 1s: boot logo; 5s: live grid, disarmed
        }
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

    if (osdDump) {
        dumpOsd(0, "end of flight");
        if (perturb != NO_PERTURB && perturb < numInstances) {
            dumpOsd(perturb, "end of flight (perturbed)");
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
