"""JAX interface to the GPU Betaflight fleet.

Same library and device buffers as the torch wrapper, JAX-idiomatic
contract: because JAX arrays are immutable, step() takes an actions array
and returns fresh (obs, rewards, dones) arrays. Data still never leaves
the GPU — actions are uploaded with one device-to-device copy, and the
outputs are device-to-device snapshots of the live buffers (made via a
zero-copy DLPack view; ~70 bytes/instance, negligible).

XLA and libbfgym share the device's primary CUDA context, and every
library call fully synchronizes the context on entry and exit, so XLA's
non-blocking streams are ordered against the firmware kernels.

NOTE: import this module (or set XLA_PYTHON_CLIENT_PREALLOCATE=false)
before JAX touches the GPU — by default XLA preallocates 75% of device
memory, which collides with the firmware's instance blobs and the 8GB
device-stack reservation.

Semantics (gymnasium NextStep-style auto-reset, like the torch wrapper):
  reset()       -> obs [N, 17]
  step(actions) -> (obs, rewards, dones); actions [N, 4] in [-1, 1], AETR.
Obs layout: pos NED (0:3), vel NED (3:6), quat wxyz (6:10), body rates
rad/s (10:13), normalised motors (13:17).
"""

import ctypes
import os

os.environ.setdefault("XLA_PYTHON_CLIENT_PREALLOCATE", "false")

import jax
import jax.numpy as jnp

from bfgym_lib import DEFAULT_OUT as _DEFAULT_OUT, load as _load

# ---------------------------------------------------------------------------
# Minimal DLPack producer for a raw CUDA device pointer, so jnp.from_dlpack
# can view the library's buffers with no third-party bridge.

_KDL_CUDA = 2
_KDL_INT, _KDL_UINT, _KDL_FLOAT = 0, 1, 2


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
_LIVE = {}


@_DELETER_T
def _retire(ptr):
    _LIVE.pop(ptr, None)


ctypes.pythonapi.PyCapsule_New.restype = ctypes.py_object
ctypes.pythonapi.PyCapsule_New.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p]


class _DevicePointer:
    """__dlpack__ exporter for a raw device pointer we keep ownership of."""

    def __init__(self, ptr, shape, code, bits, device_id):
        self._ptr, self._shape = ptr, tuple(shape)
        self._code, self._bits = code, bits
        self._device_id = device_id

    def __dlpack_device__(self):
        return (_KDL_CUDA, self._device_id)

    def __dlpack__(self, **kwargs):
        # data is fully synchronized by the library, so the consumer's
        # stream argument can be ignored; legacy (unversioned) capsule
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
        return ctypes.pythonapi.PyCapsule_New(addr, b"dltensor", None)


# ---------------------------------------------------------------------------


class BetaflightJaxEnv:
    def __init__(self, num_envs, cubin=None, lib=None, decimation=10,
                 device_index=0, settle_ms=0, auto_reset=True):
        self.decimation = decimation
        self.auto_reset = auto_reset
        self.device = jax.devices("gpu")[device_index]
        # make XLA claim the primary context before the library does
        jnp.zeros(1, device=self.device).block_until_ready()

        self._lib = _load(lib)
        cubin = str(cubin or _DEFAULT_OUT / "fw.cubin")
        self._h = self._lib.bfgym_create(cubin.encode(), num_envs, device_index, settle_ms)
        if not self._h:
            raise RuntimeError(f"bfgym_create failed: {self._lib.bfgym_error().decode()}")

        self.num_envs = self._lib.bfgym_num_envs(self._h)
        self.act_dim = self._lib.bfgym_act_dim(self._h)
        self.obs_dim = self._lib.bfgym_obs_dim(self._h)

        n = self.num_envs
        self._obs_view = _DevicePointer(
            self._lib.bfgym_obs_ptr(self._h), (n, self.obs_dim), _KDL_FLOAT, 32, device_index)
        self._rew_view = _DevicePointer(
            self._lib.bfgym_rewards_ptr(self._h), (n,), _KDL_FLOAT, 32, device_index)
        self._done_view = _DevicePointer(
            self._lib.bfgym_dones_ptr(self._h), (n,), _KDL_UINT, 8, device_index)

    def _check(self, rc):
        if rc != 0:
            raise RuntimeError(self._lib.bfgym_error().decode())

    def _snap(self, view):
        # zero-copy DLPack view of the live buffer, then an owned copy so
        # the returned array is a value, not an alias the next step mutates
        return jnp.copy(jnp.from_dlpack(view))

    def _outputs(self):
        return (self._snap(self._obs_view), self._snap(self._rew_view),
                self._snap(self._done_view).astype(jnp.bool_))

    def _upload_actions(self, actions):
        actions = jnp.asarray(actions, jnp.float32)
        if actions.shape != (self.num_envs, self.act_dim):
            raise ValueError(f"actions must be [{self.num_envs}, {self.act_dim}]")
        actions = jax.block_until_ready(actions)
        try:
            ptr = actions.__cuda_array_interface__["data"][0]
            self._check(self._lib.bfgym_write_actions(self._h, ptr))
        except AttributeError:  # CAI export unavailable: bounce via host
            import numpy as np
            host = np.asarray(actions, dtype=np.float32)
            self._check(self._lib.bfgym_write_actions_host(
                self._h, host.ctypes.data_as(ctypes.POINTER(ctypes.c_float))))

    def reset(self):
        """Restore every instance to the armed snapshot; returns obs."""
        self._check(self._lib.bfgym_reset_all(self._h))
        self._upload_actions(jnp.zeros((self.num_envs, self.act_dim), jnp.float32))
        self._check(self._lib.bfgym_step(self._h, 0))  # refresh obs, no sim advance
        return self._snap(self._obs_view)

    def step(self, actions):
        """Advance decimation control steps; returns (obs, rewards, dones)."""
        self._upload_actions(actions)
        self._check(self._lib.bfgym_step(self._h, self.decimation))
        out = self._outputs()
        if self.auto_reset:
            self._check(self._lib.bfgym_reset_done(self._h))
        return out

    def reset_mask(self, mask):
        """Restore instances where mask ([N] bool/uint8) is nonzero."""
        mask = jax.block_until_ready(jnp.asarray(mask, jnp.uint8))
        self._check(self._lib.bfgym_reset_mask(
            self._h, mask.__cuda_array_interface__["data"][0]))

    def snapshot(self):
        """Retake the episode-start snapshot at the current state."""
        self._check(self._lib.bfgym_snapshot(self._h))

    def hashes(self):
        """Per-instance motor-trace hashes (the determinism oracle)."""
        out = (ctypes.c_uint64 * self.num_envs)()
        self._check(self._lib.bfgym_hashes(self._h, out))
        return list(out)

    def close(self):
        if getattr(self, "_h", None):
            self._lib.bfgym_destroy(self._h)
            self._h = None

    def __del__(self):
        self.close()
