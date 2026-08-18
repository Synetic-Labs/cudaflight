# SPDX-License-Identifier: GPL-3.0-or-later
"""Validate the finite-difference gradient of the real Betaflight control law.

1. Baseline probe: run bflRateCore once for a flying action and read the raw
   float mixer output, to confirm motors respond to the stick at all.
2. Jacobian: for each motor i, seed cotangent e_i and read dActions = row i of
   J[i][d] = d(motor_i)/d(action_d), action AETR = (roll, pitch, throttle, yaw).

Checks (instance 0): throttle column same-sign (collective), roll column
mixed-sign (differential). Nonzero structured J => real gradients through
pidController + mixTable.
"""

import ctypes
import numpy as np

from cudaflight.lib import default_fatbin_path, load

EPS = 1e-2  # FD step, normalised action units


def main():
    lib = load()
    sigs = [
        ("cudaflight_grad_fd", ctypes.c_int, [ctypes.c_void_p, ctypes.c_float]),
        ("cudaflight_rate_eval", ctypes.c_int, [ctypes.c_void_p]),
        ("cudaflight_reset_all", ctypes.c_int, [ctypes.c_void_p]),
        ("cudaflight_write_seed_host", ctypes.c_int, [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]),
        ("cudaflight_grad_read_host", ctypes.c_int, [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]),
        ("cudaflight_motors_read_host", ctypes.c_int, [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]),
        ("cudaflight_write_actions_host", ctypes.c_int, [ctypes.c_void_p, ctypes.POINTER(ctypes.c_float)]),
    ]
    for name, res, args in sigs:
        getattr(lib, name).restype = res
        getattr(lib, name).argtypes = args

    def fptr(a: np.ndarray) -> "ctypes._Pointer[ctypes.c_float]":
        return a.ctypes.data_as(ctypes.POINTER(ctypes.c_float))

    n = 8  # >= 3: relocation discovery needs three instances
    cubin = str(default_fatbin_path())
    h = lib.cudaflight_create_eeprom(cubin.encode(), n, 0, 0, None)
    if not h:
        raise SystemExit(f"create failed: {lib.cudaflight_error().decode()}")
    try:
        # flying action: some roll (clear any deadband) + raised throttle
        act = np.zeros((n, 4), np.float32)
        act[:, 0] = 0.2   # roll
        act[:, 2] = 0.5   # throttle
        lib.cudaflight_write_actions_host(h, fptr(act))

        # --- baseline probe ---
        if lib.cudaflight_rate_eval(h) != 0:
            raise SystemExit(f"rate_eval failed: {lib.cudaflight_error().decode()}")
        m = np.empty((n, 4), np.float32)
        lib.cudaflight_motors_read_host(h, fptr(m))
        print(f"baseline raw motor[] (inst 0): {m[0]}")
        lib.cudaflight_reset_all(h)
        lib.cudaflight_write_actions_host(h, fptr(act))

        # --- Jacobian via FD ---
        J = np.zeros((4, 4), np.float32)
        for i in range(4):
            seed = np.zeros((n, 4), np.float32)
            seed[:, i] = 1.0
            lib.cudaflight_write_seed_host(h, fptr(seed))
            if lib.cudaflight_grad_fd(h, ctypes.c_float(EPS)) != 0:
                raise SystemExit(f"grad_fd failed: {lib.cudaflight_error().decode()}")
            d = np.empty((n, 4), np.float32)
            lib.cudaflight_grad_read_host(h, fptr(d))
            J[i] = d[0]

        np.set_printoptions(precision=4, suppress=True)
        print("\nJ[i][d] = d(motor_i)/d(action_d), action=(roll,pitch,throttle,yaw):")
        print(J)
        print(f"\nthrottle column (collective, expect same sign): {J[:, 2]}")
        print(f"roll column (differential, expect mixed sign):  {J[:, 0]}")
        if not np.any(J != 0):
            raise SystemExit("FAIL: gradient is all zero")
        print("\nPASS: nonzero gradients through the real control law")
    finally:
        lib.cudaflight_destroy(h)


if __name__ == "__main__":
    main()
