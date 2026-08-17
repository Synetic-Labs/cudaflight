// SPDX-License-Identifier: GPL-3.0-or-later
// XLA FFI handler that launches the Betaflight firmware kernel
// (bfFwStep) inside a jitted JAX program, on XLA's compute stream.
//
// This is the fusion keystone: the whole exchange loop
// (sensors -> firmware 1ms -> motors -> physics) becomes ONE XLA
// program with zero host synchronization per substep — the firmware
// kernel is just another node on the stream, ordered by its buffer
// dependencies like any other op.
//
// The launch parameters (CUfunction, instance-state pointer, grid,
// block, primary context) come from libcudaflight via integer attributes,
// so this translation unit needs no cudaflight internals — only the CUDA
// driver API and the XLA FFI headers.
//
// Build (see tools/lockstep_instancer/python/Makefile, target `shim`):
//   g++ -std=c++17 -O2 -shared -fPIC -I$(jax.ffi.include_dir())
//       cudaflight_xla.cpp -lcuda -o libcudaflight_xla.so
//
// No CUDA *toolkit* headers needed: the shim uses only a handful of driver-API
// symbols, vendored below, and links libcuda (the NVIDIA driver lib, present on
// any GPU box). This drops the `cuda.h` / CUDA_HOME build dependency entirely.

// -- minimal CUDA driver API (Linux x86_64), in place of <cuda.h> ----------
// Opaque handles + the 4 driver calls this file uses. Types/signatures are
// ABI-stable (CUDA Driver API reference). We pass an EXPLICIT stream to
// cuLaunchKernel, so the plain (legacy-stream) symbol is correct — no need for
// the per-thread-default-stream (_ptsz) variant that cuda.h remaps to.
extern "C" {
typedef struct CUctx_st*    CUcontext;
typedef struct CUfunc_st*   CUfunction;
typedef struct CUstream_st* CUstream;
typedef CUstream            cudaStream_t;   // runtime stream == driver stream (opaque)
typedef unsigned long long  CUdeviceptr;    // 64-bit
typedef int                 CUresult;       // enum in cuda.h; int is ABI-compatible
#define CUDA_SUCCESS 0
CUresult cuCtxGetCurrent(CUcontext*);
CUresult cuCtxSetCurrent(CUcontext);
CUresult cuGetErrorString(CUresult, const char**);
CUresult cuLaunchKernel(CUfunction f,
    unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ,
    unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ,
    unsigned int sharedMemBytes, CUstream hStream,
    void** kernelParams, void** extra);
}

#include <cstdint>
#include <string>

#include "xla/ffi/api/ffi.h"

namespace ffi = xla::ffi;

static ffi::Error BfFwStepImpl(
    cudaStream_t stream,
    int64_t fn, int64_t state, int64_t cuctx,
    int64_t grid, int64_t block, int64_t substeps,
    ffi::Buffer<ffi::F32> actions, ffi::Buffer<ffi::F32> sensors,
    ffi::Result<ffi::Buffer<ffi::F32>> motors,
    ffi::Result<ffi::Buffer<ffi::U8>> armed)
{
    // XLA's callback thread may not have the primary context current
    // (it drives CUDA through the runtime API); the driver-API launch
    // needs one.
    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    void *act = actions.untyped_data();
    void *sens = sensors.untyped_data();
    void *mot = motors->untyped_data();
    void *arm = armed->untyped_data();
    uint32_t sub = (uint32_t)substeps;
    void *st = (void *)state;
    void *args[] = { &st, &act, &sens, &mot, &arm, &sub };

    const CUresult rc = cuLaunchKernel(
        (CUfunction)fn, (unsigned)grid, 1, 1, (unsigned)block, 1, 1,
        0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfFwStep launch failed: ")
                          + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStep, BfFwStepImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("fn")
        .Attr<int64_t>("state")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<int64_t>("substeps")
        .Arg<ffi::Buffer<ffi::F32>>()   // actions [N, 4]
        .Arg<ffi::Buffer<ffi::F32>>()   // sensors [N, 7]
        .Ret<ffi::Buffer<ffi::F32>>()   // motors [N, 4]
        .Ret<ffi::Buffer<ffi::U8>>());  // armed [N]


// Masked reset (bfReset) on XLA's stream: restores instances flagged in the
// uint8[N] mask to the episode-start snapshot. Pure side effect (mutates the
// firmware blobs), no results — has_side_effect keeps it and its order.
static ffi::Error BfResetImpl(
    cudaStream_t stream,
    int64_t fn, int64_t state, int64_t snapst, int64_t snap, int64_t cuctx,
    int64_t grid, int64_t block,
    ffi::Buffer<ffi::U8> mask)
{
    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    void *st = (void *)state, *ss = (void *)snapst, *sn = (void *)snap;
    void *m = mask.untyped_data();
    void *args[] = { &st, &ss, &sn, &m };

    const CUresult rc = cuLaunchKernel(
        (CUfunction)fn, (unsigned)grid, 1, 1, (unsigned)block, 1, 1,
        0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfReset launch failed: ")
                          + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfReset, BfResetImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("fn")
        .Attr<int64_t>("state")
        .Attr<int64_t>("snapst")
        .Attr<int64_t>("snap")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Arg<ffi::Buffer<ffi::U8>>());  // mask [N]


// Finite-difference Jacobian of the real control law on XLA's stream:
// bfFwStepJacFD(actions[N,4], jac[N,16], scratch, eps) writes
// jac[k][i][d] = d(motor_i)/d(action_d) at the current per-instance state.
// Used by the firmware-step custom_vjp: J is computed in the forward pass and
// contracted (J^T . cotangent) in pure JAX in the backward pass.
static ffi::Error BfFwStepJacFDImpl(
    cudaStream_t stream,
    int64_t fn, int64_t scratch, int64_t cuctx,
    int64_t grid, int64_t block, float eps,
    ffi::Buffer<ffi::F32> actions,
    ffi::Result<ffi::Buffer<ffi::F32>> jac)
{
    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    void *act = actions.untyped_data();
    void *j = jac->untyped_data();
    void *sc = (void *)scratch;
    float e = eps;
    void *args[] = { &act, &j, &sc, &e };

    const CUresult rc = cuLaunchKernel(
        (CUfunction)fn, (unsigned)grid, 1, 1, (unsigned)block, 1, 1,
        0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfFwStepJacFD launch failed: ")
                          + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStepJacFD, BfFwStepJacFDImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("fn")
        .Attr<int64_t>("scratch")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<float>("eps")
        .Arg<ffi::Buffer<ffi::F32>>()    // actions [N, 4]
        .Ret<ffi::Buffer<ffi::F32>>());  // jac [N, 16]


// Enzyme reverse-mode VJP of the real control law on XLA's stream:
// bfFwStepGrad(actions[N,4], seedMotors[N,4], dActions[N,4]) computes, per
// instance, dActions = J^T . seedMotors at the current per-instance state,
// where J[i][d] = d(motor_i)/d(action_d). Unlike the FD Jacobian this is a
// single reverse-mode sweep (no per-column re-evaluation, no state scratch):
// the firmware globals are read-only here (config/state frozen), so the call
// is pure w.r.t. the instance blobs — only actions/seed in, dActions out.
static ffi::Error BfFwStepGradImpl(
    cudaStream_t stream,
    int64_t fn, int64_t cuctx, int64_t grid, int64_t block, int64_t scratch,
    ffi::Buffer<ffi::F32> actions, ffi::Buffer<ffi::F32> seed,
    ffi::Result<ffi::Buffer<ffi::F32>> dactions)
{
    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    void *act = actions.untyped_data();
    void *sd = seed.untyped_data();
    void *da = dactions->untyped_data();
    // bfFwStepGrad saves/restores the per-instance blob through this scratch so
    // each call evaluates at the same frozen state (no filter drift); same
    // buffer the FD Jacobian uses.
    CUdeviceptr sc = (CUdeviceptr)scratch;
    void *args[] = { &act, &sd, &da, &sc };

    const CUresult rc = cuLaunchKernel(
        (CUfunction)fn, (unsigned)grid, 1, 1, (unsigned)block, 1, 1,
        0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfFwStepGrad launch failed: ")
                          + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStepGrad, BfFwStepGradImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("fn")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<int64_t>("scratch")
        .Arg<ffi::Buffer<ffi::F32>>()    // actions     [N, 4]
        .Arg<ffi::Buffer<ffi::F32>>()    // seedMotors  [N, 4]
        .Ret<ffi::Buffer<ffi::F32>>());  // dActions    [N, 4]


// ===========================================================================
// Pure (value-threaded) firmware step. The firmware blob and the bfFlight_t
// state are donated JAX buffers (input==output via input_output_aliases), not
// a hidden handle: the whole firmware state flows through the computation as
// values. Because the blob's device address is chosen by XLA, a 1-thread
// bfSetBase launch first points the global instance base at it (stream-ordered
// ahead of the step); the step kernel's rebase-on-entry then fixes the blob's
// self-pointers for that address. Ordering comes from the blob data dependency,
// so no has_side_effect is needed.
static ffi::Error BfFwStepPureImpl(
    cudaStream_t stream,
    int64_t set_base_fn, int64_t step_fn, int64_t cuctx,
    int64_t grid, int64_t block, int64_t substeps,
    ffi::Buffer<ffi::U8> blob, ffi::Buffer<ffi::U8> fwstate,
    ffi::Buffer<ffi::F32> actions, ffi::Buffer<ffi::F32> sensors,
    ffi::Result<ffi::Buffer<ffi::U8>> blob_out,
    ffi::Result<ffi::Buffer<ffi::U8>> fwstate_out,
    ffi::Result<ffi::Buffer<ffi::F32>> motors,
    ffi::Result<ffi::Buffer<ffi::U8>> armed)
{
    (void)blob_out;     // aliased to blob (in place); kernel writes through it
    (void)fwstate_out;  // aliased to fwstate (in place)

    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    // 1) point the instance base at the (XLA-placed) blob buffer
    char *base = (char *)blob.untyped_data();
    void *sbargs[] = { &base };
    CUresult rc = cuLaunchKernel((CUfunction)set_base_fn, 1, 1, 1, 1, 1, 1,
                                 0, (CUstream)stream, sbargs, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfSetBase launch failed: ") + (msg ? msg : "?"));
    }

    // 2) the firmware step, threading the bfFlight_t state buffer as `st`
    void *st = fwstate.untyped_data();
    void *act = actions.untyped_data();
    void *sens = sensors.untyped_data();
    void *mot = motors->untyped_data();
    void *arm = armed->untyped_data();
    uint32_t sub = (uint32_t)substeps;
    void *args[] = { &st, &act, &sens, &mot, &arm, &sub };
    rc = cuLaunchKernel((CUfunction)step_fn, (unsigned)grid, 1, 1,
                        (unsigned)block, 1, 1, 0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfFwStep(pure) launch failed: ") + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStepPure, BfFwStepPureImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("set_base_fn")
        .Attr<int64_t>("step_fn")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<int64_t>("substeps")
        .Arg<ffi::Buffer<ffi::U8>>()    // blob       [N*stride]
        .Arg<ffi::Buffer<ffi::U8>>()    // fwstate    [N*stateSize]
        .Arg<ffi::Buffer<ffi::F32>>()   // actions    [N, 4]
        .Arg<ffi::Buffer<ffi::F32>>()   // sensors    [N, 7]
        .Ret<ffi::Buffer<ffi::U8>>()    // blob_out   (alias 0)
        .Ret<ffi::Buffer<ffi::U8>>()    // fwstate_out(alias 1)
        .Ret<ffi::Buffer<ffi::F32>>()   // motors     [N, 4]
        .Ret<ffi::Buffer<ffi::U8>>(),   // armed      [N]
    {ffi::Traits::kCmdBufferCompatible});  // stream-only -> CUDA-graph capturable


// Pure masked reset: restore the flagged instances in the threaded blob +
// bfFlight_t state to the episode-start snapshot (the handle's snapshot buffers,
// passed as read-only baked pointers). blob/fwstate are donated (in==out); the
// snapshot's pointers are based at the original instBuf, so the restored
// instances carry blobBase != their new address and the next step's
// rebase-on-entry fixes them. Pure: ordering rides the blob data dependency.
static ffi::Error BfResetPureImpl(
    cudaStream_t stream,
    int64_t set_base_fn, int64_t reset_fn, int64_t snapst, int64_t snap,
    int64_t cuctx, int64_t grid, int64_t block,
    ffi::Buffer<ffi::U8> blob, ffi::Buffer<ffi::U8> fwstate, ffi::Buffer<ffi::U8> mask,
    ffi::Result<ffi::Buffer<ffi::U8>> blob_out,
    ffi::Result<ffi::Buffer<ffi::U8>> fwstate_out)
{
    (void)blob_out;
    (void)fwstate_out;

    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    char *base = (char *)blob.untyped_data();
    void *sbargs[] = { &base };
    CUresult rc = cuLaunchKernel((CUfunction)set_base_fn, 1, 1, 1, 1, 1, 1,
                                 0, (CUstream)stream, sbargs, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfSetBase(reset) launch failed: ") + (msg ? msg : "?"));
    }

    void *st = fwstate.untyped_data();
    void *ss = (void *)snapst, *sn = (void *)snap;
    void *m = mask.untyped_data();
    void *args[] = { &st, &ss, &sn, &m };
    rc = cuLaunchKernel((CUfunction)reset_fn, (unsigned)grid, 1, 1,
                        (unsigned)block, 1, 1, 0, (CUstream)stream, args, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfReset(pure) launch failed: ") + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfResetPure, BfResetPureImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("set_base_fn")
        .Attr<int64_t>("reset_fn")
        .Attr<int64_t>("snapst")
        .Attr<int64_t>("snap")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Arg<ffi::Buffer<ffi::U8>>()    // blob       [N*stride]
        .Arg<ffi::Buffer<ffi::U8>>()    // fwstate    [N*stateSize]
        .Arg<ffi::Buffer<ffi::U8>>()    // mask       [N]
        .Ret<ffi::Buffer<ffi::U8>>()    // blob_out   (alias 0)
        .Ret<ffi::Buffer<ffi::U8>>(),   // fwstate_out(alias 1)
    {ffi::Traits::kCmdBufferCompatible});  // stream-only -> CUDA-graph capturable


// ===========================================================================
// Value-threaded ("pure") control-law Jacobian for the differentiable rollout.
// The custom_vjp's forward needs J[k][i][d] = d(motor_i)/d(action_d) at the
// PRE-step state; here that state lives in the donated blob JAX buffer, so —
// like bfFwStepPure — a 1-thread bfSetBase first points the global instance base
// at the blob, then the Jacobian kernel rebases-on-entry and fills jac[N,16].
// The blob is aliased in==out: the rebase rewrites its self-pointers, so XLA
// must treat it as written (and thereby order this ahead of the firmware step
// that consumes the same buffer). Two flavours share this shape:
//   BfFwStepJacFDPure   — finite-difference kernel, takes an eps attr
//   BfFwStepJacGradPure — Enzyme reverse-mode kernel, no eps
static ffi::Error BfStepJacPureLaunch(
    cudaStream_t stream, int64_t set_base_fn, int64_t jac_fn, int64_t cuctx,
    int64_t grid, int64_t block, int64_t scratch, const float *eps, int sel,
    ffi::Buffer<ffi::U8> blob, ffi::Buffer<ffi::F32> actions,
    ffi::Result<ffi::Buffer<ffi::U8>> blob_out,
    ffi::Result<ffi::Buffer<ffi::F32>> jac)
{
    (void)blob_out;   // aliased to blob (the rebase writes through it in place)

    CUcontext cur = nullptr;
    cuCtxGetCurrent(&cur);
    if (cur == nullptr) {
        cuCtxSetCurrent((CUcontext)cuctx);
    }

    // 1) point the instance base at the (XLA-placed) blob buffer
    char *base = (char *)blob.untyped_data();
    void *sbargs[] = { &base };
    CUresult rc = cuLaunchKernel((CUfunction)set_base_fn, 1, 1, 1, 1, 1, 1,
                                 0, (CUstream)stream, sbargs, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfSetBase(jac) launch failed: ") + (msg ? msg : "?"));
    }

    // 2) the Jacobian kernel: bf...JacFDPure(actions, jacOut, scratch, eps) or
    //    bf...JacGradPure(actions, jacOut, scratch). eps is appended only when
    //    non-null, so the same launcher serves both kernel signatures.
    void *act = actions.untyped_data();
    void *j = jac->untyped_data();
    CUdeviceptr sc = (CUdeviceptr)scratch;
    float e = eps ? *eps : 0.0f;
    int sel32 = sel;
    // sel = FD column for bfFwStepJacFDPure(actions, jac, scratch, eps, col), or
    // = ncols for bfFwStepJacGradPure(actions, jac, scratch, ncols).
    void *argsFD[]  = { &act, &j, &sc, &e, &sel32 };
    void *argsEnz[] = { &act, &j, &sc, &sel32 };
    rc = cuLaunchKernel((CUfunction)jac_fn, (unsigned)grid, 1, 1,
                        (unsigned)block, 1, 1, 0, (CUstream)stream,
                        eps ? argsFD : argsEnz, nullptr);
    if (rc != CUDA_SUCCESS) {
        const char *msg = nullptr;
        cuGetErrorString(rc, &msg);
        return ffi::Error(ffi::ErrorCode::kInternal,
                          std::string("bfFwStepJacPure launch failed: ") + (msg ? msg : "?"));
    }
    return ffi::Error::Success();
}

static ffi::Error BfFwStepJacFDPureImpl(
    cudaStream_t stream, int64_t set_base_fn, int64_t jac_fn, int64_t cuctx,
    int64_t grid, int64_t block, int64_t scratch, float eps, int64_t sel,
    ffi::Buffer<ffi::U8> blob, ffi::Buffer<ffi::F32> actions,
    ffi::Result<ffi::Buffer<ffi::U8>> blob_out,
    ffi::Result<ffi::Buffer<ffi::F32>> jac)
{
    return BfStepJacPureLaunch(stream, set_base_fn, jac_fn, cuctx, grid, block,
                               scratch, &eps, (int)sel, blob, actions, blob_out, jac);
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStepJacFDPure, BfFwStepJacFDPureImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("set_base_fn")
        .Attr<int64_t>("jac_fn")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<int64_t>("scratch")
        .Attr<float>("eps")
        .Attr<int64_t>("sel")            // FD column (-1 = all 4, else only `sel`)
        .Arg<ffi::Buffer<ffi::U8>>()     // blob     [N*stride]
        .Arg<ffi::Buffer<ffi::F32>>()    // actions  [N, 4]
        .Ret<ffi::Buffer<ffi::U8>>()     // blob_out (alias 0)
        .Ret<ffi::Buffer<ffi::F32>>(),   // jac      [N, 16]
    {ffi::Traits::kCmdBufferCompatible});  // stream-only -> CUDA-graph capturable

static ffi::Error BfFwStepJacGradPureImpl(
    cudaStream_t stream, int64_t set_base_fn, int64_t jac_fn, int64_t cuctx,
    int64_t grid, int64_t block, int64_t scratch, int64_t sel,
    ffi::Buffer<ffi::U8> blob, ffi::Buffer<ffi::F32> actions,
    ffi::Result<ffi::Buffer<ffi::U8>> blob_out,
    ffi::Result<ffi::Buffer<ffi::F32>> jac)
{
    return BfStepJacPureLaunch(stream, set_base_fn, jac_fn, cuctx, grid, block,
                               scratch, nullptr, (int)sel, blob, actions, blob_out, jac);
}

XLA_FFI_DEFINE_HANDLER_SYMBOL(
    BfFwStepJacGradPure, BfFwStepJacGradPureImpl,
    ffi::Ffi::Bind()
        .Ctx<ffi::PlatformStream<cudaStream_t>>()
        .Attr<int64_t>("set_base_fn")
        .Attr<int64_t>("jac_fn")
        .Attr<int64_t>("cuctx")
        .Attr<int64_t>("grid")
        .Attr<int64_t>("block")
        .Attr<int64_t>("scratch")
        .Attr<int64_t>("sel")            // ncols: leading action columns to fill (4 = full)
        .Arg<ffi::Buffer<ffi::U8>>()     // blob     [N*stride]
        .Arg<ffi::Buffer<ffi::F32>>()    // actions  [N, 4]
        .Ret<ffi::Buffer<ffi::U8>>()     // blob_out (alias 0)
        .Ret<ffi::Buffer<ffi::F32>>(),   // jac      [N, 16]
    {ffi::Traits::kCmdBufferCompatible});  // stream-only -> CUDA-graph capturable
