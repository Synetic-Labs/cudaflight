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

MCU_COMMON_SRC  := \
        $(LIB_MAIN_DIR)/dyad/dyad.c \
        SIMULATOR/sitl_lockstep.c \
        SIMULATOR/sitl_lockstep_physics.c \
        SIMULATOR/sitl_lockstep_main.c

#Flags
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

LIBS        = -lm -lpthread -lc -lrt

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

ifneq ($(DEBUG),GDB)
OPTIMISE_DEFAULT    := -Ofast
OPTIMISE_SPEED      := -Ofast
OPTIMISE_SIZE       := -Os

LTO_FLAGS           := $(OPTIMISATION_BASE) $(OPTIMISE_SPEED)
endif
