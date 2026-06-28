# Build the cudaflight wheel.
#
#   make wheel        build the firmware then package
#   make artifacts    just refresh the data files (skip wheel build)
#   make firmware     just rebuild fw.fatbin / libcudaflight.so
#   make clean        wipe build/, dist/, and the bundled data files
#
# The wheel ships fw.fatbin and libcudaflight.so under cudaflight/_data/
# alongside the MAX7456 font from the firmware tree, so importlib.resources
# resolves all runtime artifacts without reaching outside the wheel.

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
VERSION    ?= 0.1.0
REPO       ?= synaptech-solutions/cudaflight
TAG        := cudaflight-v$(VERSION)
WHEEL      := dist/cudaflight-$(VERSION)-py3-none-linux_x86_64.whl

.PHONY: all wheel firmware artifacts clean release

all: wheel

firmware:
	cd $(ROOT) && bash $(BUILD_SH)

artifacts: firmware
	mkdir -p $(DATA)
	cp $(GPU_OUT)/libcudaflight.so $(DATA)/libcudaflight.so
	cp $(GPU_OUT)/fw.fatbin   $(DATA)/fw.fatbin
	cp $(FONT_SRC)            $(DATA)/font_betaflight.c

wheel: artifacts
	rm -rf build dist *.egg-info src/*.egg-info
	$(PYTHON) -m build --wheel

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
			--notes "Prebuilt cudaflight wheel (Linux x86_64, CUDA 12.x). pip install the asset URL directly."; \
	fi
	@echo "Installable from: https://github.com/$(REPO)/releases/download/$(TAG)/$(notdir $(WHEEL))"

clean:
	rm -rf build dist *.egg-info src/*.egg-info
	rm -f $(DATA)/libcudaflight.so $(DATA)/fw.fatbin $(DATA)/font_betaflight.c
