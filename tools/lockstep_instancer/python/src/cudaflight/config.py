# SPDX-License-Identifier: GPL-3.0-or-later
"""Render Betaflight CLI config text into a boot-ready eeprom image.

The CLI text (``dump all`` from the drone) is the durable config format and
the ONLY per-drone input. The binary eeprom image is a derived artifact,
valid only for the exact firmware build that wrote it — render it at use
time, never commit it. (A committed image one PG-version behind the wheel
factory-resets the whole config at boot, silently: pgLoad() rejects the
stale group, loadEEPROM() returns false, init.c calls resetEEPROM().)

Safety model — three checks, all mechanical:

1. VERSION GATE. The dump's header names the exact firmware it came from
   (release string + git commit). The render refuses any dump whose
   firmware does not match this wheel's firmware. At exact parity a
   rejected line cannot be a renamed setting (one release = one namespace)
   and cannot be an out-of-range value from a machine dump (the drone's
   own firmware enforced the same ranges) — so every reject is a hardware
   feature this SITL build compiles out. Those are skipped and reported.

2. STRICT OVERRIDES. ``overrides`` is hand-written CLI text appended after
   the dump (sim-only lines; last value wins). Hand-written lines get no
   leniency: the render runs twice, and any error the overrides add over
   the dump-only run fails the render. A typo cannot pass as a gap.

3. ROUND-TRIP VERIFICATION. After the save, the firmware emits its own
   ``dump all``. Every setting the dump names that this build knows must
   hold the dump's value (or the override's, where overridden). Any
   difference fails the render.
"""

import ctypes
import logging
import os
import re

from .lib import load_cpu

logger = logging.getLogger("cudaflight.config")

# eepromData is 32 KiB today; leave headroom so a larger future
# EEPROM_SIZE fails with the firmware's own "buffer too small" message
# instead of silent truncation.
_RENDER_BUF_CAP = 256 * 1024

# "# Betaflight / STM32G47X (G473) 2026.6.0-alpha May 15 2026 / 06:14:55 (e92c10887) MSP API: 1.48"
_HEADER_RE = re.compile(
    r"^# Betaflight / (?P<target>\S+) \((?P<board>\S+)\) (?P<release>\S+) "
    r"(?P<date>[A-Z][a-z]{2}\s+\d+ \d{4}) / [\d:]+ \((?P<rev>[0-9a-fA-F]+|norevision)\)",
    re.M,
)


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


def parse_header(text: str) -> "dict[str, str] | None":
    """The identity a `dump all` header carries: target, board, release,
    build date, and the git revision the firmware was built from."""
    m = _HEADER_RE.search(text)
    return m.groupdict() if m else None


def _wheel_base_rev() -> "str | None":
    """The Betaflight base commit this wheel was built on, from the wheel's
    own version (e.g. '0.5.0+bf.e92c1088' -> 'e92c1088'). None for local
    source checkouts and wheels without a base tag."""
    try:
        from importlib.metadata import version
        m = re.search(r"\+bf\.([0-9a-f]+)", version("cudaflight"))
        return m.group(1) if m else None
    except Exception:
        return None


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


_SET_RE = re.compile(r"^set (\w+)\s*=\s*(.*?)\s*$")
_SECTION_RE = re.compile(r"^(profile|rateprofile) (\d+)\s*$")


def _section_maps(text: str) -> "dict[str, dict[str, str]]":
    """(section -> {setting -> value}) from CLI text. `dump all` scopes
    per-profile settings under `profile N` / `rateprofile N` lines; the
    same tracking applied to both sides makes the maps comparable."""
    maps: dict[str, dict[str, str]] = {}
    section = "master"
    for raw in text.splitlines():
        line = raw.strip()
        m = _SECTION_RE.match(line)
        if m:
            section = f"{m.group(1)} {m.group(2)}"
            continue
        m = _SET_RE.match(line)
        if m:
            maps.setdefault(section, {})[m.group(1)] = m.group(2)
    return maps


def _norm(value: str) -> str:
    v = value.strip().lower()
    try:
        return repr(float(v))
    except ValueError:
        return v


def _values_equal(a: str, b: str) -> bool:
    if a.strip().lower() == b.strip().lower():
        return True
    return _norm(a) == _norm(b)


def _render_once(lib, text: bytes) -> "tuple[bytes, list[str], str]":
    out = (ctypes.c_uint8 * _RENDER_BUF_CAP)()
    out_len = ctypes.c_uint32(0)
    rc = lib.cpuflight_render_eeprom(
        text, len(text), 0, out, _RENDER_BUF_CAP, ctypes.byref(out_len))
    if rc != 0:
        raise RuntimeError(
            f"eeprom render failed: {lib.cpuflight_error().decode()}")
    errors = lib.cpuflight_render_errors().decode().splitlines()
    roundtrip = lib.cpuflight_render_dump().decode()
    return bytes(bytearray(out)[: out_len.value]), errors, roundtrip


def render_eeprom(
    dump: "str | bytes | os.PathLike[str]",
    overrides: "str | bytes | os.PathLike[str] | None" = None,
    *,
    lib_path: "str | os.PathLike[str] | None" = None,
) -> bytes:
    """Render a drone's `dump all` into a boot-ready eeprom image.

    ``dump`` must be complete `dump all` text (it pins every value
    explicitly and carries the firmware-identity header the version gate
    needs). ``overrides`` is optional hand-written CLI text appended after
    the dump — sim-only lines and nothing else; last value wins; strict.
    Both accept a path, str, or bytes.

    Raises RuntimeError when the dump's firmware is not this wheel's
    firmware, when an override line does not apply, or when any sim-known
    setting fails to hold its expected value. Hardware settings this SITL
    build compiles out are skipped and logged (they cannot be anything
    else at exact version parity).

    Returns the image bytes for ``eeprom=`` (write to a temp file) or a
    direct RAM preload. Boots one CPU firmware instance per pass: call
    before creating a fleet or after destroying one (one fleet per
    process).
    """
    dump_text = _as_text(dump)
    header = parse_header(dump_text.decode(errors="replace"))
    if header is None:
        raise RuntimeError(
            "no Betaflight version header found — the input must be complete "
            "`dump all` text from the drone (the header carries the firmware "
            "identity the version gate checks)")

    lib = load_cpu(lib_path)
    if not hasattr(lib, "cpuflight_render_dump"):
        raise RuntimeError(
            "this libcpuflight.so predates the render round-trip (< 0.5.0); "
            "rebuild via tools/lockstep_instancer/build_multi.sh or upgrade "
            "the cudaflight wheel")

    image, errors, roundtrip = _render_once(lib, dump_text)
    gaps = _reject_keys(errors)

    # --- version gate -----------------------------------------------------
    fw = parse_header(roundtrip)
    if fw is None:
        raise RuntimeError("firmware round-trip dump carries no version header")
    if header["release"] != fw["release"]:
        raise RuntimeError(
            f"version gate: the dump is from Betaflight {header['release']} "
            f"(built {header['date']}, rev {header['rev']}); this wheel's "
            f"firmware is {fw['release']}. Use the cudaflight wheel built on "
            f"the dump's base commit — or re-flash the drone and re-dump.")
    dump_rev = header["rev"]
    for what, fw_rev in (("firmware rev", fw["rev"]),
                         ("wheel base", _wheel_base_rev())):
        if fw_rev is None or "norevision" in (dump_rev, fw_rev):
            continue
        n = min(len(dump_rev), len(fw_rev))
        if dump_rev[:n].lower() != fw_rev[:n].lower():
            raise RuntimeError(
                f"version gate: same release string ({fw['release']}) but "
                f"different builds — dump rev {dump_rev}, {what} {fw_rev}. "
                f"Use the wheel built on the dump's base commit.")

    # --- strict overrides: a second pass must add zero new errors ---------
    if overrides is not None:
        combined = dump_text.rstrip(b"\n") + b"\n" + _as_text(overrides)
        image, errors2, roundtrip = _render_once(lib, combined)
        new = sorted(set(_reject_keys(errors2)) - set(gaps))
        if new:
            raise RuntimeError(
                "override line(s) did not apply — overrides are hand-written "
                "and get no leniency:\n  " + "\n  ".join(new))

    # --- gaps are hardware-only at parity; report them --------------------
    if gaps:
        logger.info("render: %d hardware setting(s) absent from this SITL "
                    "build were skipped (run `python -m cudaflight.config` "
                    "on the dump to list them)", len(gaps))
        logger.debug("render gaps: %s", ", ".join(gaps))

    # --- round-trip verification ------------------------------------------
    expected = _section_maps(dump_text.decode(errors="replace"))
    got = _section_maps(roundtrip)
    override_map: dict[str, str] = {}
    if overrides is not None:
        for sec_map in _section_maps(_as_text(overrides).decode(errors="replace")).values():
            override_map.update(sec_map)

    mismatches = []
    for section, settings in expected.items():
        for key, value in settings.items():
            want = override_map.get(key, value)
            have = got.get(section, {}).get(key)
            if have is not None and not _values_equal(want, have):
                mismatches.append(f"{section}: {key} = {have} (dump wants {want})")
    for key, value in override_map.items():
        held = [m.get(key) for m in got.values() if key in m]
        if held and not any(_values_equal(value, h) for h in held):
            mismatches.append(f"override: {key} = {held[0]} (override wants {value})")
    if mismatches:
        raise RuntimeError(
            "round-trip verification failed — the rendered config does not "
            "hold these values:\n  " + "\n  ".join(mismatches))

    return image


if __name__ == "__main__":
    # Inspector: identity, gate, hardware gaps, verification — without
    # having to boot a fleet. Exit 0 = renders clean under this wheel.
    import sys
    from pathlib import Path

    if len(sys.argv) not in (2, 3):
        print("usage: python -m cudaflight.config DUMP.txt [OVERRIDES.txt]",
              file=sys.stderr)
        sys.exit(1)

    logging.basicConfig(level=logging.DEBUG, format="%(message)s")
    dump_path = Path(sys.argv[1])
    overrides_path = Path(sys.argv[2]) if len(sys.argv) == 3 else None

    header = parse_header(_as_text(dump_path).decode(errors="replace")) or {}
    print(f"dump identity : {header.get('release', '?')} rev {header.get('rev', '?')} "
          f"({header.get('target', '?')}/{header.get('board', '?')}, built {header.get('date', '?')})")

    # the firmware boot prints on fd 1; divert it to stderr so this tool's
    # stdout stays a clean report
    saved_stdout = os.dup(1)
    os.dup2(2, 1)
    try:
        image = render_eeprom(dump_path, overrides_path)
    except (RuntimeError, ValueError) as e:
        os.dup2(saved_stdout, 1)
        os.close(saved_stdout)
        print(f"REFUSED: {e}")
        sys.exit(2)
    os.dup2(saved_stdout, 1)
    os.close(saved_stdout)
    print(f"render OK     : {len(image)} bytes")
    sys.exit(0)
