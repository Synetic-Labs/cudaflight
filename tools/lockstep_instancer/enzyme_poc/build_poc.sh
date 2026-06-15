#!/usr/bin/env bash
# Build + run the Enzyme-on-NVPTX proof of concept through the same toolchain
# the firmware uses: clang-20 -> nvptx64 bitcode -> Enzyme opt pass -> ptxas
# -> cubin, then a driver-API runner checks the gradient against the analytic
# derivative. Proves Enzyme works on NVPTX in this LLVM-20 slot before we
# touch firmware.
set -euo pipefail
cd "$(dirname "$0")"

LLVM=/usr/lib/llvm/20/bin
ENZ=${ENZ:-../../enzyme_build/Enzyme/LLVMEnzyme-20.so}
GPUARCH=${GPUARCH:-sm_120}

[ -f "$ENZ" ] || { echo "Enzyme plugin not found at $ENZ"; exit 1; }

echo "== clang -> nvptx bitcode (passes disabled so the autodiff call survives)"
"$LLVM/clang" -target nvptx64-nvidia-cuda -march=$GPUARCH -ffast-math \
    -O1 -Xclang -disable-llvm-passes -nostdlibinc -fno-builtin -w \
    -emit-llvm -c poc.c -o poc.bc

echo "== Enzyme pass"
"$LLVM/opt" poc.bc -load-pass-plugin="$ENZ" \
    -passes="enzyme,function(mem2reg,instcombine,simplifycfg)" \
    -o poc_ad.bc

echo "== optimize + codegen to PTX"
"$LLVM/opt" poc_ad.bc -O3 -o poc_opt.bc
"$LLVM/clang" -target nvptx64-nvidia-cuda -march=$GPUARCH -O3 -x ir poc_opt.bc -S -o poc.ptx
ptxas -arch=$GPUARCH -O3 poc.ptx -o poc.cubin
echo "   kernels in ptx: $(grep -c '\.entry' poc.ptx)"

echo "== host runner"
g++ -O2 poc_run.cpp -I/opt/cuda/include -lcuda -o poc_run
./poc_run
