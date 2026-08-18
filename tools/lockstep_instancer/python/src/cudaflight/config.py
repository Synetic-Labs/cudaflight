# SPDX-License-Identifier: GPL-3.0-or-later
"""Render Betaflight CLI config text into a boot-ready eeprom image.

The CLI text (``dump all`` / ``diff all``) is the durable config format:
it is applied by setting NAME, so it survives firmware upgrades that
change struct layouts, parameter-group versions or ``EEPROM_CONF_VERSION``.
The binary eeprom image is a derived artifact, valid only for the exact
firmware build that wrote it — render it at use time, never commit it.
(A committed image one PG-version behind the wheel factory-resets the
whole config at boot, silently: pgLoad() rejects the stale group,
loadEEPROM() returns false, init.c calls resetEEPROM().)

Rendering is STRICT: any line the firmware rejects fails the render and
every rejected setting is named. A rejected line means the dump does not
match this firmware build — fix the dump, or repair renamed settings via
``overrides``. There is no lenient mode and no accept-list; a setting must
never silently stay at a compiled default.
"""

import ctypes
import os

from .lib import load_cpu

# eepromData is 32 KiB today; leave headroom so a larger future
# EEPROM_SIZE fails with the firmware's own "buffer too small" message
# instead of silent truncation.
_RENDER_BUF_CAP = 256 * 1024


def _as_text(source: "str | bytes | os.PathLike[str]") -> bytes:
    if isinstance(source, bytes):
        return source
    if isinstance(source, str):
        # a str is CLI TEXT; a file must come as a pathlib.Path. Guard the
        # trap: a pathname rendered as text would apply zero settings and
        # produce a defaults image without a word.
        if "\n" not in source and os.path.exists(source):
            raise ValueError(
                f"'{source}' is CLI text but names an existing file; pass a "
                "pathlib.Path to render the file's contents")
        return source.encode()
    return open(os.fspath(source), "rb").read()


def _reject_keys(error_lines: "list[str]") -> "list[str]":
    """Stable identity for each reject: the setting name for INVALID NAME
    errors, the command word for commands absent from this build
    (ERR_CMD_NA), the error text otherwise."""
    keys = set()
    for line in error_lines:
        marker = "INVALID NAME: "
        i = line.find(marker)
        if i >= 0:
            name = line[i + len(marker):].split("=")[0].strip().strip("#").strip()
            keys.add(name)
            continue
        marker = "ERR_CMD_NA: "
        i = line.find(marker)
        if i >= 0:
            words = line[i + len(marker):].split()
            keys.add("ERR_CMD_NA: " + (words[0] if words else ""))
            continue
        # other errors: key from the marker onward (a captured line can
        # carry unrelated response text before it)
        i = line.find("###ERROR")
        key = line[i:] if i >= 0 else line
        keys.add(key.strip().strip("#").strip())
    return sorted(keys)


def render_eeprom(
    dump: "str | bytes | os.PathLike[str]",
    overrides: "str | bytes | os.PathLike[str] | None" = None,
    *,
    lib_path: "str | os.PathLike[str] | None" = None,
) -> bytes:
    """Render CLI config text into a boot-ready eeprom image, strictly.

    ``dump`` is the config's CLI text (``dump all`` recommended: it pins
    every value explicitly, so compiled-default drift between firmware
    versions cannot change behaviour). ``overrides`` is optional extra CLI
    text appended after the dump — the place for sim-only lines and rename
    repairs; last value wins. Both accept a path, str, or bytes.

    Raises RuntimeError when the firmware rejects any line, naming every
    rejected setting. Returns the eeprom image bytes for ``eeprom=``
    (write to a temp file) or a direct RAM preload. Boots one CPU firmware
    instance internally: call before creating a fleet or after destroying
    one (one fleet per process).
    """
    text = _as_text(dump)
    if overrides is not None:
        text = text.rstrip(b"\n") + b"\n" + _as_text(overrides)

    lib = load_cpu(lib_path)
    if not hasattr(lib, "cpuflight_render_eeprom"):
        raise RuntimeError(
            "this libcpuflight.so predates cpuflight_render_eeprom (< 0.4.0); "
            "rebuild via tools/lockstep_instancer/build_multi.sh or upgrade "
            "the cudaflight wheel")

    out = (ctypes.c_uint8 * _RENDER_BUF_CAP)()
    out_len = ctypes.c_uint32(0)
    rc = lib.cpuflight_render_eeprom(
        text, len(text), 0, out, _RENDER_BUF_CAP, ctypes.byref(out_len))
    if rc != 0:
        raise RuntimeError(
            f"eeprom render failed: {lib.cpuflight_error().decode()}")

    rejects = _reject_keys(lib.cpuflight_render_errors().decode().splitlines())
    if rejects:
        raise RuntimeError(
            "eeprom render refused — the firmware rejected these, so their "
            "values would silently stay at compiled defaults. Fix the dump "
            "or repair renames via overrides:\n  " + "\n  ".join(rejects))

    return bytes(bytearray(out)[: out_len.value])


if __name__ == "__main__":
    # Diagnostic: list every setting the firmware rejects from a dump,
    # without failing. Usage: python -m cudaflight.config DUMP.txt [OVERRIDES.txt]
    import sys
    from pathlib import Path

    if len(sys.argv) not in (2, 3):
        print("usage: python -m cudaflight.config DUMP.txt [OVERRIDES.txt]",
              file=sys.stderr)
        sys.exit(1)
    text = _as_text(Path(sys.argv[1]))
    if len(sys.argv) == 3:
        text = text.rstrip(b"\n") + b"\n" + _as_text(Path(sys.argv[2]))

    # the firmware boot prints on fd 1; divert it to stderr so redirecting
    # this tool's stdout captures only the keys
    saved_stdout = os.dup(1)
    os.dup2(2, 1)
    try:
        lib = load_cpu()
        out = (ctypes.c_uint8 * _RENDER_BUF_CAP)()
        out_len = ctypes.c_uint32(0)
        rc = lib.cpuflight_render_eeprom(
            text, len(text), 0, out, _RENDER_BUF_CAP, ctypes.byref(out_len))
        if rc != 0:
            print(f"render failed: {lib.cpuflight_error().decode()}",
                  file=sys.stderr)
            sys.exit(1)
        keys = _reject_keys(lib.cpuflight_render_errors().decode().splitlines())
    finally:
        os.dup2(saved_stdout, 1)
        os.close(saved_stdout)
    for key in keys:
        print(key)
    sys.exit(2 if keys else 0)
