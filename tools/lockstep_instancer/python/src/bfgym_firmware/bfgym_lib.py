"""ctypes binding for libbfgym.so and artifact resolution.

The packaged wheel ships its own ``libbfgym.so`` and ``fw.fatbin`` under
``bfgym_firmware/_data/``; ``importlib.resources`` returns those by default.
Both can be overridden via the ``BFGYM_LIB`` / ``BFGYM_FATBIN`` environment
variables for ad-hoc rebuilds without reinstalling the wheel.
"""

import ctypes
import os
from importlib.resources import as_file, files
from pathlib import Path


def _resource(name: str) -> Path:
    ref = files("bfgym_firmware._data") / name
    with as_file(ref) as p:
        return Path(p)


def default_lib_path() -> Path:
    env = os.environ.get("BFGYM_LIB")
    return Path(env) if env else _resource("libbfgym.so")


def default_fatbin_path() -> Path:
    env = os.environ.get("BFGYM_FATBIN")
    if env:
        return Path(env)
    p = _resource("fw.fatbin")
    if not p.exists():
        raise FileNotFoundError(
            "fw.fatbin missing from bfgym_firmware._data; build with "
            "`make wheel` in tools/lockstep_instancer/python/")
    return p


def load(lib_path=None) -> ctypes.CDLL:
    path = Path(lib_path) if lib_path else default_lib_path()
    if not path.exists():
        raise FileNotFoundError(
            f"libbfgym.so not found at {path}; rebuild the bfgym-firmware "
            "wheel (`make wheel` in tools/lockstep_instancer/python/) or "
            "set BFGYM_LIB to a built libbfgym.so")
    lib = ctypes.CDLL(str(path))
    lib.bfgym_error.restype = ctypes.c_char_p
    lib.bfgym_create.restype = ctypes.c_void_p
    lib.bfgym_create.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_int, ctypes.c_uint32]
    # eeprom_path: boot-ready config image from the CPU --cli-dump converter
    # (None boots defaults); see tools/lockstep_instancer/configs/
    lib.bfgym_create_eeprom.restype = ctypes.c_void_p
    lib.bfgym_create_eeprom.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_int,
                                        ctypes.c_uint32, ctypes.c_char_p]
    for name in ("bfgym_num_envs", "bfgym_act_dim", "bfgym_obs_dim"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("bfgym_actions_ptr", "bfgym_obs_ptr", "bfgym_rewards_ptr", "bfgym_dones_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.bfgym_step.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    lib.bfgym_reset_mask.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.bfgym_write_actions.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.bfgym_write_actions_host.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
    # external-physics mode
    for name in ("bfgym_sensors_ptr", "bfgym_motors_ptr", "bfgym_armed_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.bfgym_fw_step.argtypes = [ctypes.c_void_p, ctypes.c_uint32]
    lib.bfgym_write_sensors.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
    lib.bfgym_write_sensors_host.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
    for name in ("bfgym_reset_done", "bfgym_reset_all", "bfgym_snapshot", "bfgym_sync"):
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.bfgym_hashes.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint64)]
    lib.bfgym_destroy.argtypes = [ctypes.c_void_p]
    # OSD readback
    for name in ("bfgym_osd_rows", "bfgym_osd_cols"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("bfgym_osd_ptr", "bfgym_osd_attrs_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.bfgym_osd_update.argtypes = [ctypes.c_void_p]
    # raw launch parameters for the XLA FFI fused path (sim.fused in
    # betaflight-gym): kernel function pointers, the shared CUDA context,
    # and the per-instance state/snapshot buffers, all consumed by the
    # in-jit custom calls.
    for name in ("bfgym_fw_step_kernel", "bfgym_state_ptr", "bfgym_ctx",
                 "bfgym_reset_kernel", "bfgym_snap_state_ptr", "bfgym_snap_ptr",
                 "bfgym_jac_fd_kernel", "bfgym_grad_scratch_ptr",
                 "bfgym_grad_kernel",
                 "bfgym_jac_fd_pure_kernel", "bfgym_jac_grad_pure_kernel",
                 "bfgym_set_base_kernel", "bfgym_stride", "bfgym_state_size",
                 "bfgym_inst_ptr"):
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("bfgym_grid", "bfgym_block"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    return lib
