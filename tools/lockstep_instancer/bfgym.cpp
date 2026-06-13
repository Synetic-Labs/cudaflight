// bfgym: C shared library exposing the GPU Betaflight fleet as an RL
// environment. bfgym_create() boots, settles and arms N firmware
// instances and snapshots them as the episode start state; bfgym_step()
// and the reset calls then operate entirely on device buffers whose
// pointers are exported, so a Python wrapper can map actions / obs /
// rewards / dones as zero-copy CUDA tensors.
//
// Runs in the CUDA *primary* context (cuDevicePrimaryCtxRetain) so the
// buffers live in the same context PyTorch uses. Every entry point is
// synchronous — the context is synchronized before returning — so callers
// may touch the buffers from any stream once a call returns.
//
// Build: g++ -O2 -shared -fPIC bfgym.cpp -I/opt/cuda/include -lcuda

#include <cuda.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

static char g_err[512];

static bool cuOk(CUresult rc, const char *what)
{
    if (rc == CUDA_SUCCESS) {
        return true;
    }
    const char *msg = nullptr;
    cuGetErrorString(rc, &msg);
    snprintf(g_err, sizeof(g_err), "%s: %s (%d)", what, msg ? msg : "?", (int)rc);
    return false;
}

#define CUTRY(call) \
    do { \
        if (!cuOk((call), #call)) { \
            return -1; \
        } \
    } while (0)

struct bfgym {
    CUdevice dev;
    CUcontext ctx;          // primary context, retained
    CUmodule mod;
    CUfunction fInit, fBoot, fRun, fFinish, fSnapshot, fReset, fStep, fFwStep;
    CUfunction fOsd;
    uint32_t n;
    unsigned grid, block;
    uint64_t imageSize, stride, stateSize, actDim, obsDim;
    uint64_t osdRows, osdCols;
    CUdeviceptr instBuf, stateBuf, snapBuf, snapStBuf;
    CUdeviceptr actBuf, obsBuf, rewBuf, doneBuf;
    CUdeviceptr hashBuf, altBuf, armedBuf;
    CUdeviceptr sensBuf, motorBuf;  // external-physics exchange: [n x 7] in, [n x 4] out
    CUdeviceptr osdBuf, osdAttrBuf; // OSD char grids: [n x rows*cols] u8 each
};

// The primary context may not be current on the calling thread (or torch
// may have it current already — pushing the same context again is fine).
struct CtxGuard {
    explicit CtxGuard(CUcontext c) { cuCtxPushCurrent(c); }
    ~CtxGuard()
    {
        CUcontext dummy;
        cuCtxPopCurrent(&dummy);
    }
};

static int launch(bfgym *g, CUfunction f, void **args)
{
    // Entry fence: JAX/XLA (and torch on non-default streams) enqueue
    // reads of our buffers on non-blocking streams, which the NULL
    // stream does not implicitly order against.
    CUTRY(cuCtxSynchronize());
    CUTRY(cuLaunchKernel(f, g->grid, 1, 1, g->block, 1, 1, 0, 0, args, nullptr));
    CUTRY(cuCtxSynchronize());
    return 0;
}

// Chunked so long sim stretches survive display-GPU kernel watchdogs.
static int runMs(bfgym *g, unsigned ms)
{
    const unsigned chunk = 250;
    for (unsigned done = 0; done < ms;) {
        unsigned step = ms - done < chunk ? ms - done : chunk;
        void *args[] = { &g->stateBuf, &step };
        if (launch(g, g->fRun, args)) {
            return -1;
        }
        done += step;
    }
    return 0;
}

static uint64_t readU64(bfgym *g, const char *name, int *err)
{
    CUdeviceptr p;
    size_t sz;
    uint64_t v = 0;
    if (!cuOk(cuModuleGetGlobal(&p, &sz, g->mod, name), name) ||
        !cuOk(cuMemcpyDtoH(&v, p, sizeof(v)), name)) {
        *err = 1;
    }
    return v;
}

static int writeGlobal(bfgym *g, const char *name, const void *src, size_t len)
{
    CUdeviceptr p;
    size_t sz;
    CUTRY(cuModuleGetGlobal(&p, &sz, g->mod, name));
    CUTRY(cuMemcpyHtoD(p, src, len));
    return 0;
}

extern "C" {

const char *bfgym_error(void)
{
    return g_err;
}

void bfgym_destroy(bfgym *g)
{
    if (!g) {
        return;
    }
    cuCtxPushCurrent(g->ctx);
    for (CUdeviceptr p : { g->instBuf, g->stateBuf, g->snapBuf, g->snapStBuf,
                           g->actBuf, g->obsBuf, g->rewBuf, g->doneBuf,
                           g->hashBuf, g->altBuf, g->armedBuf,
                           g->sensBuf, g->motorBuf, g->osdBuf, g->osdAttrBuf }) {
        if (p) {
            cuMemFree(p);
        }
    }
    if (g->mod) {
        cuModuleUnload(g->mod);
    }
    CUcontext dummy;
    cuCtxPopCurrent(&dummy);
    cuDevicePrimaryCtxRelease(g->dev);
    delete g;
}

// Boot, settle and arm n instances, snapshot the armed state as the
// episode start. settle_ms == 0 means the default 7000 (6s gyro
// calibration + arming grace, 1s armed on the ground), matching the
// validated harness schedule. eeprom_path optionally names a boot-ready
// config image (from the CPU harness's --cli-dump converter) preloaded
// into every instance's RAM EEPROM before boot, so the fleet flies that
// configuration instead of defaults. Returns NULL on failure (see
// bfgym_error).
bfgym *bfgym_create_eeprom(const char *cubin_path, uint32_t n, int device, uint32_t settle_ms,
                           const char *eeprom_path)
{
    if (settle_ms == 0) {
        settle_ms = 7000;
    }
    bfgym *g = new bfgym();
    memset(g, 0, sizeof(*g));
    g->n = n;
    g->block = 32;
    g->grid = (n + g->block - 1) / g->block;

    if (!cuOk(cuInit(0), "cuInit") ||
        !cuOk(cuDeviceGet(&g->dev, device), "cuDeviceGet") ||
        !cuOk(cuDevicePrimaryCtxRetain(&g->ctx, g->dev), "cuDevicePrimaryCtxRetain")) {
        delete g;
        return nullptr;
    }

    CtxGuard guard(g->ctx);

    // The whole firmware runs on one thread's stack. The driver reserves
    // stack for the device-wide max resident thread count, so this can't
    // be extravagant: 32 KB ≈ 8 GB reserved on a 5090.
    if (!cuOk(cuCtxSetLimit(CU_LIMIT_STACK_SIZE, 32 * 1024), "cuCtxSetLimit") ||
        !cuOk(cuModuleLoad(&g->mod, cubin_path), "cuModuleLoad")) {
        bfgym_destroy(g);
        return nullptr;
    }

    int err = 0;
    g->imageSize = readU64(g, "__bf_image_size", &err);
    uint64_t align = readU64(g, "__bf_image_align", &err);
    g->stateSize = readU64(g, "__bf_state_size", &err);
    g->actDim = readU64(g, "__bf_act_dim", &err);
    g->obsDim = readU64(g, "__bf_obs_dim", &err);
    g->osdRows = readU64(g, "__bf_osd_rows", &err);
    g->osdCols = readU64(g, "__bf_osd_cols", &err);
    if (err) {
        bfgym_destroy(g);
        return nullptr;
    }
    if (align < 256) {
        align = 256; // cuMemAlloc returns >=256-aligned; keep deltas congruent
    }
    g->stride = (g->imageSize + align - 1) & ~(align - 1);

    bool ok =
        cuOk(cuMemAlloc(&g->instBuf, g->stride * n), "alloc instances") &&
        cuOk(cuMemAlloc(&g->snapBuf, g->stride * n), "alloc snapshot") &&
        cuOk(cuMemAlloc(&g->stateBuf, g->stateSize * n), "alloc state") &&
        cuOk(cuMemAlloc(&g->snapStBuf, g->stateSize * n), "alloc snap state") &&
        cuOk(cuMemAlloc(&g->actBuf, 4 * g->actDim * n), "alloc actions") &&
        cuOk(cuMemAlloc(&g->obsBuf, 4 * g->obsDim * n), "alloc obs") &&
        cuOk(cuMemAlloc(&g->rewBuf, 4 * n), "alloc rewards") &&
        cuOk(cuMemAlloc(&g->doneBuf, n), "alloc dones") &&
        cuOk(cuMemAlloc(&g->hashBuf, 8 * n), "alloc hashes") &&
        cuOk(cuMemAlloc(&g->altBuf, 4 * n), "alloc alt") &&
        cuOk(cuMemAlloc(&g->armedBuf, n), "alloc armed") &&
        cuOk(cuMemAlloc(&g->sensBuf, 4 * 7 * n), "alloc sensors") &&
        cuOk(cuMemAlloc(&g->motorBuf, 4 * 4 * n), "alloc motors") &&
        cuOk(cuMemAlloc(&g->osdBuf, g->osdRows * g->osdCols * n), "alloc osd") &&
        cuOk(cuMemAlloc(&g->osdAttrBuf, g->osdRows * g->osdCols * n), "alloc osd attrs") &&
        cuOk(cuMemsetD8(g->osdBuf, 0x20, g->osdRows * g->osdCols * n), "blank osd") &&
        cuOk(cuMemsetD8(g->osdAttrBuf, 0, g->osdRows * g->osdCols * n), "zero osd attrs") &&
        cuOk(cuMemsetD8(g->actBuf, 0, 4 * g->actDim * n), "zero actions") &&
        cuOk(cuMemsetD8(g->obsBuf, 0, 4 * g->obsDim * n), "zero obs") &&
        cuOk(cuMemsetD8(g->rewBuf, 0, 4 * n), "zero rewards") &&
        cuOk(cuMemsetD8(g->doneBuf, 0, n), "zero dones") &&
        cuOk(cuMemsetD8(g->sensBuf, 0, 4 * 7 * n), "zero sensors") &&
        cuOk(cuMemsetD8(g->motorBuf, 0, 4 * 4 * n), "zero motors");
    if (!ok) {
        bfgym_destroy(g);
        return nullptr;
    }

    char *base = (char *)g->instBuf;
    if (writeGlobal(g, "__bf_inst_base", &base, sizeof(base)) ||
        writeGlobal(g, "__bf_inst_stride", &g->stride, sizeof(g->stride)) ||
        writeGlobal(g, "__bf_inst_count", &n, sizeof(n))) {
        bfgym_destroy(g);
        return nullptr;
    }

    ok = cuOk(cuModuleGetFunction(&g->fInit, g->mod, "bfInstanceInit"), "bfInstanceInit") &&
         cuOk(cuModuleGetFunction(&g->fBoot, g->mod, "bfBoot"), "bfBoot") &&
         cuOk(cuModuleGetFunction(&g->fRun, g->mod, "bfRun"), "bfRun") &&
         cuOk(cuModuleGetFunction(&g->fFinish, g->mod, "bfFinish"), "bfFinish") &&
         cuOk(cuModuleGetFunction(&g->fSnapshot, g->mod, "bfSnapshot"), "bfSnapshot") &&
         cuOk(cuModuleGetFunction(&g->fReset, g->mod, "bfReset"), "bfReset") &&
         cuOk(cuModuleGetFunction(&g->fStep, g->mod, "bfStep"), "bfStep") &&
         cuOk(cuModuleGetFunction(&g->fFwStep, g->mod, "bfFwStep"), "bfFwStep") &&
         cuOk(cuModuleGetFunction(&g->fOsd, g->mod, "bfOsdSnapshot"), "bfOsdSnapshot");
    if (!ok) {
        bfgym_destroy(g);
        return nullptr;
    }

    uint32_t perturb = UINT32_MAX;
    void *initArgs[] = { &g->stateBuf, &perturb };
    void *bootArgs[] = { &g->stateBuf };
    void *snapArgs[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf };
    if (launch(g, g->fInit, initArgs)) {
        bfgym_destroy(g);
        return nullptr;
    }

    if (eeprom_path) {
        FILE *f = fopen(eeprom_path, "rb");
        if (!f) {
            snprintf(g_err, sizeof(g_err), "cannot open eeprom image '%s'", eeprom_path);
            bfgym_destroy(g);
            return nullptr;
        }
        fseek(f, 0, SEEK_END);
        const uint64_t eeLen = (uint64_t)ftell(f);
        rewind(f);
        std::vector<uint8_t> ee(eeLen);
        const bool readOk = fread(ee.data(), 1, eeLen, f) == eeLen;
        fclose(f);
        if (!readOk) {
            snprintf(g_err, sizeof(g_err), "failed to read eeprom image '%s'", eeprom_path);
            bfgym_destroy(g);
            return nullptr;
        }

        CUfunction fLoadEeprom;
        CUdeviceptr eeBuf = 0;
        uint64_t len = eeLen;
        uint32_t perInstance = 0;
        void *eeArgs[] = { &eeBuf, &len, &perInstance };
        const bool ok =
            cuOk(cuModuleGetFunction(&fLoadEeprom, g->mod, "bfLoadEeprom"), "bfLoadEeprom") &&
            cuOk(cuMemAlloc(&eeBuf, eeLen), "alloc eeprom") &&
            cuOk(cuMemcpyHtoD(eeBuf, ee.data(), eeLen), "upload eeprom") &&
            launch(g, fLoadEeprom, eeArgs) == 0;
        if (eeBuf) {
            cuMemFree(eeBuf);
        }
        if (!ok) {
            bfgym_destroy(g);
            return nullptr;
        }
    }

    if (launch(g, g->fBoot, bootArgs) ||
        runMs(g, settle_ms)) {
        bfgym_destroy(g);
        return nullptr;
    }

    // Every instance must be armed at the snapshot point, or episodes
    // would start dead.
    void *finArgs[] = { &g->stateBuf, &g->hashBuf, &g->altBuf, &g->armedBuf };
    std::vector<uint8_t> armed(n);
    if (launch(g, g->fFinish, finArgs) ||
        !cuOk(cuMemcpyDtoH(armed.data(), g->armedBuf, n), "read armed")) {
        bfgym_destroy(g);
        return nullptr;
    }
    for (uint32_t k = 0; k < n; k++) {
        if (!armed[k]) {
            snprintf(g_err, sizeof(g_err), "instance %u failed to arm during create", k);
            bfgym_destroy(g);
            return nullptr;
        }
    }

    if (launch(g, g->fSnapshot, snapArgs)) {
        bfgym_destroy(g);
        return nullptr;
    }
    return g;
}

// Default-config create, kept for ABI compatibility.
bfgym *bfgym_create(const char *cubin_path, uint32_t n, int device, uint32_t settle_ms)
{
    return bfgym_create_eeprom(cubin_path, n, device, settle_ms, nullptr);
}

uint32_t bfgym_num_envs(bfgym *g) { return g->n; }
uint32_t bfgym_act_dim(bfgym *g) { return (uint32_t)g->actDim; }
uint32_t bfgym_obs_dim(bfgym *g) { return (uint32_t)g->obsDim; }

// Device pointers for zero-copy tensor wrapping.
uint64_t bfgym_actions_ptr(bfgym *g) { return (uint64_t)g->actBuf; }
uint64_t bfgym_obs_ptr(bfgym *g) { return (uint64_t)g->obsBuf; }
uint64_t bfgym_rewards_ptr(bfgym *g) { return (uint64_t)g->rewBuf; }
uint64_t bfgym_dones_ptr(bfgym *g) { return (uint64_t)g->doneBuf; }

// Apply the actions buffer for `decimation` 1ms control steps, then write
// obs / rewards / dones. decimation == 0 refreshes obs without advancing
// the sim (used for the post-reset observation).
int bfgym_step(bfgym *g, uint32_t decimation)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->actBuf, &g->obsBuf, &g->rewBuf, &g->doneBuf, &decimation };
    return launch(g, g->fStep, args);
}

// Restore instances flagged in a device uint8[n] mask to the snapshot.
int bfgym_reset_mask(bfgym *g, uint64_t mask_devptr)
{
    CtxGuard guard(g->ctx);
    CUdeviceptr mask = (CUdeviceptr)mask_devptr;
    void *args[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf, &mask };
    return launch(g, g->fReset, args);
}

// Auto-reset composition: the dones buffer doubles as the reset mask.
int bfgym_reset_done(bfgym *g)
{
    return bfgym_reset_mask(g, (uint64_t)g->doneBuf);
}

int bfgym_reset_all(bfgym *g)
{
    CtxGuard guard(g->ctx);
    // reuse armedBuf as an all-ones mask; it is 1 everywhere after create
    // but may be stale, so rewrite it
    CUTRY(cuMemsetD8(g->armedBuf, 1, g->n));
    return bfgym_reset_mask(g, (uint64_t)g->armedBuf);
}

// Retake the episode-start snapshot at the current state (curriculum /
// domain randomization hooks).
int bfgym_snapshot(bfgym *g)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->snapStBuf, &g->snapBuf };
    return launch(g, g->fSnapshot, args);
}

// Copy actions from another device buffer (float32 [n, act_dim]) into the
// actions buffer — for frameworks with immutable arrays (JAX) that cannot
// write into our buffer in place. Caller must ensure the source is fully
// written (e.g. block_until_ready) before calling.
int bfgym_write_actions(bfgym *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoD(g->actBuf, (CUdeviceptr)src_devptr, 4 * g->actDim * g->n));
    CUTRY(cuCtxSynchronize()); // source may be freed/reused once we return
    return 0;
}

// Host fallback for the same.
int bfgym_write_actions_host(bfgym *g, const float *src)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyHtoD(g->actBuf, src, 4 * g->actDim * g->n));
    return 0;
}

// Drain all device work (every stream in the context).
int bfgym_sync(bfgym *g)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuCtxSynchronize());
    return 0;
}

// ---------------------------------------------------------------------------
// External-physics mode: the firmware advances against sensor values from an
// outside simulator, which in turn consumes the firmware's motor outputs.
// Per 1ms control step: write sensors -> bfgym_fw_step(1) -> read motors.
// The in-kernel physics is bypassed (its state goes stale); obs/reward/done
// are the external simulator's business.

// Device pointers for the exchange buffers.
uint64_t bfgym_sensors_ptr(bfgym *g) { return (uint64_t)g->sensBuf; }
uint64_t bfgym_motors_ptr(bfgym *g) { return (uint64_t)g->motorBuf; }
uint64_t bfgym_armed_ptr(bfgym *g) { return (uint64_t)g->armedBuf; }

// Copy sensors (float32 [n, 7]: gyro NED rad/s, specific force NED m/s^2,
// baro Pa) from another device buffer. Caller must ensure the source is
// fully written before calling.
int bfgym_write_sensors(bfgym *g, uint64_t src_devptr)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyDtoD(g->sensBuf, (CUdeviceptr)src_devptr, 4 * 7 * g->n));
    CUTRY(cuCtxSynchronize()); // source may be freed/reused once we return
    return 0;
}

// Host fallback for the same.
int bfgym_write_sensors_host(bfgym *g, const float *src)
{
    CtxGuard guard(g->ctx);
    CUTRY(cuMemcpyHtoD(g->sensBuf, src, 4 * 7 * g->n));
    return 0;
}

// Advance the firmware `substeps` 1ms control steps against the current
// sensors and actions buffers; writes normalised motor outputs [n, 4] and
// the armed flags [n].
int bfgym_fw_step(bfgym *g, uint32_t substeps)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->actBuf, &g->sensBuf, &g->motorBuf,
                     &g->armedBuf, &substeps };
    return launch(g, g->fFwStep, args);
}

// ---------------------------------------------------------------------------
// OSD readback. The firmware draws its OSD into per-instance character
// grids as part of normal stepping; bfgym_osd_update() snapshots every
// instance's grid (and displayport attributes: severity bits + blink in
// bit 7) into the exported [n, rows*cols] uint8 device buffers, which a
// renderer maps as zero-copy tensors.

uint32_t bfgym_osd_rows(bfgym *g) { return (uint32_t)g->osdRows; }
uint32_t bfgym_osd_cols(bfgym *g) { return (uint32_t)g->osdCols; }
uint64_t bfgym_osd_ptr(bfgym *g) { return (uint64_t)g->osdBuf; }
uint64_t bfgym_osd_attrs_ptr(bfgym *g) { return (uint64_t)g->osdAttrBuf; }

int bfgym_osd_update(bfgym *g)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->osdBuf, &g->osdAttrBuf };
    return launch(g, g->fOsd, args);
}

// ---------------------------------------------------------------------------
// Raw launch parameters for external launchers (e.g. an XLA FFI handler
// that runs bfFwStep inside a jitted JAX program, on XLA's stream, with
// no synchronization). The caller owns stream ordering; the kernel only
// touches the firmware blobs plus whatever buffers the caller passes.

uint64_t bfgym_fw_step_kernel(bfgym *g) { return (uint64_t)g->fFwStep; }
uint64_t bfgym_state_ptr(bfgym *g) { return (uint64_t)g->stateBuf; }
uint64_t bfgym_ctx(bfgym *g) { return (uint64_t)g->ctx; }
uint32_t bfgym_grid(bfgym *g) { return g->grid; }
uint32_t bfgym_block(bfgym *g) { return g->block; }

// Reset kernel (bfReset) launch params, for an in-jit masked reset FFI.
// bfReset(state, snapSt, snap, mask): restores masked instances to the
// snapshot. Same grid/block/ctx as the step kernel above.
uint64_t bfgym_reset_kernel(bfgym *g) { return (uint64_t)g->fReset; }
uint64_t bfgym_snap_state_ptr(bfgym *g) { return (uint64_t)g->snapStBuf; }
uint64_t bfgym_snap_ptr(bfgym *g) { return (uint64_t)g->snapBuf; }

// Motor-trace hashes (host out, n entries) — the determinism oracle.
int bfgym_hashes(bfgym *g, uint64_t *out)
{
    CtxGuard guard(g->ctx);
    void *args[] = { &g->stateBuf, &g->hashBuf, &g->altBuf, &g->armedBuf };
    if (launch(g, g->fFinish, args)) {
        return -1;
    }
    CUTRY(cuMemcpyDtoH(out, g->hashBuf, 8 * g->n));
    return 0;
}

} // extern "C"
