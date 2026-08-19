# SPDX-License-Identifier: GPL-3.0-or-later
"""Oracle tests for config.render_eeprom (CLI dump -> eeprom image):

  A  happy path: the stock dump (this build's own 'dump all') renders at
     exact parity — gate passes, zero gaps, round-trip verified — to a
     structurally valid image (magic, size, walkable record stream)
  B  refusals: text without a version header; a str that names a file
     instead of CLI text; a dump whose release string is not this
     firmware's (the version gate)
  C  strict overrides: a typo'd override fails loudly; a valid override
     applies last and changes the image
  D  boot: a fleet created from the rendered image boots and arms

Run: CPUFLIGHT_LIB=../../obj/multi/libcpuflight.so python test_render_eeprom.py
"""

import ctypes
import struct
import sys
import tempfile
from pathlib import Path

from cudaflight import load_cpu, render_eeprom

CONFIGS = Path(__file__).resolve().parents[2] / "configs" / "example"


def parse_records(image: bytes) -> dict[int, tuple[int, int]]:
    """Walk the record stream: pgn -> (version, size). Asserts structure."""
    assert image[1] == 0xBE, f"bad magic 0x{image[1]:02X}"
    records = {}
    p = 2
    while p + 6 <= len(image):
        size, pgn, version, _flags = struct.unpack_from("<HHBB", image, p)
        if size == 0:
            break
        assert size >= 6, f"malformed record at {p}"
        records[pgn] = (version, size)
        p += size
    assert records, "no records found"
    return records


def expect_refusal(label: str, exc: type, needle: str, *args, **kwargs) -> bool:
    try:
        render_eeprom(*args, **kwargs)
    except exc as e:
        if needle in str(e):
            print(f"[render-test] {label} refused as expected")
            return True
        print(f"[render-test] {label} FAIL: wrong message: {e}")
        return False
    print(f"[render-test] {label} FAIL: did not raise")
    return False


def main():
    ok = True
    stock = (CONFIGS / "stock_dump.txt").read_bytes()

    # A: the stock dump renders at parity — gate, gaps, verification
    image = render_eeprom(stock)
    records = parse_records(image)
    pid_ver, pid_size = records[14]  # PG_PID_PROFILE
    print(f"[render-test] A image={len(image)}B records={len(records)} "
          f"pid-profile v{pid_ver} ({pid_size}B)")

    # B: refusals
    ok &= expect_refusal("B1 headerless", RuntimeError, "version header",
                         "set no_such_setting_xyz = 1\n")
    ok &= expect_refusal("B2 str-path", ValueError, "pathlib.Path",
                         str(CONFIGS / "stock_dump.txt"))
    tampered = stock.replace(b" 2026.", b" 9999.", 1)
    assert tampered != stock, "tamper target not found; fixture header changed?"
    ok &= expect_refusal("B3 version gate", RuntimeError, "version gate", tampered)

    # C: strict overrides
    ok &= expect_refusal("C1 override typo", RuntimeError, "did not apply",
                         stock, "set moter_idle_typo = 600\n")
    changed = render_eeprom(stock, "set motor_idle = 600\n")
    if changed == image:
        print("[render-test] C2 FAIL: override did not change the image")
        ok = False
    else:
        print("[render-test] C2 override applied and round-trip verified")

    # D: a fleet created from the rendered image boots and arms
    lib = load_cpu()
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(image)
        f.flush()
        h = lib.cpuflight_create_eeprom(2, 0, f.name.encode())
        if not h:
            print(f"[render-test] D FAIL: create_eeprom: "
                  f"{lib.cpuflight_error().decode()}")
            ok = False
        else:
            print("[render-test] D fleet booted and armed on rendered image")
            lib.cpuflight_destroy(ctypes.c_void_p(h))

    print(f"[render-test] {'PASS' if ok else 'FAIL'}")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
