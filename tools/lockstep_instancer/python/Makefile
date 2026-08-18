# Build the cudaflight wheel.
#
#   make wheel         build both firmwares + the XLA shim, then package
#   make artifacts     build everything and copy the data files (skip the wheel)
#   make firmware      just rebuild fw.fatbin / libcudaflight.so
#   make firmware-cpu  just rebuild libcpuflight.so
#   make shim          just rebuild libcudaflight_xla.so (XLA FFI handlers)
#   make release       build the wheel and upload it as a GitHub Release asset
#   make clean         wipe build/, dist/, and the bundled data files
#
# The wheel ships fw.fatbin, libcudaflight.so, libcpuflight.so and
# libcudaflight_xla.so (the in-jit XLA FFI handlers, cudaflight.xla) under
# cudaflight/_data/ alongside the MAX7456 font from the firmware tree, so
# importlib.resources resolves all runtime artifacts without reaching
# outside the wheel. The shim SOURCE (cudaflight_xla.cpp) ships too, as the
# fallback for boxes where the prebuilt handlers cannot load.
#
# The shim compiles against the build venv's jaxlib FFI headers (the XLA FFI
# C ABI is versioned and stable) and links only libcuda from the NVIDIA
# driver — no CUDA toolkit headers. `make shim` therefore needs `jax` in
# $(PYTHON)'s environment.

ROOT       := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))../../..)
GPU_OUT    := $(ROOT)/obj/gpu
DATA       := $(CURDIR)/src/cudaflight/_data
FONT_SRC   := $(ROOT)/src/platform/PICO/osd/font_betaflight.c
BUILD_SH   := tools/lockstep_instancer/build_gpu.sh

# Multi-arch fatbin target list; consumed by build_gpu.sh
ARCHS ?= sm_89 sm_120
export ARCHS

PYTHON     ?= python3

# Hosting: the wheel ships as a GitHub Release asset (not committed to git —
# it is a 14MB arch-locked binary regenerated on every firmware build). Keep
# VERSION in sync with pyproject.toml; the tag is namespaced (cudaflight-v*) so it
# does not collide with the firmware-version release tags.
VERSION    ?= 0.3.4
REPO       ?= synaptech-solutions/cudaflight
TAG        := cudaflight-v$(VERSION)

# Platform tag the shipped wheel must satisfy. auditwheel verifies every
# bundled .so against the manylinux_2_39 policy (glibc <= 2.39, i.e. runs on
# Ubuntu 24.04+) and FAILS the build if the host toolchain linked anything
# newer — a wheel built on a too-new glibc can no longer ship silently.
# libcuda.so.1 is excluded: it comes from the target's NVIDIA driver.
# Requires auditwheel + patchelf (pip install build auditwheel patchelf).
PLAT       ?= manylinux_2_39_x86_64
WHEEL      := dist/cudaflight-$(VERSION)-py3-none-$(PLAT).whl

SHIM_SRC   := $(CURDIR)/xla_shim/cudaflight_xla.cpp
# libcuda link dirs: driver installs + toolkit stubs + WSL2 (only -L hints;
# missing dirs are harmless). auditwheel excludes libcuda.so.1 from the wheel.
SHIM_LDIRS := -L/usr/lib/x86_64-linux-gnu -L/usr/local/cuda/lib64/stubs -L/usr/lib/wsl/lib

.PHONY: all wheel firmware firmware-cpu shim artifacts clean release

all: wheel

firmware:
	cd $(ROOT) && bash $(BUILD_SH)

# CPU SITL fleet library (libcpuflight.so) — the no-CUDA backend for small
# fleets (cudaflight refuses n < 3; see discoverRelocs).
firmware-cpu:
	cd $(ROOT) && bash tools/lockstep_instancer/build_multi.sh

# XLA FFI handlers (cudaflight.xla) — in-jit firmware step/reset custom calls.
# Compiled against $(PYTHON)'s jaxlib headers; ships prebuilt + as source.
shim:
	mkdir -p $(DATA)
	g++ -std=c++17 -O2 -shared -fPIC \
		-I$$($(PYTHON) -c "import jax.ffi; print(jax.ffi.include_dir())") \
		$(SHIM_SRC) $(SHIM_LDIRS) -lcuda -o $(DATA)/libcudaflight_xla.so
	cp $(SHIM_SRC) $(DATA)/cudaflight_xla.cpp

artifacts: firmware firmware-cpu shim
	mkdir -p $(DATA)
	cp $(GPU_OUT)/libcudaflight.so $(DATA)/libcudaflight.so
	cp $(GPU_OUT)/fw.fatbin   $(DATA)/fw.fatbin
	cp $(ROOT)/obj/multi/libcpuflight.so $(DATA)/libcpuflight.so
	cp $(FONT_SRC)            $(DATA)/font_betaflight.c

wheel: artifacts
	rm -rf build dist *.egg-info src/*.egg-info
	$(PYTHON) -m build --wheel
	$(PYTHON) -m auditwheel repair --plat $(PLAT) --only-plat --exclude libcuda.so.1 \
		--wheel-dir dist dist/cudaflight-$(VERSION)-py3-none-linux_x86_64.whl
	rm dist/cudaflight-$(VERSION)-py3-none-linux_x86_64.whl

# Publish the built wheel as a GitHub Release asset. Creates the tag's
# release on first run, re-uploads (--clobber) on subsequent runs so a
# rebuilt wheel for the same version replaces the old asset. Install URL:
#   https://github.com/$(REPO)/releases/download/$(TAG)/$(notdir $(WHEEL))
release: wheel
	@command -v gh >/dev/null || { echo "gh CLI not found"; exit 1; }
	@if gh release view $(TAG) --repo $(REPO) >/dev/null 2>&1; then \
		echo "Uploading $(WHEEL) to existing release $(TAG)"; \
		gh release upload $(TAG) $(WHEEL) --clobber --repo $(REPO); \
	else \
		echo "Creating release $(TAG)"; \
		gh release create $(TAG) $(WHEEL) --repo $(REPO) \
			--title "cudaflight $(VERSION)" \
			--notes "Prebuilt cudaflight wheel ($(PLAT): glibc 2.39+, Ubuntu 24.04+; GPU backend needs an R580+ NVIDIA driver). pip install the asset URL directly."; \
	fi
	@echo "Installable from: https://github.com/$(REPO)/releases/download/$(TAG)/$(notdir $(WHEEL))"

clean:
	rm -rf build dist *.egg-info src/*.egg-info
	rm -f $(DATA)/libcudaflight.so $(DATA)/libcpuflight.so $(DATA)/fw.fatbin $(DATA)/font_betaflight.c
	rm -f $(DATA)/libcudaflight_xla.so $(DATA)/cudaflight_xla.cpp
