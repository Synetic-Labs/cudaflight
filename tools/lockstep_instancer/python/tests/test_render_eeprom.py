# SPDX-License-Identifier: GPL-3.0-or-later
"""Oracle tests for config.render_eeprom (CLI dump -> eeprom image):

  A  happy path: the stock dump (this build's own 'dump all') renders
     strictly — zero rejects — to a structurally valid image (magic, size,
     walkable record stream)
  B  loud rejection: a dump line the firmware does not know fails the
     render and names the setting (never a silent fall-back to defaults);
     a str that names a file instead of CLI text is refused
  C  overrides: appended CLI text applies last and changes the image
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


def main():
    ok = True

    # A: the stock dump renders strictly, zero rejects
    image = render_eeprom(CONFIGS / "stock_dump.txt")
    records = parse_records(image)
    pid_ver, pid_size = records[14]  # PG_PID_PROFILE
    print(f"[render-test] A image={len(image)}B records={len(records)} "
          f"pid-profile v{pid_ver} ({pid_size}B)")

    # B: an unknown setting must fail the render, loudly, naming it
    try:
        render_eeprom("set no_such_setting_xyz = 1\n")
        print("[render-test] B FAIL: unknown setting did not raise")
        ok = False
    except RuntimeError as e:
        print("[render-test] B rejected as expected")
        if "no_such_setting_xyz" not in str(e):
            print(f"[render-test] B FAIL: error does not name the setting: {e}")
            ok = False

    # B2: a str that names a file must be refused (a pathname applied as
    # CLI text would silently render a defaults image)
    try:
        render_eeprom(str(CONFIGS / "stock_dump.txt"))
        print("[render-test] B2 FAIL: str path did not raise")
        ok = False
    except ValueError:
        print("[render-test] B2 str-path trap refused as expected")

    # C: overrides append after the dump and change the image
    changed = render_eeprom(CONFIGS / "stock_dump.txt", "set motor_idle = 600\n")
    if changed == image:
        print("[render-test] C FAIL: override did not change the image")
        ok = False
    else:
        print("[render-test] C override applied")

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
