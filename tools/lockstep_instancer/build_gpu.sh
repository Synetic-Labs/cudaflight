#!/usr/bin/env bash
# Build the GPU (NVPTX) SITL_LOCKSTEP module + host runner.
#
# Same shape as build_multi.sh, different backend: every firmware TU is
# compiled to nvptx64 bitcode against freestanding shim headers, linked
# into one module, run through the SAME instancer pass, then linked with
# the device glue (per-thread delta, mini-libc, libdevice libm, physics,
# flight kernels), internalized, and codegenned to a cubin loaded by the
# driver-API runner.
#
# Output: obj/gpu/fw.cubin + obj/gpu/gpu_runner

set -euo pipefail
cd "$(dirname "$0")/../.."

OUT=obj/gpu
DEV=tools/lockstep_instancer/device
INSTANCER=tools/lockstep_instancer/instancer
HARNESS_SRCS='sitl_lockstep_main.c|sitl_lockstep_physics.c|sitl_lockstep_instance.c'
GPUARCH=${GPUARCH:-sm_120}
LIBDEVICE=/opt/cuda/nvvm/libdevice/libdevice.10.bc

# Device target: shim headers instead of glibc, -fno-builtin so libm/str
# calls stay calls (resolved by device_libc/device_libm) instead of
# becoming intrinsics the NVPTX backend can't select, RAM-backed EEPROM.
DEVFLAGS="-target nvptx64-nvidia-cuda -march=$GPUARCH -nostdlibinc \
 -isystem tools/lockstep_instancer/device/include -fno-builtin -DBFL_EEPROM_RAM -w"

mkdir -p "$OUT/bc" "$OUT/patched"

# NVPTX has no variable-length arrays. All VLA sites live on MSP/CLI cold
# paths that never execute on the GPU (serial is a sink); rewrite them to
# bounded fixed buffers in build-local copies. Firmware sources stay
# untouched.
VLA_PATCHED='src/main/msp/msp.c src/main/cli/cli.c src/main/drivers/dshot.c'
sed -E 's/char ([a-zA-Z_]+)\[len \+ 1\];/char \1[1024];/' \
    src/main/msp/msp.c > "$OUT/patched/msp.c"
sed -E 's/uint32_t defaultBufAligned\[[^]]+\];/uint32_t defaultBufAligned[1024];/' \
    src/main/cli/cli.c > "$OUT/patched/cli.c"
sed -E 's/int valuesAsIndexes\[size\];/int valuesAsIndexes[64];/' \
    src/main/drivers/dshot.c > "$OUT/patched/dshot.c"

if [ ! -x "$INSTANCER" ] || [ tools/lockstep_instancer/instancer.cpp -nt "$INSTANCER" ]; then
    echo "== building instancer"
    clang++ -O2 tools/lockstep_instancer/instancer.cpp \
        $(llvm-config --cxxflags --ldflags --libs core irreader bitwriter support transformutils) \
        -o "$INSTANCER"
fi

echo "== flag harvest"
if [ ! -f obj/multi/cmds.txt ]; then
    make TARGET=SITL_LOCKSTEP CROSS_CC=clang -B -n > obj/multi/cmds.txt 2>/dev/null
fi

echo "== compiling firmware TUs to nvptx bitcode"
: > "$OUT/bc_jobs.txt"
: > "$OUT/bc_list.txt"
grep -E ' -c -o ' obj/multi/cmds.txt | sed 's/^echo[^&]*&& //' | \
while IFS= read -r cmd; do
    if [[ "$cmd" =~ $HARNESS_SRCS ]]; then
        continue
    fi
    obj=$(sed -E 's/.* -c -o ([^ ]+) .*/\1/' <<<"$cmd")
    bc=$OUT/bc/$(sed 's|obj/main/SITL_LOCKSTEP/||; s|\.o$|.bc|; s|/|__|g' <<<"$obj")
    # retarget: drop host codegen/warning flags, add device flags
    cmd=$(sed -E "s@ -ffunction-sections@@; s@ -fdata-sections@@; \
                  s@ -MMD@@; s@ -MP@@; s@ -Werror@@; \
                  s@ -c -o [^ ]+ @ -c -emit-llvm -o $bc @" <<<"$cmd")
    cmd="clang $DEVFLAGS ${cmd#clang }"
    for f in $VLA_PATCHED; do
        if [[ "$cmd" == *"$f"* ]]; then
            # quote-includes resolve relative to the original directory
            cmd="${cmd/clang /clang -I./$(dirname "$f") }"
            cmd=${cmd//.\/$f/$OUT/patched/$(basename "$f")}
            cmd=${cmd// $f/ $OUT/patched/$(basename "$f")}
        fi
    done
    echo "$cmd" >> "$OUT/bc_jobs.txt"
    echo "$bc" >> "$OUT/bc_list.txt"
done
xargs -d '\n' -P "$(nproc)" -n 1 bash -c < "$OUT/bc_jobs.txt"
echo "   $(wc -l < "$OUT/bc_list.txt") TUs"

echo "== device glue to bitcode"
INC="-Isrc/main -Isrc/platform/SIMULATOR -Isrc/platform/SIMULATOR/include -Isrc/platform/SIMULATOR/target/SITL_LOCKSTEP"
clang $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_libc.c" -o "$OUT/device_libc.bc"
clang $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_libm.c" -o "$OUT/device_libm.bc"
clang $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/delta_gpu.c" -o "$OUT/delta_gpu.bc"
clang $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_flight.c" -o "$OUT/device_flight.bc"
clang $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm \
    src/platform/SIMULATOR/sitl_lockstep_physics.c -o "$OUT/physics_gpu.bc"

echo "== llvm-link + instancer"
llvm-link $(cat "$OUT/bc_list.txt") -o "$OUT/fw_gpu.bc"
"$INSTANCER" "$OUT/fw_gpu.bc" "$OUT/fw_gpu_inst.bc"
llvm-link "$OUT/fw_gpu_inst.bc" \
    "$OUT/delta_gpu.bc" "$OUT/device_flight.bc" "$OUT/physics_gpu.bc" \
    "$OUT/device_libc.bc" "$OUT/device_libm.bc" \
    --override "$LIBDEVICE" \
    -o "$OUT/whole.bc"

echo "== internalize + DCE + codegen"
KEEP="bfInstanceInit,bfBoot,bfRun,bfFinish,bfSnapshot,bfReset,bfStep,__bf_image,__bf_image_size,__bf_image_align,__bf_state_size,__bf_act_dim,__bf_obs_dim,__bf_inst_base,__bf_inst_stride,__bf_inst_count,__bf_relocs,__bf_reloc_count,__bf_instanced_build"
opt -passes='internalize,globaldce' -internalize-public-api-list="$KEEP" \
    "$OUT/whole.bc" -o "$OUT/whole_dce.bc"
clang -target nvptx64-nvidia-cuda -march=$GPUARCH -O3 -x ir "$OUT/whole_dce.bc" -S -o "$OUT/fw.ptx"
ptxas -arch=$GPUARCH -O3 "$OUT/fw.ptx" -o "$OUT/fw.cubin"
echo "   $(grep -c '^\.visible \.entry\|^.visible .entry' "$OUT/fw.ptx" || true) kernels, $(wc -c < "$OUT/fw.cubin") B cubin"

echo "== host runner + bfgym shared lib"
g++ -O2 tools/lockstep_instancer/gpu_runner.cpp -I/opt/cuda/include -lcuda -o "$OUT/gpu_runner"
g++ -O2 -shared -fPIC tools/lockstep_instancer/bfgym.cpp -I/opt/cuda/include -lcuda -o "$OUT/libbfgym.so"

echo "== done: $OUT/gpu_runner --module $OUT/fw.cubin"
