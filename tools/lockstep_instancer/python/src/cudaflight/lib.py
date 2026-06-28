"""ctypes binding for libcudaflight.so and artifact resolution.

The packaged wheel ships its own ``libcudaflight.so`` and ``fw.fatbin`` under
``cudaflight/_data/``; ``importlib.resources`` returns those by default.
Both can be overridden via the ``CUDAFLIGHT_LIB`` / ``CUDAFLIGHT_FATBIN`` environment
variables for ad-hoc rebuilds without reinstalling the wheel.
"""

import ctypes
import os
from importlib.resources import as_file, files
from pathlib import Path


def _resource(name: str) -> Path:
    ref = files("cudaflight._data") / name
    with as_file(ref) as p:
        return Path(p)


def default_lib_path() -> Path:
    env = os.environ.get("CUDAFLIGHT_LIB")
    return Path(env) if env else _resource("libcudaflight.so")


def default_fatbin_path() -> Path:
    env = os.environ.get("CUDAFLIGHT_FATBIN")
    if env:
        return Path(env)
    p = _resource("fw.fatbin")
    if not p.exists():
        raise FileNotFoundError(
            "fw.fatbin missing from cudaflight._data; build with "
            "`make wheel` in tools/lockstep_instancer/python/")
    return p


def load(lib_path=None) -> ctypes.CDLL:
    path = Path(lib_path) if lib_path else default_lib_path()
    if not path.exists():
        raise FileNotFoundError(
            f"libcudaflight.so not found at {path}; rebuild the cudaflight "
            "wheel (`make wheel` in tools/lockstep_instancer/python/) or "
            "set CUDAFLIGHT_LIB to a built libcudaflight.so")
    lib = ctypes.CDLL(str(path))
    lib.cudaflight_error.restype = ctypes.c_char_p
    lib.cudaflight_create.restype = ctypes.c_void_p
    lib.cudaflight_create.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_int, ctypes.c_uint32]
    # eeprom_path: boot-ready config image from the CPU --cli-dump converter
    # (None boots defaults); see tools/lockstep_instancer/configs/
    lib.cudaflight_create_eeprom.restype = ctypes.c_void_p
    lib.cudaflight_create_eeprom.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_int,
                                        ctypes.c_uint32, ctypes.c_char_p]
    # with_grad variant: when 0, skips the differentiable-rollout scratch
    # (gradScratch = stride*n, as large as the whole instance array) so a PPO-only
    # fleet fits ~1.5x more worlds. Guarded for older libcudaflight.so without it.
    if hasattr(lib, "cudaflight_create_eeprom_ex"):
        lib.cudaflight_create_eeprom_ex.restype = ctypes.c_void_p
        lib.cudaflight_create_eeprom_ex.argtypes = [ctypes.c_char_p, ctypes.c_uint32,
                                               ctypes.c_int, ctypes.c_uint32,
                                               ctypes.c_char_p, ctypes.c_int]
    for name in ("cudaflight_num_envs", "cudaflight_act_dim", "cudaflight_obs_dim"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("cudaflight_actions_ptr", "cudaflight_obs_ptr", "cudaflight_rewards_ptr", "cudaflight_dones_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.cudaflight_step.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    lib.cudaflight_reset_mask.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.cudaflight_write_actions.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.cudaflight_write_actions_host.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
    # external-physics mode
    for name in ("cudaflight_sensors_ptr", "cudaflight_motors_ptr", "cudaflight_armed_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.cudaflight_fw_step.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    lib.cudaflight_write_sensors.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.cudaflight_write_sensors_host.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
    for name in ("cudaflight_reset_done", "cudaflight_reset_all", "cudaflight_snapshot", "cudaflight_sync"):
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.cudaflight_hashes.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint64)]
    lib.cudaflight_destroy.argtypes = [ctypes.c_void_p]
    # OSD readback
    for name in ("cudaflight_osd_rows", "cudaflight_osd_cols"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("cudaflight_osd_ptr", "cudaflight_osd_attrs_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.cudaflight_osd_update.argtypes = [ctypes.c_void_p]
    # raw launch parameters for the XLA FFI fused path (sim.fused in
    # betaflight-gym): kernel function pointers, the shared CUDA context,
    # and the per-instance state/snapshot buffers, all consumed by the
    # in-jit custom calls.
    for name in ("cudaflight_fw_step_kernel", "cudaflight_state_ptr", "cudaflight_ctx",
                 "cudaflight_reset_kernel", "cudaflight_snap_state_ptr", "cudaflight_snap_ptr",
                 "cudaflight_jac_fd_kernel", "cudaflight_grad_scratch_ptr",
                 "cudaflight_grad_kernel",
                 "cudaflight_jac_fd_pure_kernel", "cudaflight_jac_grad_pure_kernel",
                 "cudaflight_set_base_kernel", "cudaflight_stride", "cudaflight_state_size",
                 "cudaflight_inst_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("cudaflight_grid", "cudaflight_block"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    return lib
