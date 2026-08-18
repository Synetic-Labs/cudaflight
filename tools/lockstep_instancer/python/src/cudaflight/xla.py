# SPDX-License-Identifier: GPL-3.0-or-later
"""Firmware-in-the-jit: the Betaflight kernels as XLA FFI custom calls.

The coupled-env throughput ceiling is never the GPU — it is the host dispatches
and stream synchronizations per 1 ms exchange. This module registers the
packaged XLA FFI handlers (``_data/libcudaflight_xla.so``, source
``_data/cudaflight_xla.cpp``) that launch the firmware kernels directly on
XLA's compute stream, so a whole decimation loop fuses into one jitted
program: zero syncs, zero copies, the firmware ordered between physics ops
purely by its buffer dependencies.

Requires ``jax`` (not a dependency of the core package — install the ``jax``
extra or bring your own). The handlers resolve in this order:

1. ``CUDAFLIGHT_XLA_LIB`` — explicit path to a built ``libcudaflight_xla.so``.
2. The prebuilt library packaged in the wheel (``_data/libcudaflight_xla.so``).
3. A local build of the packaged source against this venv's jaxlib headers,
   cached under ``~/.cache/cudaflight`` (needs ``g++`` and the NVIDIA driver's
   ``libcuda``; no CUDA toolkit).

Typical use, on a handle from ``cudaflight.lib.load()`` +
``cudaflight_create_eeprom_ex``:

    fw_step = fw_step_pure_call(lib, handle)   # value-threaded, donated buffers
    reset   = reset_pure_call(lib, handle)
    blob, fwstate = snapshot_state(lib, handle, device_index=0)  # armed snapshot

The *pure* calls thread the firmware instance blob and the ``bfFlight_t``
state through the computation as donated JAX buffers (input_output_aliases) —
no hidden handle mutation, so the caller's step is a pure jittable function.
The plain calls (``fw_step_call``/``reset_call``) mutate the handle's live
buffers and are registered with side effects instead.
"""

from __future__ import annotations

import ctypes
import hashlib
import os
import subprocess
from collections.abc import Callable, Sequence
from pathlib import Path
from typing import Any

from cudaflight.lib import _resource

__all__ = [
    "THROTTLE_ACTION_IDX",
    "device_view",
    "diff_fw_step_call",
    "diff_fw_step_pure_amortized_call",
    "diff_fw_step_pure_call",
    "fw_step_call",
    "fw_step_grad_call",
    "fw_step_jac_call",
    "fw_step_jac_fd_pure_call",
    "fw_step_jac_grad_pure_call",
    "fw_step_jac_hybrid_pure_call",
    "fw_step_pure_call",
    "register",
    "reset_call",
    "reset_pure_call",
    "snapshot_state",
]


# ---------------------------------------------------------------------------
# Handler library resolution + registration


def _cuda_lib_dirs() -> list[str]:
    """Directories to search for ``libcuda.so`` at link time (the ``-L`` for
    ``-lcuda``). ``libcuda`` is the CUDA *driver* library, separate from the
    toolkit: on a headless / toolkit-only box it is often present only as the
    toolkit STUB (``$CUDA/lib64/stubs/libcuda.so``), which is NOT on g++'s
    default link path — so a bare ``-lcuda`` fails with "cannot find -lcuda".
    Probe the toolkit stubs + the usual driver install dirs; only existing dirs
    are returned, so a spurious ``-L`` never itself breaks the link."""
    roots = []
    if os.environ.get("CUDA_HOME"):
        roots.append(os.environ["CUDA_HOME"])
    roots += ["/opt/cuda", "/usr/local/cuda"]
    dirs: list[str] = []
    for root in roots:
        dirs += [str(Path(root, "lib64", "stubs")), str(Path(root, "lib", "stubs")),
                 str(Path(root, "lib64")), str(Path(root, "lib"))]
    dirs += ["/usr/lib/x86_64-linux-gnu", "/usr/lib64", "/usr/lib", "/usr/lib/wsl/lib"]
    seen: set[str] = set()
    out: list[str] = []
    for d in dirs:
        if d not in seen and Path(d).is_dir():
            seen.add(d)
            out.append(d)
    return out


def _build_from_source() -> Path:
    """Compile the packaged handler source against this venv's jaxlib headers,
    cached by source+include hash. No CUDA *toolkit* needed — the shim vendors
    the few driver-API declarations it uses, so the build needs only g++,
    jaxlib's headers, and libcuda (the NVIDIA driver lib, on every GPU box)."""
    import jax
    import jax.ffi

    src = _resource("cudaflight_xla.cpp")
    if not src.exists():
        raise FileNotFoundError(
            "cudaflight_xla.cpp missing from cudaflight._data; rebuild the wheel "
            "(`make wheel` in tools/lockstep_instancer/python/)")
    inc = jax.ffi.include_dir()
    tag = hashlib.sha256(
        src.read_bytes() + inc.encode() + jax.__version__.encode()
    ).hexdigest()[:16]
    cache = Path(os.environ.get("XDG_CACHE_HOME", Path.home() / ".cache")) / "cudaflight"
    cache.mkdir(parents=True, exist_ok=True)
    lib_path = cache / f"libcudaflight_xla_{tag}.so"
    if lib_path.exists():
        return lib_path
    lib_dirs = _cuda_lib_dirs()
    cmd = ["g++", "-std=c++17", "-O2", "-shared", "-fPIC",
           f"-I{inc}", str(src),
           *[f"-L{d}" for d in lib_dirs], "-lcuda", "-o", str(lib_path)]
    try:
        subprocess.run(cmd, check=True, capture_output=True, text=True)
    except FileNotFoundError as e:
        raise RuntimeError(
            "cudaflight XLA shim build failed: 'g++' not found. Install a C++ "
            "toolchain, or set CUDAFLIGHT_XLA_LIB to a prebuilt libcudaflight_xla.so."
        ) from e
    except subprocess.CalledProcessError as e:
        raise RuntimeError(
            "Failed to compile the packaged cudaflight XLA shim (cudaflight_xla.cpp).\n"
            "  (No CUDA toolkit needed — only g++, jaxlib headers, and libcuda.)\n"
            f"  libcuda -L   : {lib_dirs or '(NONE found — is the NVIDIA driver installed?)'}\n"
            f"  jaxlib inc   : {inc}\n"
            f"  command      : {' '.join(cmd)}\n\n"
            f"--- g++ output ---\n{(e.stderr or '') + (e.stdout or '')}"
        ) from e
    return lib_path


def _shim_cdll() -> ctypes.CDLL:
    """The handler library: env override → packaged prebuilt → local build."""
    env = os.environ.get("CUDAFLIGHT_XLA_LIB")
    if env:
        return ctypes.CDLL(env)
    prebuilt = _resource("libcudaflight_xla.so")
    if prebuilt.exists():
        try:
            return ctypes.CDLL(str(prebuilt))
        except OSError:
            pass  # e.g. loader/ABI mismatch on an exotic box → rebuild locally
    return ctypes.CDLL(str(_build_from_source()))


_registered = False


def register() -> None:
    """Register every firmware FFI handler with XLA (idempotent).

    The pure handlers are tagged COMMAND_BUFFER_COMPATIBLE in their C++ handler
    definition (the kCmdBufferCompatible trait on the handler bundle — NOT the
    Python traits= kwarg, which the CUDA PJRT plugin rejects), so registration
    is plain here.
    """
    global _registered
    if _registered:
        return
    import jax.ffi

    lib = _shim_cdll()

    def reg(name: str, sym: Any) -> None:
        jax.ffi.register_ffi_target(name, jax.ffi.pycapsule(sym), platform="CUDA")

    reg("bf_fw_step", lib.BfFwStep)
    reg("bf_reset", lib.BfReset)
    reg("bf_fw_step_jac", lib.BfFwStepJacFD)
    reg("bf_fw_step_grad", lib.BfFwStepGrad)
    reg("bf_fw_step_pure", lib.BfFwStepPure)
    reg("bf_reset_pure", lib.BfResetPure)
    reg("bf_fw_step_jac_fd_pure", lib.BfFwStepJacFDPure)
    reg("bf_fw_step_jac_grad_pure", lib.BfFwStepJacGradPure)
    if hasattr(lib, "BfResetPureV2"):  # absent from pre-0.3.4 handler builds
        reg("bf_reset_pure_v2", lib.BfResetPureV2)
    else:
        global SUPPORTS_SNAPSHOT_ARGS
        SUPPORTS_SNAPSHOT_ARGS = False
    _registered = True


#: True when the handler library provides BfResetPureV2 — the snapshot-as-
#: buffer-arguments reset (see reset_pure_call's `snapshot` parameter).
#: register() downgrades this to False when an older handler build loads
#: (e.g. a CUDAFLIGHT_XLA_LIB override).
SUPPORTS_SNAPSHOT_ARGS = True


# ---------------------------------------------------------------------------
# DLPack view of raw device pointers (snapshot copies, buffer inspection)

_KDL_CUDA = 2
_KDL_UINT = 1


class _DLDevice(ctypes.Structure):
    _fields_ = [("device_type", ctypes.c_int32), ("device_id", ctypes.c_int32)]


class _DLDataType(ctypes.Structure):
    _fields_ = [("code", ctypes.c_uint8), ("bits", ctypes.c_uint8), ("lanes", ctypes.c_uint16)]


class _DLTensor(ctypes.Structure):
    _fields_ = [
        ("data", ctypes.c_void_p),
        ("device", _DLDevice),
        ("ndim", ctypes.c_int32),
        ("dtype", _DLDataType),
        ("shape", ctypes.POINTER(ctypes.c_int64)),
        ("strides", ctypes.POINTER(ctypes.c_int64)),  # NULL = compact row-major
        ("byte_offset", ctypes.c_uint64),
    ]


_DELETER_T = ctypes.CFUNCTYPE(None, ctypes.c_void_p)


class _DLManagedTensor(ctypes.Structure):
    _fields_ = [
        ("dl_tensor", _DLTensor),
        ("manager_ctx", ctypes.c_void_p),
        ("deleter", _DELETER_T),
    ]


# the ctypes structures must outlive the consumer; the deleter retires them
_LIVE: dict[int, tuple[Any, Any]] = {}


@_DELETER_T
def _retire(ptr: int) -> None:
    _LIVE.pop(ptr, None)


# Private binding: ctypes.pythonapi is a process-wide singleton whose
# per-function argtypes other libraries overwrite (jax.ffi.pycapsule does) —
# configuring our own PyDLL instance keeps this immune.
_capsule_new = ctypes.PyDLL(None).PyCapsule_New
_capsule_new.restype = ctypes.py_object
_capsule_new.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p]


class device_view:
    """DLPack exporter for a raw CUDA device pointer the library owns.

    ``jnp.from_dlpack(device_view(ptr, (n,), device_index))`` views ``n`` bytes
    at ``ptr`` as a uint8 array with no copy; ``jnp.copy`` of that view is the
    snapshot idiom. The library fully synchronizes on entry/exit of every call,
    so the consumer's stream argument can be ignored."""

    def __init__(self, ptr: int, shape: Sequence[int], device_index: int = 0,
                 code: int = _KDL_UINT, bits: int = 8) -> None:
        self._ptr, self._shape = ptr, tuple(shape)
        self._code, self._bits = code, bits
        self._device_id = device_index

    def __dlpack_device__(self) -> tuple[int, int]:
        return (_KDL_CUDA, self._device_id)

    def __dlpack__(self, **kwargs: Any) -> Any:  # returns a DLPack PyCapsule
        shape = (ctypes.c_int64 * len(self._shape))(*self._shape)
        mt = _DLManagedTensor()
        mt.dl_tensor.data = self._ptr
        mt.dl_tensor.device = _DLDevice(_KDL_CUDA, self._device_id)
        mt.dl_tensor.ndim = len(self._shape)
        mt.dl_tensor.dtype = _DLDataType(self._code, self._bits, 1)
        mt.dl_tensor.shape = shape
        mt.dl_tensor.strides = None
        mt.dl_tensor.byte_offset = 0
        mt.manager_ctx = None
        mt.deleter = _retire
        addr = ctypes.addressof(mt)
        _LIVE[addr] = (mt, shape)
        return _capsule_new(addr, b"dltensor", None)


def snapshot_state(lib: ctypes.CDLL, handle: int, device_index: int = 0) -> tuple[Any, Any]:
    """Copy the armed-on-ground episode-start snapshot into fresh JAX buffers.

    Returns ``(blob, fwstate)`` uint8 arrays of shape ``[n*stride]`` /
    ``[n*state_size]`` — the value-threaded pair ``fw_step_pure_call`` steps and
    ``reset_pure_call`` restores."""
    import jax.numpy as jnp

    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    state_size = lib.cudaflight_state_size(handle)
    blob_view = device_view(
        lib.cudaflight_snap_ptr(handle), (n * stride,), device_index)
    state_view = device_view(
        lib.cudaflight_snap_state_ptr(handle), (n * state_size,), device_index)
    return jnp.copy(jnp.from_dlpack(blob_view)), jnp.copy(jnp.from_dlpack(state_view))


# ---------------------------------------------------------------------------
# In-jit call builders (side-effecting flavor: the handle's live buffers)


def fw_step_call(
    lib: ctypes.CDLL, handle: int, substeps: int = 1
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Build the in-jit firmware step for a cudaflight fleet.

    lib/handle: the ctypes libcudaflight binding and cudaflight_create handle.
    Returns fw_step(actions [N,4] f32, sensors [N,7] f32) ->
    (motors [N,4] f32, armed [N] u8), traceable under jit/scan. The call
    mutates the firmware instance blobs (has_side_effect keeps every call and
    their order)."""
    import jax
    import jax.numpy as jnp

    register()
    n = lib.cudaflight_num_envs(handle)
    attrs = dict(
        fn=int(lib.cudaflight_fw_step_kernel(handle)),
        state=int(lib.cudaflight_state_ptr(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
        substeps=int(substeps),
    )

    call = jax.ffi.ffi_call(
        "bf_fw_step",
        (jax.ShapeDtypeStruct((n, 4), jnp.float32),   # motors
         jax.ShapeDtypeStruct((n,), jnp.uint8)),      # armed
        has_side_effect=True,
    )

    def fw_step(actions: Any, sensors: Any) -> tuple[Any, Any]:
        motors, armed = call(actions, sensors, **attrs)
        return motors, armed

    return fw_step


def reset_call(lib: ctypes.CDLL, handle: int) -> Callable[[Any], Any]:
    """Build the in-jit masked firmware reset for a cudaflight fleet.

    Returns reset(mask [N] u8) -> () — a pure side effect that restores the
    flagged instances to the episode-start snapshot on XLA's stream. Sequence
    it after fw_step inside a scan; has_side_effect preserves the order."""
    import jax

    register()
    attrs = dict(
        fn=int(lib.cudaflight_reset_kernel(handle)),
        state=int(lib.cudaflight_state_ptr(handle)),
        snapst=int(lib.cudaflight_snap_state_ptr(handle)),
        snap=int(lib.cudaflight_snap_ptr(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
    )

    call = jax.ffi.ffi_call("bf_reset", (), has_side_effect=True)  # no results

    def reset(mask: Any) -> Any:
        return call(mask, **attrs)

    return reset


# ---------------------------------------------------------------------------
# In-jit call builders (pure flavor: value-threaded donated buffers)


def fw_step_pure_call(
    lib: ctypes.CDLL, handle: int, substeps: int = 1
) -> Callable[[Any, Any, Any, Any], tuple[Any, Any, Any, Any]]:
    """Build the *pure* (value-threaded) firmware step for a cudaflight fleet.

    The firmware blob and bfFlight_t state are donated JAX buffers, not hidden
    handle state — the whole firmware state flows through the computation as
    values, so the env is a pure function (vmappable, branchable, no
    has_side_effect). XLA places the blob buffer; the handler points the global
    instance base at it (bfSetBase) and the step kernel rebases-on-entry.

    Returns fw_step_pure(blob [N*stride u8], fwstate [N*stateSize u8],
    actions [N,4 f32], sensors [N,7 f32]) -> (blob, fwstate, motors [N,4 f32],
    armed [N u8]); blob/fwstate are aliased in place (donated)."""
    import jax
    import jax.numpy as jnp

    register()
    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    state_size = lib.cudaflight_state_size(handle)
    attrs = dict(
        set_base_fn=int(lib.cudaflight_set_base_kernel(handle)),
        step_fn=int(lib.cudaflight_fw_step_kernel(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
        substeps=int(substeps),
    )

    call = jax.ffi.ffi_call(
        "bf_fw_step_pure",
        (jax.ShapeDtypeStruct((n * stride,), jnp.uint8),      # blob_out
         jax.ShapeDtypeStruct((n * state_size,), jnp.uint8),  # fwstate_out
         jax.ShapeDtypeStruct((n, 4), jnp.float32),           # motors
         jax.ShapeDtypeStruct((n,), jnp.uint8)),              # armed
        input_output_aliases={0: 0, 1: 1},
    )

    def fw_step_pure(blob: Any, fwstate: Any, actions: Any,
                     sensors: Any) -> tuple[Any, Any, Any, Any]:
        return call(blob, fwstate, actions, sensors, **attrs)

    return fw_step_pure


def reset_pure_call(
    lib: ctypes.CDLL, handle: int, snapshot: tuple[Any, Any] | None = None
) -> Callable[[Any, Any, Any], tuple[Any, Any]]:
    """Build the *pure* masked firmware reset for a cudaflight fleet.

    Restores the flagged instances in the value-threaded blob + bfFlight_t state
    to the episode-start snapshot. Returns reset_pure(blob, fwstate, mask [N u8])
    -> (blob, fwstate), blob/fwstate aliased in place (donated). The restored
    instances carry a stale blobBase, fixed by the next step's rebase-on-entry.

    `snapshot`: None reads the handle's library-side snapshot buffers through
    baked device addresses (works on every handler build). A `(snap_blob,
    snap_state)` pair of caller-owned uint8 arrays (see `snapshot_state`) rides
    into the call as READ-ONLY buffer arguments instead — after which the
    library-side copies can be freed (`cudaflight_release_snapshots`), leaving
    all firmware data in caller buffers. Needs a >= 0.3.4 handler build
    (SUPPORTS_SNAPSHOT_ARGS)."""
    import jax
    import jax.numpy as jnp
    import numpy as np

    register()
    if snapshot is not None and not SUPPORTS_SNAPSHOT_ARGS:
        raise RuntimeError(
            "snapshot-as-arguments reset needs the BfResetPureV2 handler "
            "(cudaflight >= 0.3.4); the loaded handler library lacks it")
    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    state_size = lib.cudaflight_state_size(handle)
    attrs = dict(
        set_base_fn=int(lib.cudaflight_set_base_kernel(handle)),
        reset_fn=int(lib.cudaflight_reset_kernel(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
    )
    if snapshot is None:
        attrs["snapst"] = int(lib.cudaflight_snap_state_ptr(handle))
        attrs["snap"] = int(lib.cudaflight_snap_ptr(handle))

    call = jax.ffi.ffi_call(
        "bf_reset_pure" if snapshot is None else "bf_reset_pure_v2",
        (jax.ShapeDtypeStruct((n * stride,), jnp.uint8),       # blob_out
         jax.ShapeDtypeStruct((n * state_size,), jnp.uint8)),  # fwstate_out
        input_output_aliases={0: 0, 1: 1},
    )

    # custom_vjp so the masked reset survives reverse-mode AD (truncated BPTT
    # differentiates straight through a step that calls this). The reset only
    # restores integer firmware state — blob/fwstate/mask are all uint8
    # (non-differentiable) — so it contributes no policy gradient. Without a
    # rule the bare ffi_call has no jvp and the linearisation pass raises.
    # The captured snapshot arrays are constants of the trace; AD never
    # touches them.
    @jax.custom_vjp
    def reset_pure(blob: Any, fwstate: Any, mask: Any) -> tuple[Any, Any]:
        if snapshot is None:
            return call(blob, fwstate, mask, **attrs)
        snap_blob, snap_state = snapshot
        return call(blob, fwstate, snap_blob, snap_state, mask, **attrs)

    def reset_pure_fwd(blob, fwstate, mask):
        return reset_pure(blob, fwstate, mask), None

    def reset_pure_bwd(_res, _g):
        # all inputs uint8: hand back the symbolic-zero float0 cotangents the
        # differentiated scan expects.
        return (np.zeros((n * stride,), dtype=jax.dtypes.float0),
                np.zeros((n * state_size,), dtype=jax.dtypes.float0),
                np.zeros((n,), dtype=jax.dtypes.float0))

    reset_pure.defvjp(reset_pure_fwd, reset_pure_bwd)
    return reset_pure


# ---------------------------------------------------------------------------
# Differentiable control-law machinery (Jacobians + custom_vjp steps)


def fw_step_jac_call(
    lib: ctypes.CDLL, handle: int, eps: float = 1e-2
) -> Callable[[Any], Any]:
    """Build the in-jit finite-difference Jacobian of the real control law.

    Returns fw_step_jac(actions [N,4]) -> J [N,4,4], J[k,i,d] =
    d(motor_i)/d(action_d) at the current per-instance state (central
    differences, state saved/restored per perturbation). Side-effecting so XLA
    pins its order relative to the firmware step."""
    import jax
    import jax.numpy as jnp
    import numpy as np

    register()
    n = lib.cudaflight_num_envs(handle)
    attrs = dict(
        fn=int(lib.cudaflight_jac_fd_kernel(handle)),
        scratch=int(lib.cudaflight_grad_scratch_ptr(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
        eps=np.float32(eps),
    )
    call = jax.ffi.ffi_call(
        "bf_fw_step_jac",
        jax.ShapeDtypeStruct((n, 16), jnp.float32),
        has_side_effect=True,
    )

    def fw_step_jac(actions: Any) -> Any:
        return call(actions, **attrs).reshape(n, 4, 4)

    return fw_step_jac


def fw_step_grad_call(
    lib: ctypes.CDLL, handle: int
) -> Callable[[Any, Any], Any]:
    """Build the in-jit Enzyme reverse-mode VJP of the real control law.

    Returns fw_step_grad(actions [N,4], seed [N,4]) -> dActions [N,4], where
    dActions[k] = J[k]^T . seed[k]. One reverse-mode sweep — no per-column
    re-evaluation. Raises RuntimeError if the module was not built DIFF=1."""
    import jax
    import jax.numpy as jnp

    register()
    n = lib.cudaflight_num_envs(handle)
    fn = int(lib.cudaflight_grad_kernel(handle))
    if fn == 0:
        raise RuntimeError(
            "bfFwStepGrad kernel absent from the module; rebuild the firmware "
            "with DIFF=1 (tools/lockstep_instancer/build_gpu.sh)")
    attrs = dict(
        fn=fn,
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
        # bfFwStepGrad saves/restores the per-instance blob through this scratch
        # so each VJP evaluates at the same frozen state (no filter drift).
        scratch=int(lib.cudaflight_grad_scratch_ptr(handle)),
    )
    call = jax.ffi.ffi_call(
        "bf_fw_step_grad",
        jax.ShapeDtypeStruct((n, 4), jnp.float32),   # dActions
        has_side_effect=True,
    )

    def fw_step_grad(actions: Any, seed: Any) -> Any:
        return call(actions, seed, **attrs)

    return fw_step_grad


def diff_fw_step_call(
    lib: ctypes.CDLL, handle: int, substeps: int = 1, eps: float = 1e-2
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Firmware step whose motors are differentiable w.r.t. actions, via a
    custom_vjp: the forward runs the real firmware (bf_fw_step) and computes
    the control-law Jacobian J once (at the pre-step state); the backward is
    the pure-JAX contraction d_action = J^T . d_motors. Gradient w.r.t.
    sensors is zero (state frozen)."""
    import jax
    import jax.numpy as jnp

    fw = fw_step_call(lib, handle, substeps)
    jac = fw_step_jac_call(lib, handle, eps)
    n = lib.cudaflight_num_envs(handle)

    @jax.custom_vjp
    def step(actions: Any, sensors: Any) -> tuple[Any, Any]:
        return fw(actions, sensors)

    def step_fwd(actions, sensors):
        j = jac(actions)                    # at the pre-step state
        motors, armed = fw(actions, sensors)
        return (motors, armed), j

    def step_bwd(j, g):
        g_motors, _g_armed = g
        g_actions = jnp.einsum("kid,ki->kd", j, g_motors)
        g_sensors = jnp.zeros((n, 7), jnp.float32)
        return g_actions, g_sensors

    step.defvjp(step_fwd, step_bwd)
    return step


def _jac_pure_call(
    lib: ctypes.CDLL, handle: int, target: str, kernel_attr: str,
    eps: float | None, sel: int,
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Shared builder for the value-threaded control-law Jacobian FFIs.

    The kernel rebases the donated blob on entry (a bfSetBase points the global
    instance base at it first) and writes the Jacobian at the current
    per-instance state. `target` is the registered FFI name, `kernel_attr` the
    libcudaflight getter for the kernel pointer, `eps` the FD perturbation
    (None for the Enzyme kernel). `sel` selects which action columns to fill.
    Returns jac(blob [N*stride u8], actions [N,4 f32]) -> (J [N,4,4], blob_out);
    blob is aliased in place (the rebase mutates its self-pointers)."""
    import jax
    import jax.numpy as jnp
    import numpy as np

    register()
    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    attrs = dict(
        set_base_fn=int(lib.cudaflight_set_base_kernel(handle)),
        jac_fn=int(getattr(lib, kernel_attr)(handle)),
        cuctx=int(lib.cudaflight_ctx(handle)),
        grid=int(lib.cudaflight_grid(handle)),
        block=int(lib.cudaflight_block(handle)),
        scratch=int(lib.cudaflight_grad_scratch_ptr(handle)),
        sel=int(sel),
    )
    if eps is not None:
        attrs["eps"] = np.float32(eps)

    call = jax.ffi.ffi_call(
        target,
        (jax.ShapeDtypeStruct((n * stride,), jnp.uint8),    # blob_out (alias 0)
         jax.ShapeDtypeStruct((n, 16), jnp.float32)),       # jac
        input_output_aliases={0: 0},
    )

    def jac(blob: Any, actions: Any) -> tuple[Any, Any]:
        blob_out, j = call(blob, actions, **attrs)
        return j.reshape(n, 4, 4), blob_out

    return jac


def fw_step_jac_fd_pure_call(
    lib: ctypes.CDLL, handle: int, eps: float = 1e-2, col: int = -1
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Rebase-aware finite-difference Jacobian on a value-threaded blob.
    J[k,i,d] = d(motor_i)/d(action_d) at the threaded pre-step state. `col`
    selects a single action column (-1 = full 4x4)."""
    return _jac_pure_call(lib, handle, "bf_fw_step_jac_fd_pure",
                          "cudaflight_jac_fd_pure_kernel", eps, col)


def fw_step_jac_grad_pure_call(
    lib: ctypes.CDLL, handle: int, ncols: int = 4
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Rebase-aware Enzyme forward-mode Jacobian on a value-threaded blob.
    One forward sweep per action column fills J[k,i,d] at the threaded pre-step
    state. `ncols` is the number of leading action columns to sweep (4 = full).
    Requires a DIFF=1 module (bfFwStepJacGradPure)."""
    return _jac_pure_call(lib, handle, "bf_fw_step_jac_grad_pure",
                          "cudaflight_jac_grad_pure_kernel", None, ncols)


# Action channel that is NOT analytically differentiable through the firmware:
# rcCommand[THROTTLE] comes from an int16 lookup table (rcLookupThrottle, rc.c),
# so Enzyme correctly returns a zero throttle gradient. The hybrid backend below
# patches this one column with the finite-difference (secant) value. Index 3 is
# Betaflight's THROTTLE enum (ROLL=0, PITCH=1, YAW=2, THROTTLE=3).
THROTTLE_ACTION_IDX = 3


def fw_step_jac_hybrid_pure_call(
    lib: ctypes.CDLL, handle: int, eps: float = 1e-2
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Hybrid value-threaded Jacobian: exact Enzyme reverse-mode for the smooth
    channels, finite difference for the throttle column (non-differentiable
    through the int16 throttle lookup). Both kernels rebase the donated blob and
    run at the same pre-step state; the blob threads enzyme->fd->caller so the
    two launches stay ordered. Requires a DIFF=1 module."""
    jac_enz = fw_step_jac_grad_pure_call(lib, handle, ncols=THROTTLE_ACTION_IDX)
    jac_fd = fw_step_jac_fd_pure_call(lib, handle, eps, col=THROTTLE_ACTION_IDX)

    def jac(blob: Any, actions: Any) -> tuple[Any, Any]:
        j_enz, blob = jac_enz(blob, actions)
        j_fd, blob = jac_fd(blob, actions)
        j = j_enz.at[:, :, THROTTLE_ACTION_IDX].set(j_fd[:, :, THROTTLE_ACTION_IDX])
        return j, blob

    return jac


def _select_jac_pure(
    lib: ctypes.CDLL, handle: int, backend: str, eps: float
) -> Callable[[Any, Any], tuple[Any, Any]]:
    """Pick the value-threaded Jacobian builder for `backend` (hybrid/enzyme/fd)."""
    if backend in ("hybrid", "enzyme"):
        if int(lib.cudaflight_jac_grad_pure_kernel(handle)) == 0:
            raise RuntimeError(
                f"Enzyme pure Jacobian kernel (bfFwStepJacGradPure) absent from the "
                f"module; rebuild the firmware DIFF=1 or pass backend='fd' "
                f"(requested backend={backend!r})")
        return (fw_step_jac_hybrid_pure_call(lib, handle, eps) if backend == "hybrid"
                else fw_step_jac_grad_pure_call(lib, handle))
    if backend == "fd":
        return fw_step_jac_fd_pure_call(lib, handle, eps)
    raise ValueError(f"unknown grad backend {backend!r} (use 'hybrid', 'enzyme' or 'fd')")


def diff_fw_step_pure_call(
    lib: ctypes.CDLL, handle: int, backend: str = "hybrid",
    substeps: int = 1, eps: float = 1e-2,
) -> Callable[..., tuple[Any, Any, Any, Any]]:
    """Value-threaded firmware step whose motors are differentiable w.r.t.
    actions, via custom_vjp — the pure-path twin of diff_fw_step_call. The
    forward threads the firmware blob + bfFlight_t state as donated JAX values
    (fw_step_pure) and computes the pre-step control-law Jacobian J once; the
    backward is the pure contraction d_action = J^T . d_motors, never
    re-entering the firmware. Gradient w.r.t. blob/fwstate/sensors is zero.

    backend: "hybrid" (Enzyme + FD throttle column, needs DIFF=1), "enzyme"
    (exact attitude, ZERO throttle gradient), or "fd" (any module)."""
    import jax
    import jax.numpy as jnp
    import numpy as np

    fw = fw_step_pure_call(lib, handle, substeps)
    jac = _select_jac_pure(lib, handle, backend, eps)

    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    state_size = lib.cudaflight_state_size(handle)

    @jax.custom_vjp
    def step(blob: Any, fwstate: Any, actions: Any,
             sensors: Any) -> tuple[Any, Any, Any, Any]:
        return fw(blob, fwstate, actions, sensors)

    def step_fwd(blob, fwstate, actions, sensors):
        # J at the PRE-step state; the rebased blob threads on into the step so
        # the Jacobian and the step see the same (correctly based) instance.
        j, blob = jac(blob, actions)
        blob, fwstate, motors, armed = fw(blob, fwstate, actions, sensors)
        return (blob, fwstate, motors, armed), j

    def step_bwd(j, g):
        _g_blob, _g_fwstate, g_motors, _g_armed = g
        g_actions = jnp.einsum("kid,ki->kd", j, g_motors)
        g_sensors = jnp.zeros((n, 7), jnp.float32)
        # blob/fwstate are uint8 (non-differentiable): their cotangents are the
        # symbolic-zero float0 the carry expects in a differentiated scan.
        z_blob = np.zeros((n * stride,), dtype=jax.dtypes.float0)
        z_state = np.zeros((n * state_size,), dtype=jax.dtypes.float0)
        return (z_blob, z_state, g_actions, g_sensors)

    step.defvjp(step_fwd, step_bwd)
    return step


def diff_fw_step_pure_amortized_call(
    lib: ctypes.CDLL, handle: int, backend: str = "hybrid",
    substeps: int = 1, eps: float = 1e-2,
) -> tuple[Callable[[Any, Any], tuple[Any, Any]],
           Callable[..., tuple[Any, Any, Any, Any]]]:
    """Amortized-Jacobian twin of diff_fw_step_pure_call: the control-law
    Jacobian is supplied EXTERNALLY (computed once per control step and reused
    across that step's decimation substeps).

    Returns ``(jac, step)``:
      ``jac(blob, actions) -> (J [N,4,4], blob)`` — call ONCE per control step.
      ``step(blob, fwstate, actions, sensors, J)`` — per-substep differentiable
      firmware step whose custom_vjp backward contracts the SHARED J. The
      caller must pass a ``stop_gradient``'d J (a constant residual)."""
    import jax
    import jax.numpy as jnp
    import numpy as np

    fw = fw_step_pure_call(lib, handle, substeps)
    jac = _select_jac_pure(lib, handle, backend, eps)
    n = lib.cudaflight_num_envs(handle)
    stride = lib.cudaflight_stride(handle)
    state_size = lib.cudaflight_state_size(handle)

    @jax.custom_vjp
    def step(blob: Any, fwstate: Any, actions: Any,
             sensors: Any, jac_shared: Any) -> tuple[Any, Any, Any, Any]:
        return fw(blob, fwstate, actions, sensors)

    def step_fwd(blob, fwstate, actions, sensors, jac_shared):
        blob, fwstate, motors, armed = fw(blob, fwstate, actions, sensors)
        return (blob, fwstate, motors, armed), jac_shared

    def step_bwd(jac_shared, g):
        _g_blob, _g_fwstate, g_motors, _g_armed = g
        g_actions = jnp.einsum("kid,ki->kd", jac_shared, g_motors)
        g_sensors = jnp.zeros((n, 7), jnp.float32)
        z_blob = np.zeros((n * stride,), dtype=jax.dtypes.float0)
        z_state = np.zeros((n * state_size,), dtype=jax.dtypes.float0)
        g_jac = jnp.zeros_like(jac_shared)   # J is a constant residual (no path back)
        return (z_blob, z_state, g_actions, g_sensors, g_jac)

    step.defvjp(step_fwd, step_bwd)
    return jac, step
