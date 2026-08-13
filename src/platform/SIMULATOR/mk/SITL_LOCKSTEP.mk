# SITL_LOCKSTEP Makefile: deterministic single-threaded SITL variant.
# Same flight sources as SITL; platform glue replaced by a virtual clock
# and a direct-call stepping harness (no UDP, no worker threads).

PLATFORM_SDK := none

# Default output is an exe file
DEFAULT_OUTPUT := exe

INCLUDE_DIRS := \
        $(INCLUDE_DIRS) \
        $(TARGET_PLATFORM_DIR) \
        $(TARGET_PLATFORM_DIR)/include \
        $(LIB_MAIN_DIR)/dyad
# dyad is needed for its header only (via drivers/serial_tcp.h); dyad.c
# itself is not built — this target has no TCP serial backend.

# sitl_lockstep_{main,physics,instance}.c are harness-side: the
# multi-instance pipeline (tools/lockstep_instancer) compiles every other
# TU to LLVM bitcode and rewrites firmware state accesses; these three
# stay native and orchestrate the instances.
MCU_COMMON_SRC  := \
        SIMULATOR/sitl_lockstep.c \
        SIMULATOR/sitl_lockstep_serial.c \
        SIMULATOR/sitl_lockstep_osd.c \
        SIMULATOR/sitl_lockstep_physics.c \
        SIMULATOR/sitl_lockstep_instance.c \
        SIMULATOR/sitl_lockstep_main.c

ARCH_FLAGS      =
DEVICE_FLAGS    =
LD_SCRIPT       = $(LINKER_DIR)/sitl.ld
STARTUP_SRC     =

MCU_FLASH_SIZE  := 2048

ARM_SDK_PREFIX  =

# main.c is excluded: the lockstep harness provides its own main() that
# drives init and the scheduler against the virtual clock.
MCU_EXCLUDES = \
        main.c \
        drivers/rx/rx_xn297.c \
        drivers/display_ug2864hsweg01.c \
        telemetry/crsf.c \
        telemetry/ghst.c \
        telemetry/srxl.c \
        io/displayport_oled.c

TARGET_MAP  = $(OBJECT_DIR)/$(FORKNAME)_$(TARGET).map

# no -lpthread/-lrt: unlike SITL there are no worker threads or timers
LIBS        = -lm -lc

LD_FLAGS    := \
            $(LIBS) \
            $(ARCH_FLAGS) \
            $(LTO_FLAGS) \
            $(DEBUG_FLAGS) \
            -Wl,-gc-sections,-Map,$(TARGET_MAP) \
            -Wl,-L$(LINKER_DIR) \
            -Wl,--cref \
            -Wl,-z,noexecstack \
            -T$(LD_SCRIPT)

# gcc-LTO is off: the multi-instance pipeline does its own whole-program
# step at the LLVM IR level, and the flag set must be clang-compatible
# (-Ofast is a hard error on clang >= 20; -ffast-math comes from
# OPTIMISATION_BASE; -Wunsafe-loop-optimizations is gcc-only).
LTO := no
CFLAGS_DISABLED += -Wunsafe-loop-optimizations

# clang warns (as error) about INFINITY under -ffast-math; the firmware's
# explog_approx.c uses it deliberately as a saturation value, same as on
# the gcc embedded targets.
ifneq ($(findstring clang,$(CROSS_CC)),)
DEVICE_FLAGS += -Wno-nan-infinity-disabled -Wno-parentheses-equality
endif

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -O3
OPTIMISE_SPEED      := -O3
OPTIMISE_SIZE       := -Os

LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_SPEED)
endif
