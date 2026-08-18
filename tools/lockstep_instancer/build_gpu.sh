#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Build the GPU (NVPTX) SITL_LOCKSTEP module + host runner.
#
# Same shape as build_multi.sh, different backend: every firmware TU is
# compiled to nvptx64 bitcode against freestanding shim headers, linked
# into one module, run through the SAME instancer pass, then linked with
# the device glue (per-thread delta, mini-libc, libdevice libm, physics,
# flight kernels), internalized, and codegenned to a cubin loaded by the
# driver-API runner.
#
# Output: obj/gpu/fw.fatbin + obj/gpu/gpu_runner

set -euo pipefail
cd "$(dirname "$0")/../.."

OUT=obj/gpu
DEV=tools/lockstep_instancer/device
INSTANCER=tools/lockstep_instancer/instancer
HARNESS_SRCS='sitl_lockstep_main.c|sitl_lockstep_physics.c|sitl_lockstep_instance.c'
# Per-TU bitcode compile target. Default to the lowest SM in the ARCHS
# matrix below: PTX is forward-compatible (lower-arch PTX runs on higher-arch
# devices), so compiling firmware bitcode at sm_89 lets ptxas codegen valid
# cubins for sm_89 AND sm_120 from the same IR. Override only if you know
# you need newer intrinsics in the firmware itself.
GPUARCH=${GPUARCH:-sm_89}
# Final fatbin target list: one cubin per SM, combined into fw.fatbin so the
# driver picks the closest match at load time. Default covers the GPUs we
# train on: sm_89 (RTX 4090), sm_120 (RTX 5090, RTX PRO 6000).
ARCHS=${ARCHS:-"sm_89 sm_120"}
CUDA_HOME=${CUDA_HOME:-/opt/cuda}
LIBDEVICE=$CUDA_HOME/nvvm/libdevice/libdevice.10.bc

CLANG=${CLANG:-clang}; CLANGXX=${CLANGXX:-clang++}; OPT=${OPT:-opt}
LLVMLINK=${LLVMLINK:-llvm-link}; LLVMCONFIG=${LLVMCONFIG:-llvm-config}

# Device target: shim headers instead of glibc, -fno-builtin so libm/str
# calls stay calls (resolved by device_libc/device_libm) instead of
# becoming intrinsics the NVPTX backend can't select, RAM-backed EEPROM.
# (The self-relative lookup tables clang's -O2 emits are un-done later by the
# instancer's delinearizeRelLookupTables — NVPTX can't codegen them, and LLVM
# 20 has no flag to suppress the RelLookupTableConverter pass.)
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

# Always rebuild: the instancer must be linked against the SAME LLVM whose
# bitcode it rewrites.
echo "== building instancer (against $("$LLVMCONFIG" --version))"
"$CLANGXX" -O2 tools/lockstep_instancer/instancer.cpp \
    $("$LLVMCONFIG" --cxxflags --ldflags --libs core irreader bitwriter support transformutils) \
    -o "$INSTANCER"

echo "== flag harvest"
if [ ! -f obj/multi/cmds.txt ]; then
    mkdir -p obj/multi
    # A real build must precede the -B -n harvest: in a pristine tree the
    # dry run prints unresolved vpath sources (common/chirp.c instead of
    # ./src/main/common/chirp.c) and every bitcode compile fails.
    make TARGET=SITL_LOCKSTEP CROSS_CC=clang -j"$(nproc)" > /dev/null
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
    cmd="$CLANG $DEVFLAGS ${cmd#clang }"
    for f in $VLA_PATCHED; do
        if [[ "$cmd" == *"$f"* ]]; then
            # quote-includes resolve relative to the original directory
            cmd="${cmd/$CLANG /$CLANG -I./$(dirname "$f") }"
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
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_libc.c" -o "$OUT/device_libc.bc"
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_libm.c" -o "$OUT/device_libm.bc"
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/delta_gpu.c" -o "$OUT/delta_gpu.bc"
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_flight.c" -o "$OUT/device_flight.bc"
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm \
    src/platform/SIMULATOR/sitl_lockstep_physics.c -o "$OUT/physics_gpu.bc"

echo "== llvm-link firmware"
"$LLVMLINK" $(cat "$OUT/bc_list.txt") -o "$OUT/fw_gpu.bc"

echo "== instancer + link"
"$INSTANCER" "$OUT/fw_gpu.bc" "$OUT/fw_gpu_inst.bc"
"$LLVMLINK" "$OUT/fw_gpu_inst.bc" \
    "$OUT/delta_gpu.bc" "$OUT/device_flight.bc" "$OUT/physics_gpu.bc" \
    "$OUT/device_libc.bc" "$OUT/device_libm.bc" \
    --override "$LIBDEVICE" \
    -o "$OUT/whole.bc"

echo "== internalize + DCE + codegen"
KEEP="bfInstanceInit,bfBoot,bfRun,bfFinish,bfSnapshot,bfReset,bfStep,bfFwStep,bfSetAux,bfFwStepGradFD,bfFwStepJacFD,bfFwStepJacFDPure,bfRateEval,bfLoadEeprom,bfOsdSnapshot,bfSetBase,__bf_image,__bf_image_size,__bf_image_align,__bf_state_size,__bf_act_dim,__bf_obs_dim,__bf_aux_dim,__bf_osd_rows,__bf_osd_cols,__bf_inst_base,__bf_inst_stride,__bf_inst_count,__bf_relocs,__bf_reloc_count,__bf_instanced_build,__bf_full_relocs,__bf_full_reloc_count"
"$OPT" -passes='internalize,globaldce' -internalize-public-api-list="$KEEP" \
    "$OUT/whole.bc" -o "$OUT/whole_dce.bc"
# Per-arch codegen: emit one PTX + cubin per SM in $ARCHS, then combine
# everything into a single fatbin the CUDA driver picks the right slice
# from at cuModuleLoad time.
FATBIN_IMAGES=""
n_kernels=0
for arch in $ARCHS; do
    "$CLANG" -target nvptx64-nvidia-cuda -march=$arch -O3 -x ir \
        "$OUT/whole_dce.bc" -S -o "$OUT/fw.$arch.ptx"
    ptxas -arch=$arch -O3 --split-compile=0 "$OUT/fw.$arch.ptx" -o "$OUT/fw.$arch.cubin"
    sm_num=${arch#sm_}
    FATBIN_IMAGES="$FATBIN_IMAGES --image3=kind=elf,sm=$sm_num,file=$OUT/fw.$arch.cubin"
    [ "$n_kernels" = 0 ] && n_kernels=$(grep -c '^\.visible \.entry' "$OUT/fw.$arch.ptx" || true)
done
fatbinary --64 --create="$OUT/fw.fatbin" $FATBIN_IMAGES
echo "   $n_kernels kernels, $(wc -c < "$OUT/fw.fatbin") B fatbin (${ARCHS// /+})"

echo "== host runner + cudaflight shared lib"
# Link against the toolkit's driver stub (-L stubs): systems like WSL ship
# only libcuda.so.1, no libcuda.so dev symlink. Runtime still resolves the
# real driver's libcuda.so.1.
g++ -O2 tools/lockstep_instancer/gpu_runner.cpp -I"$CUDA_HOME/include" \
    -L"$CUDA_HOME/lib64/stubs" -lcuda -o "$OUT/gpu_runner"
g++ -O2 -shared -fPIC tools/lockstep_instancer/cudaflight.cpp -I"$CUDA_HOME/include" \
    -L"$CUDA_HOME/lib64/stubs" -lcuda -o "$OUT/libcudaflight.so"

echo "== done: $OUT/gpu_runner --module $OUT/fw.fatbin"
