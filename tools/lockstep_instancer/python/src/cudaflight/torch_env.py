"""Zero-copy PyTorch interface to the GPU Betaflight fleet.

BetaflightEnv wraps libcudaflight.so: N real firmware instances live in device
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
import os

import torch

from .lib import default_fatbin_path as _default_fatbin
from .lib import load as _load


class _CudaArray:
    """Minimal __cuda_array_interface__ exporter for a raw device pointer."""

    def __init__(self, ptr: int, shape: "tuple[int, ...]", typestr: str) -> None:
        self.__cuda_array_interface__ = {
            "shape": tuple(shape),
            "typestr": typestr,
            "data": (ptr, False),
            "version": 2,
        }


class BetaflightEnv:
    def __init__(self, num_envs: int,
                 cubin: "str | os.PathLike[str] | None" = None,
                 lib: "str | os.PathLike[str] | None" = None,
                 decimation: int = 10, device_index: int = 0,
                 settle_ms: int = 0, auto_reset: bool = True,
                 eeprom: "str | os.PathLike[str] | None" = None) -> None:
        cubin = str(cubin or _default_fatbin())
        self.decimation = decimation
        self.auto_reset = auto_reset
        self.device = torch.device("cuda", device_index)

        # torch and the library share the device's primary CUDA context;
        # initialize torch's side first so tensor wrapping is valid.
        torch.cuda.init()
        torch.cuda.set_device(self.device)

        self._lib = _load(lib)
        self._h = self._lib.cudaflight_create_eeprom(
            cubin.encode(), num_envs, device_index, settle_ms,
            str(eeprom).encode() if eeprom else None)
        if not self._h:
            raise RuntimeError(f"cudaflight_create failed: {self._lib.cudaflight_error().decode()}")

        self.num_envs = self._lib.cudaflight_num_envs(self._h)
        self.act_dim = self._lib.cudaflight_act_dim(self._h)
        self.obs_dim = self._lib.cudaflight_obs_dim(self._h)

        n = self.num_envs

        def wrap(ptr: int, shape: "tuple[int, ...]", typestr: str) -> torch.Tensor:
            return torch.as_tensor(_CudaArray(ptr, shape, typestr), device=self.device)
        self.actions = wrap(self._lib.cudaflight_actions_ptr(self._h), (n, self.act_dim), "<f4")
        self.obs = wrap(self._lib.cudaflight_obs_ptr(self._h), (n, self.obs_dim), "<f4")
        self.rewards = wrap(self._lib.cudaflight_rewards_ptr(self._h), (n,), "<f4")
        self.dones = wrap(self._lib.cudaflight_dones_ptr(self._h), (n,), "|u1")

        # OSD character grids: each instance's firmware draws a MAX7456-style
        # screen; osd_update() refreshes these [N, rows, cols] uint8 views.
        self.osd_rows = self._lib.cudaflight_osd_rows(self._h)
        self.osd_cols = self._lib.cudaflight_osd_cols(self._h)
        grid = (n, self.osd_rows, self.osd_cols)
        self.osd = wrap(self._lib.cudaflight_osd_ptr(self._h), grid, "|u1")
        self.osd_attrs = wrap(self._lib.cudaflight_osd_attrs_ptr(self._h), grid, "|u1")

    def _check(self, rc: int) -> None:
        if rc != 0:
            raise RuntimeError(self._lib.cudaflight_error().decode())

    def reset(self) -> torch.Tensor:
        """Restore every instance to the armed snapshot; returns obs."""
        self._check(self._lib.cudaflight_reset_all(self._h))
        self.actions.zero_()
        self._check(self._lib.cudaflight_step(self._h, 0))  # refresh obs, no sim advance
        return self.obs

    def step(self, actions: "torch.Tensor | None" = None
             ) -> "tuple[torch.Tensor, torch.Tensor, torch.Tensor]":
        """Advance decimation control steps; returns (obs, rewards, dones)."""
        if actions is not None:
            self.actions.copy_(actions)
        self._check(self._lib.cudaflight_step(self._h, self.decimation))
        if self.auto_reset:
            self._check(self._lib.cudaflight_reset_done(self._h))
        return self.obs, self.rewards, self.dones

    def reset_mask(self, mask: torch.Tensor) -> None:
        """Restore instances where mask (uint8 CUDA tensor, [N]) is nonzero."""
        mask = mask.to(self.device, torch.uint8).contiguous()
        self._check(self._lib.cudaflight_reset_mask(self._h, mask.data_ptr()))

    def snapshot(self) -> None:
        """Retake the episode-start snapshot at the current state."""
        self._check(self._lib.cudaflight_snapshot(self._h))

    def osd_update(self) -> "tuple[torch.Tensor, torch.Tensor]":
        """Snapshot every instance's OSD grid into self.osd / self.osd_attrs."""
        self._check(self._lib.cudaflight_osd_update(self._h))
        return self.osd, self.osd_attrs

    def hashes(self) -> "list[int]":
        """Per-instance motor-trace hashes (the determinism oracle)."""
        out = (ctypes.c_uint64 * self.num_envs)()
        self._check(self._lib.cudaflight_hashes(self._h, out))
        return list(out)

    def close(self) -> None:
        if getattr(self, "_h", None):
            self._lib.cudaflight_destroy(self._h)
            self._h = None

    def __del__(self) -> None:
        self.close()
