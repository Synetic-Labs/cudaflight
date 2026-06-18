# bfgym-firmware

Python wheel packaging the real Betaflight firmware compiled to NVPTX
(`fw.fatbin`) and the CUDA driver-API host library (`libbfgym.so`) into a
single, pip-installable artifact.

The wheel is **architecture-locked** to Linux x86_64 with CUDA 12.x. The
fatbin contains cubins for the SMs listed in `ARCHS` at build time
(default: `sm_89 sm_120` — RTX 4090, RTX 5090, RTX PRO 6000).

## Building locally

From the repository root:

```bash
make -C tools/lockstep_instancer/python wheel
```

The output wheel lands in `tools/lockstep_instancer/python/dist/`.

Targeting a different SM set:

```bash
ARCHS="sm_80 sm_89 sm_90 sm_120" \
    make -C tools/lockstep_instancer/python wheel
```

## Installing

```bash
pip install dist/bfgym_firmware-0.1.0-cp310-cp310-linux_x86_64.whl
# or with extras:
pip install "bfgym-firmware[jax] @ file://.../dist/bfgym_firmware-...whl"
```

## Using

```python
from bfgym_firmware import default_fatbin_path, default_lib_path, load
from bfgym_firmware.bfgym_jax import BetaflightJaxEnv

env = BetaflightJaxEnv(num_envs=256)  # picks up the packaged fatbin + .so
```

Override the bundled artifacts (for in-place rebuilds without reinstalling):

```bash
export BFGYM_LIB=/path/to/built/libbfgym.so
export BFGYM_FATBIN=/path/to/built/fw.fatbin
```
