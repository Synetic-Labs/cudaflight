"""ctypes binding for libbfgym.so — shared by the torch and JAX wrappers."""

import ctypes
from pathlib import Path

DEFAULT_OUT = Path(__file__).resolve().parents[3] / "obj" / "gpu"


def load(lib_path=None):
    lib = ctypes.CDLL(str(lib_path or DEFAULT_OUT / "libbfgym.so"))
    lib.bfgym_error.restype = ctypes.c_char_p
    lib.bfgym_create.restype = ctypes.c_void_p
    lib.bfgym_create.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_int, ctypes.c_uint32]
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
    for name in ("bfgym_reset_done", "bfgym_reset_all", "bfgym_snapshot", "bfgym_sync"):
        getattr(lib, name).argtypes = [ctypes.c_void_p]
    lib.bfgym_hashes.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_uint64)]
    lib.bfgym_destroy.argtypes = [ctypes.c_void_p]
    return lib
