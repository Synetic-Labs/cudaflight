// SPDX-License-Identifier: GPL-3.0-or-later
// cpuflight: the CPU twin of cudaflight's external-physics API, as a shared
// library. N Betaflight instances run in-process through the IR-instanced
// SITL_LOCKSTEP firmware (sitl_lockstep_instance.c manages the per-instance
// state images; __bf_delta switches instances), stepped sequentially on the
// host against sensors computed by an outside simulator.
//
// Scope: exactly the external-physics surface an outside simulator needs —
// create (boot/settle/arm/snapshot), fw_step (sticks+sensors -> motors),
// aux channels, masked reset, destroy. No in-process RL task, no OSD wall.
//
// Why it exists: cudaflight refuses fleets of n < 3 (the runtime relocation
// table that makes value-threaded GPU blobs position-independent cannot be
// discovered below 3 instances). CPU instances never move — their heap blobs
// are rebased once at creation and a snapshot restored to the same address
// is trivially valid — so there is no lower fleet bound here, and at those
// sizes the CPU firmware is faster than a GPU dispatch anyway.
//
// Semantics mirror the GPU path step for step (device_flight.c): the same
// boot helpers, the same settle/arm schedule against the in-process ground
// physics, the same AETR->RC mapping and sensor injection in fw_step. One
// fleet per process at a time; create after destroy reuses the pristine
// template.

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/platform/SIMULATOR/sitl_lockstep.h"
#include "../../src/platform/SIMULATOR/sitl_lockstep_instance.h"
#include "../../src/platform/SIMULATOR/sitl_lockstep_physics.h"

// firmware entry points (fc/init.h pulls in platform headers; declare
// directly — same approach as the GPU harness device_flight.c)
void systemInit(void);
void initPhase1(void);
void initPhase2(void);
void initPhase3(void);
void printfSerialInit(void);

#define CONTROL_STEP_US 1000
#define SETTLE_MS       6000    // gyro calibration + 5s arming boot grace
#define DEFAULT_CREATE_MS 7000  // SETTLE_MS + 1s armed on the ground
#define ACT_DIM 4
#define AUX_DIM 4

static char g_err[512];

typedef struct {
    quadSim_t sim;
    uint16_t rc[BFL_MAX_RC_CHANNELS];
} cpuInstance_t;

typedef struct cpuflight {
    uint32_t n;
    size_t imageSize;
    cpuInstance_t *inst;
    cpuInstance_t *snapInst;
    char *snapBlobs;        // n * imageSize, episode-start images
} cpuflight;

static bool g_active = false;   // one fleet per process at a time

int cpuflight_snapshot(cpuflight *g);

const char *cpuflight_error(void)
{
    return g_err;
}

static float clamp1(float v)
{
    return v < -1.0f ? -1.0f : (v > 1.0f ? 1.0f : v);
}

// One 1 ms ground step for instance k: in-process physics turns the previous
// step's motors into sensor readings, then the firmware advances. Only used
// during create (settle/arm); flight stepping is cpuflight_fw_step with
// caller-supplied sensors.
static void groundStep(cpuflight *g, uint32_t k)
{
    bflInstanceActivate(k);
    quadSimStep(&g->inst[k].sim, CONTROL_STEP_US * 1e-6);
    bflSetRc(g->inst[k].rc, BFL_MAX_RC_CHANNELS);
    bflStepUs(CONTROL_STEP_US);
}

void cpuflight_destroy(cpuflight *g)
{
    if (!g) {
        return;
    }
    free(g->inst);
    free(g->snapInst);
    free(g->snapBlobs);
    free(g);
    bflInstancesDestroy();
    g_active = false;
}

cpuflight *cpuflight_create_eeprom(uint32_t n, uint32_t settle_ms, const char *eeprom_path)
{
    if (n == 0) {
        snprintf(g_err, sizeof(g_err), "fleet size must be >= 1 (got %u)", n);
        return NULL;
    }
    if (g_active) {
        snprintf(g_err, sizeof(g_err),
                 "a cpuflight fleet already exists in this process; destroy it first");
        return NULL;
    }
    if (settle_ms == 0) {
        settle_ms = DEFAULT_CREATE_MS;
    }

    bflInstanceTemplateFixup();
    if (bflInstanceImageSize() == 0) {
        snprintf(g_err, sizeof(g_err),
                 "this binary is not instanced (build libcpuflight.so via "
                 "tools/lockstep_instancer/build_multi.sh)");
        return NULL;
    }
    if (bflInstancesCreate(n) != 0) {
        snprintf(g_err, sizeof(g_err), "instance allocation failed (n=%u)", n);
        return NULL;
    }
    g_active = true;

    cpuflight *g = calloc(1, sizeof(*g));
    if (!g) {
        snprintf(g_err, sizeof(g_err), "host allocation failed (n=%u)", n);
        bflInstancesDestroy();
        g_active = false;
        return NULL;
    }
    g->n = n;
    g->imageSize = bflInstanceImageSize();
    g->inst = calloc(n, sizeof(*g->inst));
    g->snapInst = calloc(n, sizeof(*g->snapInst));
    g->snapBlobs = malloc((size_t)n * g->imageSize);
    if (!g->inst || !g->snapInst || !g->snapBlobs) {
        snprintf(g_err, sizeof(g_err), "host allocation failed (n=%u)", n);
        cpuflight_destroy(g);
        return NULL;
    }

    // optional boot-ready eeprom image (from the harness --cli-dump converter)
    uint8_t *ee = NULL;
    size_t eeLen = 0;
    if (eeprom_path) {
        FILE *f = fopen(eeprom_path, "rb");
        if (!f) {
            snprintf(g_err, sizeof(g_err), "cannot open eeprom image '%s'", eeprom_path);
            cpuflight_destroy(g);
            return NULL;
        }
        fseek(f, 0, SEEK_END);
        eeLen = (size_t)ftell(f);
        rewind(f);
        ee = malloc(eeLen);
        const bool readOk = ee && fread(ee, 1, eeLen, f) == eeLen;
        fclose(f);
        if (!readOk) {
            snprintf(g_err, sizeof(g_err), "failed to read eeprom image '%s'", eeprom_path);
            free(ee);
            cpuflight_destroy(g);
            return NULL;
        }
    }

    // Boot every instance through the real init path (the CPU twin of bfBoot,
    // plus the GPU host's RAM-eeprom preload ahead of it).
    for (uint32_t k = 0; k < n; k++) {
        bflInstanceActivate(k);

        // RAM-only EEPROM, like the GPU build (BFL_EEPROM_RAM): the CPU target
        // defaults to file-backed (./eeprom.bin), which would overwrite the
        // preloaded image at boot. eepromPath is per-instance state, so set it
        // after activate.
        bflSetEepromPath(NULL);
        if (ee) {
            const uint32_t cap = bflEepromSize();
            memcpy(bflEepromBuffer(), ee, eeLen > cap ? cap : eeLen);
        }

        printfSerialInit();
        systemInit();
        initPhase1();
        initPhase2();
        initPhase3();

        bflConfigureArmSwitch();
        bflConfigureModeSwitch();

        bflOsdApplyDemoLayoutIfBlank();
        char craftName[16];
        snprintf(craftName, sizeof(craftName), "BETAFLIGHT %04u", k % 10000);
        bflOsdDefaultCraftName(craftName);

        // RC defaults: sticks centred, throttle low, all aux low (AETR map)
        for (int i = 0; i < BFL_MAX_RC_CHANNELS; i++) {
            g->inst[k].rc[i] = 1500;
        }
        g->inst[k].rc[2] = 1000; // throttle
        for (int i = 4; i < BFL_MAX_RC_CHANNELS; i++) {
            g->inst[k].rc[i] = 1000;
        }

        quadSimInit(&g->inst[k].sim);
    }
    free(ee);

    // Settle on the ground (gyro calibration), arm at SETTLE_MS, then idle
    // armed until settle_ms — the schedule bfRun applies on the GPU.
    for (uint32_t ms = 0; ms < settle_ms; ms++) {
        for (uint32_t k = 0; k < n; k++) {
            if (ms == SETTLE_MS) {
                g->inst[k].rc[4] = 1800; // AUX1 high: arm
            }
            groundStep(g, k);
        }
    }

    for (uint32_t k = 0; k < n; k++) {
        bflInstanceActivate(k);
        if (!bflIsArmed()) {
            snprintf(g_err, sizeof(g_err),
                     "instance %u failed to arm during create (settle_ms=%u)", k, settle_ms);
            cpuflight_destroy(g);
            return NULL;
        }
    }

    if (cpuflight_snapshot(g) != 0) {
        cpuflight_destroy(g);
        return NULL;
    }
    return g;
}

cpuflight *cpuflight_create(uint32_t n, uint32_t settle_ms)
{
    return cpuflight_create_eeprom(n, settle_ms, NULL);
}

// --- CLI-dump -> eeprom-image renderer --------------------------------------
// The CLI text is the durable config format (name=value, applied by setting
// name, version-agnostic); the eeprom image is a derived artifact valid only
// for this firmware build — render it at use time, never commit it.

// Harness-heap line accumulators for the last render (unbounded on purpose —
// per-instance firmware state must stay small).
typedef struct {
    char *buf;
    size_t len;
    size_t cap;
} lineAcc_t;

static lineAcc_t g_renderErrs;   // output lines carrying a CLI error marker
static lineAcc_t g_renderDump;   // the round-trip 'dump all' after the save

static void accAppend(lineAcc_t *acc, const char *line)
{
    const size_t n = strlen(line);
    if (acc->len + n + 2 > acc->cap) {
        const size_t cap = (acc->cap ? acc->cap * 2 : 4096) + n + 2;
        char *grown = realloc(acc->buf, cap);
        if (!grown) {
            return; // keep what we have; bflCliErrorCount still says how many
        }
        acc->buf = grown;
        acc->cap = cap;
    }
    memcpy(acc->buf + acc->len, line, n);
    acc->len += n;
    acc->buf[acc->len++] = '\n';
    acc->buf[acc->len] = '\0';
}

static void accReset(lineAcc_t *acc)
{
    acc->len = 0;
    if (acc->buf) {
        acc->buf[0] = '\0';
    }
}

const char *cpuflight_render_errors(void)
{
    return g_renderErrs.buf ? g_renderErrs.buf : "";
}

// The configuration that actually resulted from the last render, as the
// firmware's own 'dump all' CLI text. The header carries the firmware's
// exact release string; the body carries every setting this build knows,
// with the values it accepted — callers verify against it.
const char *cpuflight_render_dump(void)
{
    return g_renderDump.buf ? g_renderDump.buf : "";
}

static void renderErrSink(const char *line)
{
    if (!strstr(line, "###ERROR") && !strstr(line, "ERR_CMD_NA")) {
        return;
    }
    accAppend(&g_renderErrs, line);
}

static void renderDumpSink(const char *line)
{
    accAppend(&g_renderDump, line);
}

// Render a Betaflight CLI dump ("dump all" / "diff all" text) into a
// boot-ready RAM-eeprom image using THIS build's firmware: boot one
// instance on defaults, apply the text through the real CLI parser, save,
// copy the resulting image out.
//
// Rejected lines never pass silently: every one is collected verbatim in
// cpuflight_render_errors(). With strict != 0 any rejection fails the
// render (first error in cpuflight_error()). With strict == 0 the render
// succeeds and the CALLER must judge the reject list — a hardware dump
// legitimately carries settings a SITL build compiled out (serialrx_*,
// adc_*, dshot_*), but a rejected line can also be real drift (a setting
// this firmware renamed or removed) whose value would otherwise silently
// stay at the compiled default. The Python wrapper compares the list
// against a committed known-rejects snapshot and fails on any difference.
//
// Uses the one-fleet-per-process slot; call before create or after destroy.
int cpuflight_render_eeprom(const char *cli_text, uint32_t text_len, int strict,
                            uint8_t *out, uint32_t out_cap, uint32_t *out_len)
{
    if (!cli_text || !out || !out_len) {
        snprintf(g_err, sizeof(g_err), "render_eeprom: NULL argument");
        return -1;
    }
    if (g_active) {
        snprintf(g_err, sizeof(g_err),
                 "a cpuflight fleet exists in this process; render before create "
                 "or after destroy");
        return -1;
    }

    bflInstanceTemplateFixup();
    if (bflInstanceImageSize() == 0) {
        snprintf(g_err, sizeof(g_err),
                 "this binary is not instanced (build libcpuflight.so via "
                 "tools/lockstep_instancer/build_multi.sh)");
        return -1;
    }
    if (bflInstancesCreate(1) != 0) {
        snprintf(g_err, sizeof(g_err), "instance allocation failed (n=1)");
        return -1;
    }
    g_active = true;
    accReset(&g_renderErrs);
    accReset(&g_renderDump);

    int rc = -1;
    char *san = NULL;
    bflInstanceActivate(0);
    bflSetEepromPath(NULL);     // RAM eeprom: defaults boot, save stays in RAM

    printfSerialInit();
    systemInit();
    initPhase1();
    initPhase2();
    initPhase3();

    uint32_t sanLen = 0;
    san = bflCliSanitizeDump(cli_text, text_len, &sanLen);
    if (!san) {
        snprintf(g_err, sizeof(g_err), "out of memory sanitising CLI dump");
        goto done;
    }

    bflCliSetSink(renderErrSink);
    bflCliExec(san, sanLen);
    bflCliSetSink(NULL);

    if (strict && bflCliErrorCount() > 0) {
        snprintf(g_err, sizeof(g_err), "CLI rejected %u line(s); first: %s",
                 bflCliErrorCount(), bflCliFirstError());
        goto done;
    }

    // round-trip: the saved configuration as this firmware's own CLI text
    // (version header + every accepted value), for caller-side verification
    bflCliSetSink(renderDumpSink);
    static const char dumpCmd[] = "dump all\n";
    bflCliExec(dumpCmd, sizeof(dumpCmd) - 1);
    bflCliSetSink(NULL);

    const uint32_t eeSize = bflEepromSize();
    if (out_cap < eeSize) {
        snprintf(g_err, sizeof(g_err),
                 "output buffer too small: %u < %u", out_cap, eeSize);
        goto done;
    }
    memcpy(out, bflEepromBuffer(), eeSize);
    *out_len = eeSize;
    rc = 0;

done:
    free(san);
    bflInstancesDestroy();
    g_active = false;
    return rc;
}

uint32_t cpuflight_num_envs(cpuflight *g) { return g->n; }
uint32_t cpuflight_act_dim(cpuflight *g)  { (void)g; return ACT_DIM; }
uint32_t cpuflight_aux_dim(cpuflight *g)  { (void)g; return AUX_DIM; }

// One firmware exchange for the whole fleet — the CPU twin of bfFwStep
// (device_flight.c). actions: [n x 4] AETR in [-1,1]; sensors: [n x 7] body
// rates rad/s FRD (3), specific force m/s^2 (3), baro Pa (1); motors out:
// [n x 4] normalised [0,1]; armed out: [n] u8. `substeps` 1 ms steps per call.
int cpuflight_fw_step(cpuflight *g, const float *actions, const float *sensors,
                      float *motors, uint8_t *armed, uint32_t substeps)
{
    for (uint32_t k = 0; k < g->n; k++) {
        bflInstanceActivate(k);
        cpuInstance_t *s = &g->inst[k];

        const float *a = &actions[(size_t)k * ACT_DIM];
        s->rc[0] = (uint16_t)(1500.0f + 500.0f * clamp1(a[0]));
        s->rc[1] = (uint16_t)(1500.0f + 500.0f * clamp1(a[1]));
        s->rc[2] = (uint16_t)(1500.0f + 500.0f * clamp1(a[2]));
        s->rc[3] = (uint16_t)(1500.0f + 500.0f * clamp1(a[3]));

        const float *sn = &sensors[(size_t)k * 7];
        bflSetGyroAccel(sn[0], sn[1], sn[2], sn[3], sn[4], sn[5]);
        bflSetBaro((int32_t)sn[6]);

        for (uint32_t i = 0; i < substeps; i++) {
            bflSetRc(s->rc, BFL_MAX_RC_CHANNELS);
            bflStepUs(CONTROL_STEP_US);
        }

        bflGetMotorsNormalised(&motors[(size_t)k * 4], 4);
        armed[k] = bflIsArmed() ? 1 : 0;
    }
    return 0;
}

// Host-driven AUX channels (arm switch, flight mode) — the CPU twin of
// bfSetAux. aux: [n x 4] RC microsecond values for AUX1..AUX4.
void cpuflight_set_aux(cpuflight *g, const float *aux)
{
    for (uint32_t k = 0; k < g->n; k++) {
        for (int i = 0; i < AUX_DIM && (4 + i) < BFL_MAX_RC_CHANNELS; i++) {
            float us = aux[(size_t)k * AUX_DIM + i];
            us = us < 1000.0f ? 1000.0f : (us > 2000.0f ? 2000.0f : us);
            g->inst[k].rc[4 + i] = (uint16_t)us;
        }
    }
}

// Retake the episode-start snapshot at the current state. CPU blobs never
// move, so a plain byte copy is a complete snapshot (cf. bfSnapshot's
// same-address rationale on the GPU).
int cpuflight_snapshot(cpuflight *g)
{
    for (uint32_t k = 0; k < g->n; k++) {
        char *blob = bflInstanceBlob(k);
        if (!blob) {
            snprintf(g_err, sizeof(g_err), "instance %u has no blob", k);
            return -1;
        }
        memcpy(g->snapBlobs + (size_t)k * g->imageSize, blob, g->imageSize);
        g->snapInst[k] = g->inst[k];
    }
    return 0;
}

// Restore the flagged instances (u8 mask, cudaflight_reset_mask layout) to
// the episode-start snapshot. Same-address restore: no pointer rebasing needed.
int cpuflight_reset_mask(cpuflight *g, const uint8_t *mask)
{
    for (uint32_t k = 0; k < g->n; k++) {
        if (!mask[k]) {
            continue;
        }
        char *blob = bflInstanceBlob(k);
        if (!blob) {
            snprintf(g_err, sizeof(g_err), "instance %u has no blob", k);
            return -1;
        }
        memcpy(blob, g->snapBlobs + (size_t)k * g->imageSize, g->imageSize);
        g->inst[k] = g->snapInst[k];
    }
    return 0;
}

int cpuflight_reset_all(cpuflight *g)
{
    for (uint32_t k = 0; k < g->n; k++) {
        char *blob = bflInstanceBlob(k);
        if (!blob) {
            snprintf(g_err, sizeof(g_err), "instance %u has no blob", k);
            return -1;
        }
        memcpy(blob, g->snapBlobs + (size_t)k * g->imageSize, g->imageSize);
        g->inst[k] = g->snapInst[k];
    }
    return 0;
}
