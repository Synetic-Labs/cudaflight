#!/usr/bin/env bash
# Build betaflight_realtime_server.elf: the single-instance CPU SITL_LOCKSTEP
# real-time pose server for the GTA V mod (AirDojo-GTA) — real Betaflight
# firmware + first-principles quad physics on the CPU, served over localhost TCP.
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

SRCDIR=tools/gtav_realtime_server
OUT=obj/main/betaflight_realtime_server.elf

echo "== native build (firmware objects + harness exe + flag harvest)"
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -j"$(nproc)" >/dev/null
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -B -n > obj/realtime_cmds.txt 2>/dev/null

echo "== compiling realtime_server.c (reusing the harness main's flags)"
maincc=$(grep -E ' -c -o .*sitl_lockstep_main\.c' obj/realtime_cmds.txt | sed 's/^echo[^&]*&& //' | head -1)
main_obj=$(grep -oE 'obj/[^ ]*sitl_lockstep_main\.o' <<<"$maincc" | head -1)
server_obj="${main_obj%sitl_lockstep_main.o}realtime_server.o"
servercc=$(sed -E "s@[^ ]*sitl_lockstep_main\.o@$server_obj@; \
                   s@[^ ]*sitl_lockstep_main\.c@$SRCDIR/realtime_server.c@" <<<"$maincc")
eval "$servercc"

echo "== compiling firstprinciples_physics.c (5\" quad model, reuses harness flags)"
fp_obj="${main_obj%sitl_lockstep_main.o}firstprinciples_physics.o"
fpcc=$(sed -E "s@[^ ]*sitl_lockstep_main\.o@$fp_obj@; \
                   s@[^ ]*sitl_lockstep_main\.c@$SRCDIR/firstprinciples_physics.c@" <<<"$maincc")
eval "$fpcc"

echo "== linking $OUT (swap harness main -> server main, + first-principles physics)"
linkcmd=$(grep -E '^clang -o obj/main/betaflight_SITL_LOCKSTEP\.elf' obj/realtime_cmds.txt | head -1)
relink=$(sed -E "s@$main_obj@$server_obj $fp_obj@; s@obj/main/betaflight_SITL_LOCKSTEP\.elf@$OUT@" <<<"$linkcmd")
eval "$relink"

echo "== done: $OUT"
