"""Zero-copy PyTorch interface to the GPU Betaflight fleet.

BetaflightEnv wraps libbfgym.so: N real firmware instances live in device
memory, and actions / obs / rewards / dones are CUDA tensors mapped
directly onto the library's device buffers via __cuda_array_interface__ —
a training loop that computes actions on the GPU runs fully device-side,
with no PCIe transfer per step.

Semantics:
  reset()                -> obs [N, 17]  (all instances restored to the
                            armed-on-the-ground snapshot)
  step(actions)          -> (obs, rewards, dones); actions [N, 4] in
                            [-1, 1], AETR order (roll, pitch, throttle,
                            yaw), held for `decimation` 1ms control steps.
  Auto-reset (default): done instances are restored right after the step,
  gymnasium "NextStep" style — the returned obs is the terminal one, the
  restored state shows up in the next step's obs.

The returned tensors are views of live library buffers, overwritten by the
next step() — clone() anything you store in a rollout buffer.

Obs layout: pos NED (0:3), vel NED (3:6), quat wxyz (6:10), body rates
rad/s (10:13), normalised motors (13:17). The built-in reward/done is the
hover-at-5m task, but obs carry the full physical state, so reward and
termination can be recomputed in torch without touching the cubin.
"""

import ctypes

import torch

from bfgym_lib import DEFAULT_OUT as _DEFAULT_OUT, load as _load


class _CudaArray:
    """Minimal __cuda_array_interface__ exporter for a raw device pointer."""

    def __init__(self, ptr, shape, typestr):
        self.__cuda_array_interface__ = {
            "shape": tuple(shape),
            "typestr": typestr,
            "data": (ptr, False),
            "version": 2,
        }


class BetaflightEnv:
    def __init__(self, num_envs, cubin=None, lib=None, decimation=10,
                 device_index=0, settle_ms=0, auto_reset=True):
        cubin = str(cubin or _DEFAULT_OUT / "fw.cubin")
        lib = str(lib or _DEFAULT_OUT / "libbfgym.so")
        self.decimation = decimation
        self.auto_reset = auto_reset
        self.device = torch.device("cuda", device_index)

        # torch and the library share the device's primary CUDA context;
        # initialize torch's side first so tensor wrapping is valid.
        torch.cuda.init()
        torch.cuda.set_device(self.device)

        self._lib = _load(lib)
        self._h = self._lib.bfgym_create(cubin.encode(), num_envs, device_index, settle_ms)
        if not self._h:
            raise RuntimeError(f"bfgym_create failed: {self._lib.bfgym_error().decode()}")

        self.num_envs = self._lib.bfgym_num_envs(self._h)
        self.act_dim = self._lib.bfgym_act_dim(self._h)
        self.obs_dim = self._lib.bfgym_obs_dim(self._h)

        n = self.num_envs
        wrap = lambda ptr, shape, typestr: torch.as_tensor(
            _CudaArray(ptr, shape, typestr), device=self.device)
        self.actions = wrap(self._lib.bfgym_actions_ptr(self._h), (n, self.act_dim), "<f4")
        self.obs = wrap(self._lib.bfgym_obs_ptr(self._h), (n, self.obs_dim), "<f4")
        self.rewards = wrap(self._lib.bfgym_rewards_ptr(self._h), (n,), "<f4")
        self.dones = wrap(self._lib.bfgym_dones_ptr(self._h), (n,), "|u1")

    def _check(self, rc):
        if rc != 0:
            raise RuntimeError(self._lib.bfgym_error().decode())

    def reset(self):
        """Restore every instance to the armed snapshot; returns obs."""
        self._check(self._lib.bfgym_reset_all(self._h))
        self.actions.zero_()
        self._check(self._lib.bfgym_step(self._h, 0))  # refresh obs, no sim advance
        return self.obs

    def step(self, actions=None):
        """Advance decimation control steps; returns (obs, rewards, dones)."""
        if actions is not None:
            self.actions.copy_(actions)
        self._check(self._lib.bfgym_step(self._h, self.decimation))
        if self.auto_reset:
            self._check(self._lib.bfgym_reset_done(self._h))
        return self.obs, self.rewards, self.dones

    def reset_mask(self, mask):
        """Restore instances where mask (uint8 CUDA tensor, [N]) is nonzero."""
        mask = mask.to(self.device, torch.uint8).contiguous()
        self._check(self._lib.bfgym_reset_mask(self._h, mask.data_ptr()))

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
