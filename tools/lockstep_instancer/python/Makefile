# Build the bfgym-firmware wheel.
#
#   make wheel        build the firmware then package
#   make artifacts    just refresh the data files (skip wheel build)
#   make firmware     just rebuild fw.fatbin / libbfgym.so
#   make clean        wipe build/, dist/, and the bundled data files
#
# The wheel ships fw.fatbin and libbfgym.so under bfgym_firmware/_data/
# alongside the MAX7456 font from the firmware tree, so importlib.resources
# resolves all runtime artifacts without reaching outside the wheel.

ROOT       := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))../../..)
GPU_OUT    := $(ROOT)/obj/gpu
DATA       := $(CURDIR)/src/bfgym_firmware/_data
FONT_SRC   := $(ROOT)/src/platform/PICO/osd/font_betaflight.c
BUILD_SH   := tools/lockstep_instancer/build_gpu.sh

# Multi-arch fatbin target list; consumed by build_gpu.sh
ARCHS ?= sm_89 sm_120
export ARCHS

PYTHON     ?= python3

.PHONY: all wheel firmware artifacts clean

all: wheel

firmware:
	cd $(ROOT) && bash $(BUILD_SH)

artifacts: firmware
	mkdir -p $(DATA)
	cp $(GPU_OUT)/libbfgym.so $(DATA)/libbfgym.so
	cp $(GPU_OUT)/fw.fatbin   $(DATA)/fw.fatbin
	cp $(FONT_SRC)            $(DATA)/font_betaflight.c

wheel: artifacts
	rm -rf build dist *.egg-info src/*.egg-info
	$(PYTHON) -m build --wheel

clean:
	rm -rf build dist *.egg-info src/*.egg-info
	rm -f $(DATA)/libbfgym.so $(DATA)/fw.fatbin $(DATA)/font_betaflight.c
