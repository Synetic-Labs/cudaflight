#!/usr/bin/env bash
# Build the multi-instance SITL_LOCKSTEP binary.
#
# Pipeline: every firmware TU -> LLVM bitcode (same flags as the normal
# clang build, harvested from a make dry run), llvm-link into one module,
# instancer rewrites firmware state accesses to honour __bf_delta, then
# codegen and relink against the native harness objects.
#
# Output: obj/multi/betaflight_SITL_LOCKSTEP_MULTI

set -euo pipefail
cd "$(dirname "$0")/../.."

OUT=obj/multi
INSTANCER=tools/lockstep_instancer/instancer
HARNESS_SRCS='sitl_lockstep_main.c|sitl_lockstep_physics.c|sitl_lockstep_instance.c'

mkdir -p "$OUT/bc"

if [ ! -x "$INSTANCER" ] || [ tools/lockstep_instancer/instancer.cpp -nt "$INSTANCER" ]; then
    echo "== building instancer"
    clang++ -O2 tools/lockstep_instancer/instancer.cpp \
        $(llvm-config --cxxflags --ldflags --libs core irreader bitwriter support transformutils) \
        -o "$INSTANCER"
fi

echo "== native build (harness objects + flag harvest)"
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -j"$(nproc)" > /dev/null
make TARGET=SITL_LOCKSTEP CROSS_CC=clang -B -n > "$OUT/cmds.txt" 2>/dev/null

echo "== compiling firmware TUs to bitcode"
: > "$OUT/bc_jobs.txt"
: > "$OUT/bc_list.txt"
grep -E ' -c -o ' "$OUT/cmds.txt" | sed 's/^echo[^&]*&& //' | \
while IFS= read -r cmd; do
    if [[ "$cmd" =~ $HARNESS_SRCS ]]; then
        continue
    fi
    obj=$(sed -E 's/.* -c -o ([^ ]+) .*/\1/' <<<"$cmd")
    bc=$OUT/bc/$(sed 's|obj/main/SITL_LOCKSTEP/||; s|\.o$|.bc|; s|/|__|g' <<<"$obj")
    sed -E "s@ -c -o [^ ]+ @ -c -emit-llvm -o $bc @" <<<"$cmd" >> "$OUT/bc_jobs.txt"
    echo "$bc" >> "$OUT/bc_list.txt"
done
xargs -d '\n' -P "$(nproc)" -n 1 bash -c < "$OUT/bc_jobs.txt"
echo "   $(wc -l < "$OUT/bc_list.txt") TUs"

echo "== llvm-link + instancer + codegen"
llvm-link $(cat "$OUT/bc_list.txt") -o "$OUT/fw.bc"
"$INSTANCER" "$OUT/fw.bc" "$OUT/fw_inst.bc"
# CPU definition of __bf_delta_load(), inlined into the firmware module
clang -O2 -c -emit-llvm tools/lockstep_instancer/delta_cpu.c -o "$OUT/delta_cpu.bc"
llvm-link "$OUT/fw_inst.bc" "$OUT/delta_cpu.bc" -o "$OUT/fw_final.bc"
# function/data sections so the final --gc-sections can strip dead code,
# matching the per-TU build (some firmware references resolve to nothing
# on SITL and only link because they are unreachable)
clang -O2 -ffunction-sections -fdata-sections -c "$OUT/fw_final.bc" -o "$OUT/fw_inst.o"

echo "== linking"
linkcmd=$(grep -E '^clang -o obj/main/betaflight_SITL_LOCKSTEP\.elf' "$OUT/cmds.txt" | head -1)
flags=$(sed -E 's|^clang -o [^ ]+ ||; s|obj/main/SITL_LOCKSTEP/[^ ]+\.o ?||g' <<<"$linkcmd")
clang -o "$OUT/betaflight_SITL_LOCKSTEP_MULTI" \
    "$OUT/fw_inst.o" \
    obj/main/SITL_LOCKSTEP/SIMULATOR/sitl_lockstep_main.o \
    obj/main/SITL_LOCKSTEP/SIMULATOR/sitl_lockstep_physics.o \
    obj/main/SITL_LOCKSTEP/SIMULATOR/sitl_lockstep_instance.o \
    $flags

echo "== done: $OUT/betaflight_SITL_LOCKSTEP_MULTI"
