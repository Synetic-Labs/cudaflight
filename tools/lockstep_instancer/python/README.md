# cudaflight

Python wheel packaging the real Betaflight firmware compiled to NVPTX
(`fw.fatbin`) and the CUDA driver-API host library (`libcudaflight.so`) into a
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

From a local build:

```bash
pip install dist/cudaflight-0.1.0-py3-none-linux_x86_64.whl
# or with extras:
pip install "cudaflight[jax] @ file:///abs/path/to/dist/cudaflight-0.1.0-py3-none-linux_x86_64.whl"
```

The wheel is tagged `py3-none-linux_x86_64`: ABI-agnostic (no CPython
extension), platform-locked (ships `libcudaflight.so` + `fw.fatbin`).

## Hosting / installing from GitHub

The wheel is a ~14MB arch-locked binary regenerated on every firmware
build, so it is **not committed to git** — it ships as a GitHub Release
asset. Build and publish in one step:

```bash
make -C tools/lockstep_instancer/python release   # builds wheel + uploads
```

This creates (or re-uploads to) the `cudaflight-v<version>` release on
`synaptech-solutions/cudaflight`. Install directly from the asset URL:

```bash
pip install https://github.com/synaptech-solutions/cudaflight/releases/download/cudaflight-v0.1.0/cudaflight-0.1.0-py3-none-linux_x86_64.whl
```

Or pin it as a dependency (works with pip and uv):

```toml
# pyproject.toml of the consuming project
dependencies = [
  "cudaflight @ https://github.com/synaptech-solutions/cudaflight/releases/download/cudaflight-v0.1.0/cudaflight-0.1.0-py3-none-linux_x86_64.whl",
]
```

> `pip install git+https://…` will **not** work: it triggers a source
> build, but the firmware binaries (`fw.fatbin`, `libcudaflight.so`) are
> generated artifacts absent from the git tree. Always install the
> prebuilt release wheel.

## Using

```python
from cudaflight import default_fatbin_path, default_lib_path, load
from cudaflight.cudaflight_jax import BetaflightJaxEnv

env = BetaflightJaxEnv(num_envs=256)  # picks up the packaged fatbin + .so
```

Override the bundled artifacts (for in-place rebuilds without reinstalling):

```bash
export CUDAFLIGHT_LIB=/path/to/built/libcudaflight.so
export CUDAFLIGHT_FATBIN=/path/to/built/fw.fatbin
```
