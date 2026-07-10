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
LIBDEVICE=/opt/cuda/nvvm/libdevice/libdevice.10.bc

# Differentiable build: compile and link the whole firmware with the LLVM 20
# slot so the Enzyme-20 autodiff plugin can run on the linked bitcode (system
# clang is 22, unsupported by Enzyme).
# Default 1: Enzyme is the canonical gradient path now that it builds cleanly, so
# the standard module carries the reverse-mode kernels (bfFwStepGrad +
# bfFwStepJacGradPure). cudaflight_create requires the pure Enzyme Jacobian, so the
# cudaflight RL env needs a DIFF=1 module. DIFF=0 still builds a valid module for the
# standalone determinism harness (gpu_runner, which never calls cudaflight_create) —
# it just omits the Enzyme kernels and so can't back the differentiable env.
DIFF=${DIFF:-1}
LLVMDIR=${LLVMDIR:-/usr/lib/llvm/20}
ENZYME=${ENZYME:-tools/enzyme_build/Enzyme/LLVMEnzyme-20.so}
if [ "$DIFF" = 1 ]; then
    CLANG=$LLVMDIR/bin/clang; CLANGXX=$LLVMDIR/bin/clang++
    OPT=$LLVMDIR/bin/opt; LLVMLINK=$LLVMDIR/bin/llvm-link; LLVMCONFIG=$LLVMDIR/bin/llvm-config
    [ -f "$ENZYME" ] || { echo "Enzyme plugin not found at $ENZYME"; exit 1; }
else
    CLANG=clang; CLANGXX=clang++; OPT=opt; LLVMLINK=llvm-link; LLVMCONFIG=llvm-config
fi

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
# bitcode it rewrites (DIFF toggles between the 20 and 22 slots).
if true; then
    echo "== building instancer (against $("$LLVMCONFIG" --version))"
    "$CLANGXX" -O2 tools/lockstep_instancer/instancer.cpp \
        $("$LLVMCONFIG" --cxxflags --ldflags --libs core irreader bitwriter support transformutils) \
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
# device_diff.c (the autodiff control core) is linked BEFORE the instancer
# (see below), so Enzyme sees distinct state globals. -O1 keeps the
# __enzyme_autodiff call and fwCore intact for the pass.
if [ "$DIFF" = 1 ]; then
    "$CLANG" $DEVFLAGS -O1 -ffast-math -std=gnu17 $INC -c -emit-llvm "$DEV/device_diff.c" -o "$OUT/device_diff.bc"
fi
"$CLANG" $DEVFLAGS -O3 -ffast-math -std=gnu17 $INC -c -emit-llvm \
    src/platform/SIMULATOR/sitl_lockstep_physics.c -o "$OUT/physics_gpu.bc"

echo "== llvm-link firmware (+ diff core) "
# Link the diff core in with the firmware TUs so Enzyme (next) sees the real
# control functions AND the per-instance state as DISTINCT global symbols it
# can mark inactive. Critically this is BEFORE the instancer: afterwards all
# state is one rebased blob and Enzyme's pointer inversion crashes.
DIFF_BC=""
[ "$DIFF" = 1 ] && DIFF_BC="$OUT/device_diff.bc"
"$LLVMLINK" $(cat "$OUT/bc_list.txt") $DIFF_BC -o "$OUT/fw_gpu.bc"

# Enzyme autodiff pass — BEFORE the instancer. Generates the reverse-mode body
# of bfFwStepGrad (the __enzyme_autodiff call in device_diff.c). preserve-nvvm
# first so the __enzyme_inactive_global_* markers freeze the stateful config/
# runtime globals (the bare "enzyme" pass does not run preserve-nvvm).
# loose-types: assume a type for any untyped byte memcpy instead of erroring.
INST_IN="$OUT/fw_gpu.bc"
if [ "$DIFF" = 1 ]; then
    # Freeze the parameter-group config globals (*_System/_Copy/_Registry):
    # stamp them enzyme_inactive (no shadow / zero derivative) and
    # enzyme_ta_norecur (don't let TypeAnalysis refine their union-laden config
    # structs). Without the latter, TA's fixpoint oscillates on those unions and
    # never terminates (accelerometerConfig_System alone was ~40% of an
    # overnight, unfinished run); with it the pass finishes in <1s.
    # Then mark_active_pipeline: enzyme_active on the no-arg control functions
    # (so the global-threaded pipeline is differentiated, not skipped as
    # argument-constant -> zero gradient) AND enzyme_ta_norecur on @motor /
    # @motor_disarmed. Those motor-output globals are [8 x float] indexed both by
    # constant byte offsets and by a VARIABLE loop index (mixTable/stopMotors);
    # the variable index injects a [-1] wildcard, so TA juggles two equivalent
    # encodings of the element type ([-1,-1]:Float vs enumerated [-1,k]:Float)
    # and the worklist oscillates forever (the second non-termination, distinct
    # from the config-union one above). norecur pins their known [8 x float] type.
    # Finally register_shadow_globals: a shared module-level shadow per control-
    # state global so the adjoint threads across the separately-differentiated
    # pipeline functions instead of each getting a throwaway local shadow.
    echo "== freeze config + mark active pipeline + shadow globals (pre-Enzyme)"
    # NB register_shadow_globals runs BEFORE mark_active_pipeline: it only
    # matches `... zeroinitializer` global lines, and mark_active_pipeline
    # appends !enzyme_ta_norecur to @motor (breaking that match if it ran first).
    # Shadowed globals = the control-state chain the adjoint must thread through.
    # NB the throttle stick has NO useful gradient and is deliberately not chased:
    # rcCommand[THROTTLE] = rcLookupThrottle() is an int16 lookup table (rc.c), so
    # d(motor)/d(throttle) is 0 a.e. (Enzyme is correct; the FD oracle only sees a
    # secant across the quantization). Same class as the motor-output int16
    # quantization that fwCore sidesteps via bflGetMotorsRaw.
    "$LLVMDIR/bin/llvm-dis" "$OUT/fw_gpu.bc" -o - \
        | python3 tools/lockstep_instancer/freeze_config_globals.py \
        | python3 tools/lockstep_instancer/register_shadow_globals.py \
              rcData rcCommand pidData motor setpointRate \
              rcThrottle rcDeflection rcDeflectionSmoothed rcDeflectionAbs \
              feedforwardRaw feedforwardSmoothed rawSetpoint \
        | python3 tools/lockstep_instancer/mark_active_pipeline.py \
        | "$LLVMDIR/bin/llvm-as" -o "$OUT/fw_gpu_frozen.bc"

    echo "== Enzyme autodiff pass (pre-instancer)"
    # assume-unknown-nofree: device libc/libm (abs, ...) are linked AFTER Enzyme,
    # so here they are body-less declarations; none of them free memory.
    "$OPT" "$OUT/fw_gpu_frozen.bc" -load-pass-plugin="$ENZYME" \
        -enzyme-loose-types -enzyme-assume-unknown-nofree=1 \
        -passes="preserve-nvvm,enzyme" -o "$OUT/fw_ad.bc"
    # Capture the symbol list first: piping llvm-nm straight into `grep -q`
    # lets grep exit on the first match and SIGPIPE llvm-nm, which under
    # `set -o pipefail` looks like a failure. Match a word boundary so
    # bfFwStepGradFD (the finite-difference kernel) doesn't count.
    ad_syms="$("$LLVMDIR/bin/llvm-nm" "$OUT/fw_ad.bc" 2>/dev/null || true)"
    if ! grep -qw bfFwStepGrad <<<"$ad_syms"; then
        echo "WARNING: bfFwStepGrad missing from Enzyme output (autodiff failed)"
    fi
    # (bfFwStepGrad save/restores the whole instance blob around the autodiff,
    # which also re-zeros the shadow globals each launch — so no separate
    # shadow-zeroing pass is needed.)
    INST_IN="$OUT/fw_ad.bc"
fi

echo "== instancer + link"
"$INSTANCER" "$INST_IN" "$OUT/fw_gpu_inst.bc"
"$LLVMLINK" "$OUT/fw_gpu_inst.bc" \
    "$OUT/delta_gpu.bc" "$OUT/device_flight.bc" "$OUT/physics_gpu.bc" \
    "$OUT/device_libc.bc" "$OUT/device_libm.bc" \
    --override "$LIBDEVICE" \
    -o "$OUT/whole.bc"
AD_IN="$OUT/whole.bc"

echo "== internalize + DCE + codegen"
KEEP="bfInstanceInit,bfBoot,bfRun,bfFinish,bfSnapshot,bfReset,bfStep,bfFwStep,bfSetAux,bfFwStepGrad,bfFwStepGradFD,bfFwStepJacFD,bfFwStepJacFDPure,bfFwStepJacGradPure,bfRateEval,bfLoadEeprom,bfOsdSnapshot,bfSetBase,__bf_image,__bf_image_size,__bf_image_align,__bf_state_size,__bf_act_dim,__bf_obs_dim,__bf_aux_dim,__bf_osd_rows,__bf_osd_cols,__bf_inst_base,__bf_inst_stride,__bf_inst_count,__bf_relocs,__bf_reloc_count,__bf_instanced_build,__bf_full_relocs,__bf_full_reloc_count"
"$OPT" -passes='internalize,globaldce' -internalize-public-api-list="$KEEP" \
    "$AD_IN" -o "$OUT/whole_dce.bc"
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
    [ "$n_kernels" = 0 ] && n_kernels=$(grep -c '^\.visible \.entry\|^.visible .entry' "$OUT/fw.$arch.ptx" || true)
done
fatbinary --64 --create="$OUT/fw.fatbin" $FATBIN_IMAGES
echo "   $n_kernels kernels, $(wc -c < "$OUT/fw.fatbin") B fatbin (${ARCHS// /+})"

echo "== host runner + cudaflight shared lib"
g++ -O2 tools/lockstep_instancer/gpu_runner.cpp -I/opt/cuda/include -lcuda -o "$OUT/gpu_runner"
g++ -O2 -shared -fPIC tools/lockstep_instancer/cudaflight.cpp -I/opt/cuda/include -lcuda -o "$OUT/libcudaflight.so"

echo "== done: $OUT/gpu_runner --module $OUT/fw.fatbin"
