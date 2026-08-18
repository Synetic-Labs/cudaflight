#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Build betaflight_realtime_server.elf: the single-instance CPU SITL_LOCKSTEP
# real-time pose server — real Betaflight firmware + first-principles quad
# physics on the CPU, served over localhost TCP to an external renderer client.
#
# We reuse the firmware's exact compile + link (LTO + sitl.ld) by harvesting the
# commands from a make dry-run and just swapping the harness main
# (sitl_lockstep_main.c, the golden-trace driver) for our server main
# (realtime_server.c). This keeps LTO's dead-code elimination (which the
# SITL target relies on to drop hardware-only references) and the PG config
# registry, both of which a no-main shared library cannot preserve.
#
# Output: obj/main/betaflight_realtime_server.elf

set -euo pipefail
cd "$(dirname "$0")/../.."

SRCDIR=tools/realtime_server
OUT=obj/main/betaflight_realtime_server.elf

echo "== native build (firmware objects + harness exe + flag harvest)"
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -j"$(nproc)" >/dev/null
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -B -n > obj/realtime_cmds.txt

maincc=$(grep -E ' -c -o .*sitl_lockstep_main\.c' obj/realtime_cmds.txt | sed 's/^echo[^&]*&& //' | head -1)
[ -n "$maincc" ] || { echo "compile-command harvest from obj/realtime_cmds.txt failed"; exit 1; }
main_obj=$(grep -oE 'obj/[^ ]*sitl_lockstep_main\.o' <<<"$maincc" | head -1)

# compile both sources with the harness main's exact flags
objs=""
for src in realtime_server.c firstprinciples_physics.c; do
    echo "== compiling $src (reusing the harness main's flags)"
    obj="${main_obj%sitl_lockstep_main.o}${src%.c}.o"
    cc=$(sed -E "s@[^ ]*sitl_lockstep_main\.o@$obj@; \
                 s@[^ ]*sitl_lockstep_main\.c@$SRCDIR/$src@" <<<"$maincc")
    eval "$cc"
    objs="$objs $obj"
done

echo "== linking $OUT (swap harness main -> server main, + first-principles physics)"
linkcmd=$(grep -E '^clang -o obj/main/betaflight_SITL_LOCKSTEP\.elf' obj/realtime_cmds.txt | head -1)
[ -n "$linkcmd" ] || { echo "link-command harvest from obj/realtime_cmds.txt failed"; exit 1; }
relink=$(sed -E "s@$main_obj@$objs@; s@obj/main/betaflight_SITL_LOCKSTEP\.elf@$OUT@" <<<"$linkcmd")
eval "$relink"

echo "== done: $OUT"
