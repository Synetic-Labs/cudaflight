# cudaflight

**The real Betaflight firmware, compiled to CUDA and run as thousands of
lockstep instances on a GPU.**

cudaflight is a fork of [Betaflight](https://github.com/betaflight/betaflight)
built for reinforcement learning and sim-to-real research: instead of a
hand-written approximation of the flight controller, the training loop steps
the *actual* firmware — PID loops, rates, mixer, filters, OSD and all — as a
vectorized fleet, on GPU or CPU, with JAX and PyTorch environment wrappers and
finite-difference Jacobian kernels for differentiable rollouts.

Everything upstream Betaflight does is untouched: all flight-controller
targets still build, and the fork pins a Betaflight **release tag**
(currently `2026.6.1`).

## How it works

1. **`SITL_LOCKSTEP` target** (`src/platform/SIMULATOR/sitl_lockstep*`) — a
   deterministic simulator build of the firmware: no scheduler jitter, no
   wall-clock, stepped explicitly by the harness in lockstep with a physics
   model.
2. **The instancer** (`tools/lockstep_instancer/instancer.cpp`) — an LLVM IR
   pass that packs every mutable firmware global into one relocatable image
   and rewrites all state accesses to `image + offset + delta`. One firmware
   binary becomes N independent instances that differ only in a base pointer —
   on the GPU, `delta` comes from the thread index.
3. **Backends** —
   - *CPU* (`build_multi.sh`): the instanced firmware relinked natively into
     `libcpuflight.so`, stepping instances sequentially in-process.
   - *CUDA* (`build_gpu.sh`): every firmware translation unit compiled to
     NVPTX bitcode, instanced, linked with a freestanding device libc/libm,
     and codegenned into `fw.fatbin`, loaded via the CUDA driver API by
     `libcudaflight.so`. One GPU thread = one complete flight controller.
4. **Python package** (`tools/lockstep_instancer/python`) — the `cudaflight`
   wheel bundles the fatbin and host libraries with ctypes bindings,
   `BetaflightJaxEnv` (zero-copy DLPack exchange with JAX), a PyTorch
   `BetaflightEnv`, and OSD rendering with the authentic MAX7456 font.

Gradients for differentiable rollouts come from finite-difference Jacobian
kernels (`bfFwStepJacFD*`) that evaluate the real control law under
perturbation with per-instance state save/restore, reading the pre-quantization
float motor outputs.

## Quick start

Requirements: Linux x86_64, clang/LLVM 20+ (with `llvm-config`; older clang
crashes on NVPTX varargs codegen), and for the GPU backend a CUDA 12.x+
toolkit plus an NVIDIA GPU.

```bash
# CPU fleet library (no CUDA needed)
bash tools/lockstep_instancer/build_multi.sh

# GPU fatbin + host library (ARCHS defaults to "sm_89 sm_120")
bash tools/lockstep_instancer/build_gpu.sh

# Python wheel (builds both backends, lands in python/dist/)
make -C tools/lockstep_instancer/python wheel
```

```python
from cudaflight.jax_env import BetaflightJaxEnv

env = BetaflightJaxEnv(num_envs=4096)   # 4096 real Betaflights on one GPU
obs = env.reset()
```

Prebuilt wheels are published as GitHub Release assets — see
[`tools/lockstep_instancer/python/README.md`](tools/lockstep_instancer/python/README.md)
for packaging, hosting, and install details.

An environment can boot from a real quad's CLI dump converted to an EEPROM
image, so the simulated fleet flies the exact tune of the physical aircraft;
reference configs for the BetaFPV Air75 live in
`tools/lockstep_instancer/configs/`.

## Repository layout (fork additions)

| Path | What it is |
| --- | --- |
| `src/platform/SIMULATOR/sitl_lockstep*` | Deterministic lockstep SITL target (`SITL_LOCKSTEP`) |
| `tools/lockstep_instancer/` | LLVM instancer pass, CPU/GPU build pipelines, device runtime |
| `tools/lockstep_instancer/python/` | `cudaflight` Python package (JAX/torch envs, wheel build) |
| `tools/realtime_server/` | Single-instance realtime firmware + physics server over TCP, for driving external renderers |

## Relationship to Betaflight

This fork is periodically rebased on upstream
[betaflight/betaflight](https://github.com/betaflight/betaflight); firmware
internals are deliberately unmodified so upstream merges stay trivial (the
only firmware-tree change is an opt-in `SIMULATOR_DYN_NOTCH` switch in
`src/main/target/common_post.h`). For flying real aircraft, use upstream
Betaflight and its documentation at [betaflight.com](https://www.betaflight.com/).

**Version policy.** Each cudaflight wheel pins one exact Betaflight release
tag (currently `2026.6.1`), and with it every internal schema version
(parameter groups, `EEPROM_CONF_VERSION`). Upstream syncs move tag to tag,
never to `master`, so the sim firmware can match the release flashed on
physical aircraft.

## Configs

Drone configs are CLI text (`dump all` from the configurator), owned by the
consuming trainer's repo. The binary eeprom image is a derived artifact: render
it at use time with `cudaflight.render_eeprom()` (or the harness binary's
`--cli-dump` mode) and never commit it — a committed image one parameter-group
version behind the firmware makes Betaflight factory-reset the whole config at
boot, with no error. The renderer is strict instead: any rejected line fails
the render and is named. cudaflight itself carries no drone configs, only the
stock dump of its own build; see `tools/lockstep_instancer/configs/example/`.

## Roadmap

- **Memory: make the kernel stack reservation a create parameter.** The library
  sets a generous per-thread kernel stack limit so the deepest Betaflight call
  chains cannot overflow. CUDA reserves local memory for every thread the
  DEVICE can hold (stack bytes x max resident threads), not per fleet instance
  — about 8 GB on a 24 GB RTX 4090, independent of fleet size. This is the
  largest GPU-memory consumer outside the training framework's allocator.
  Exposing the per-thread stack size as a `cudaflight_create*` parameter would
  let calibrated deployments reclaim multiple GB for the training framework
  (larger fleets, larger networks, vision buffers). Caution for the
  implementation: the firmware makes indirect calls, so no tool can compute the
  worst-case stack depth statically. A limit below the true worst case
  overflows the kernel stack — CUDA then reports an illegal memory access, or
  corrupts data with no error. Calibration must be empirical, per firmware
  build, with a safety margin, and the default must stay at the current
  generous value.

## License

Like Betaflight itself, cudaflight is licensed under the
[GNU GPL v3](LICENSE) (or later). It exists thanks to the work of the
Betaflight, Cleanflight, Baseflight and MultiWii communities.
