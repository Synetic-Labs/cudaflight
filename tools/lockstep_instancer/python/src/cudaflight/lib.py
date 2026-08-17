# SPDX-License-Identifier: GPL-3.0-or-later
"""ctypes bindings for libcudaflight.so / libcpuflight.so and artifact resolution.

The packaged wheel ships its own ``libcudaflight.so``, ``fw.fatbin`` and
``libcpuflight.so`` (the CPU SITL fleet, for small no-CUDA fleets) under
``cudaflight/_data/``; ``importlib.resources`` returns those by default.
All can be overridden via the ``CUDAFLIGHT_LIB`` / ``CUDAFLIGHT_FATBIN`` /
``CPUFLIGHT_LIB`` environment variables for ad-hoc rebuilds without
reinstalling the wheel.
"""

import ctypes
import os
from importlib.resources import as_file, files
from pathlib import Path


def _resource(name: str) -> Path:
    # assumes the wheel is installed unzipped (no zipimport): the path must
    # stay valid after the as_file() context exits
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


def load(lib_path: "str | os.PathLike[str] | None" = None) -> ctypes.CDLL:
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
    # with_grad variant: when 0, skips the FD-Jacobian scratch (gradScratch =
    # stride*n, as large as the whole instance array) so a PPO-only fleet fits
    # ~1.5x more worlds. Guarded for older libcudaflight.so without it.
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
    # AUX RC channels (arm switch, flight mode) for manual / free flight.
    # Guarded for older libcudaflight.so without AUX support; cudaflight_aux_dim
    # returns 0 there.
    if hasattr(lib, "cudaflight_aux_dim"):
        lib.cudaflight_aux_dim.restype = ctypes.c_uint32
        lib.cudaflight_aux_dim.argtypes = [ctypes.c_void_p]
        lib.cudaflight_aux_ptr.restype = ctypes.c_uint64
        lib.cudaflight_aux_ptr.argtypes = [ctypes.c_void_p]
        lib.cudaflight_write_aux.argtypes = [ctypes.c_void_p, ctypes.c_uint64]
        lib.cudaflight_write_aux_host.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
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
    # raw launch parameters for an out-of-tree XLA FFI fused path: kernel
    # function pointers, the shared CUDA context, and the per-instance
    # state/snapshot buffers, all consumed by in-jit custom calls.
    for name in ("cudaflight_fw_step_kernel", "cudaflight_state_ptr", "cudaflight_ctx",
                 "cudaflight_reset_kernel", "cudaflight_snap_state_ptr", "cudaflight_snap_ptr",
                 "cudaflight_jac_fd_kernel", "cudaflight_grad_scratch_ptr",
                 "cudaflight_jac_fd_pure_kernel",
                 "cudaflight_grad_kernel", "cudaflight_jac_grad_pure_kernel",
                 "cudaflight_set_base_kernel", "cudaflight_stride", "cudaflight_state_size",
                 "cudaflight_inst_ptr"):
        if not hasattr(lib, name):  # older libcudaflight.so builds lack some getters
            continue
        getattr(lib, name).restype = ctypes.c_uint64
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    for name in ("cudaflight_grid", "cudaflight_block"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    return lib


def default_cpu_lib_path() -> Path:
    env = os.environ.get("CPUFLIGHT_LIB")
    return Path(env) if env else _resource("libcpuflight.so")


def load_cpu(lib_path: "str | os.PathLike[str] | None" = None) -> ctypes.CDLL:
    """ctypes binding for libcpuflight.so — the CPU SITL twin of the
    external-physics API (cpuflight.c). No CUDA and no minimum fleet size
    (cudaflight refuses n < 3: the runtime relocation table cannot be
    discovered below 3 instances). Instances step sequentially in-process.
    Overridable via CPUFLIGHT_LIB."""
    path = Path(lib_path) if lib_path else default_cpu_lib_path()
    if not path.exists():
        raise FileNotFoundError(
            f"libcpuflight.so not found at {path}; rebuild the cudaflight wheel "
            "(`make wheel` in tools/lockstep_instancer/python/) or set "
            "CPUFLIGHT_LIB to a built libcpuflight.so")
    lib = ctypes.CDLL(str(path))
    lib.cpuflight_error.restype = ctypes.c_char_p
    lib.cpuflight_create.restype = ctypes.c_void_p
    lib.cpuflight_create.argtypes = [ctypes.c_uint32, ctypes.c_uint32]
    # eeprom_path: boot-ready config image (None boots defaults), same
    # converter output cudaflight_create_eeprom takes.
    lib.cpuflight_create_eeprom.restype = ctypes.c_void_p
    lib.cpuflight_create_eeprom.argtypes = [ctypes.c_uint32, ctypes.c_uint32,
                                            ctypes.c_char_p]
    for name in ("cpuflight_num_envs", "cpuflight_act_dim", "cpuflight_aux_dim"):
        getattr(lib, name).restype = ctypes.c_uint32
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    # fw_step(h, actions[n,4] f32, sensors[n,7] f32, motors[n,4] f32 out,
    #         armed[n] u8 out, substeps)
    lib.cpuflight_fw_step.restype = ctypes.c_int
    lib.cpuflight_fw_step.argtypes = [
        ctypes.c_void_p,
        ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_float),
        ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_uint8),
        ctypes.c_uint32]
    lib.cpuflight_set_aux.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]
    lib.cpuflight_snapshot.restype = ctypes.c_int
    lib.cpuflight_snapshot.argtypes = [ctypes.c_void_p]
    lib.cpuflight_reset_mask.restype = ctypes.c_int
    lib.cpuflight_reset_mask.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint8)]
    lib.cpuflight_reset_all.restype = ctypes.c_int
    lib.cpuflight_reset_all.argtypes = [ctypes.c_void_p]
    lib.cpuflight_destroy.argtypes = [ctypes.c_void_p]
    return lib
