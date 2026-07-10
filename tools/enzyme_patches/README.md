# Local Enzyme patches

`enzyme-local-fixes.patch` holds the local modifications to the
[Enzyme](https://github.com/EnzymeAD/Enzyme) autodiff plugin that the
`DIFF=1` GPU build (`tools/lockstep_instancer/build_gpu.sh`) depends on.
The checkout itself lives at `tools/Enzyme/` (gitignored — it is a full
upstream clone); this patch file is the only record of the changes, so
regenerate it after touching the checkout:

```sh
git -C tools/Enzyme diff > tools/enzyme_patches/enzyme-local-fixes.patch
```

Base: upstream commit `c965083` ("Add implicit interfaces for function hooks
in Fortran module (#2809)").

What the patches fix (details in the inline comments):

- **AdjointGenerator.h** — reconcile augmented-vs-gradient activity-analysis
  mismatches (DUP_ARG vs DUP_NONEED) instead of asserting; they share a
  signature, and activity threaded through globals can legitimately differ
  between sweeps.
- **GradientUtils.cpp** — all-zero constant aggregates get an all-zero shadow
  (valid as a global shadow initializer under loose types); constant-GEP
  shadow creation takes the source element type from the original GEP, not
  the inverted base pointer (which is generally not a GEP — would assert).
- **PreserveNVVM.cpp** — inactive (frozen) globals also get
  `enzyme_ta_norecur`, so TypeAnalysis stops recursing into union-laden state
  structs (pidRuntime, mixerRuntime, ...) whose fixpoint diverges.
- **TypeAnalysis.cpp** — `enzyme_ta_norecur` globals that are homogeneous FP
  arrays (e.g. the motor output array) seed their pointee type from the LLVM
  value type, so the motor adjoint still flows through mixTable's stores.

To rebuild the plugin from scratch:

```sh
git clone https://github.com/EnzymeAD/Enzyme.git tools/Enzyme
git -C tools/Enzyme checkout c965083
git -C tools/Enzyme apply "$(pwd)/tools/enzyme_patches/enzyme-local-fixes.patch"
cmake -S tools/Enzyme/enzyme -B tools/enzyme_build \
    -DCMAKE_BUILD_TYPE=Release -DLLVM_DIR=/usr/lib/llvm/20/lib64/cmake/llvm
cmake --build tools/enzyme_build -j"$(nproc)"
# build_gpu.sh expects tools/enzyme_build/Enzyme/LLVMEnzyme-20.so
```
