TARGET_MCU        := SIMULATOR
TARGET_MCU_FAMILY := SITL_LOCKSTEP
SIMULATOR_BUILD    = yes

# Keep feature gating identical to the SITL target (common_pre.h checks SITL)
TARGET_FLAGS      := $(TARGET_FLAGS) -DSITL

# serial_tcp.c is replaced by the sink backend in
# SIMULATOR/sitl_lockstep_serial.c (no sockets in lockstep builds)
TARGET_SRC = \
            drivers/accgyro/accgyro_virtual.c \
            drivers/barometer/barometer_virtual.c \
            drivers/compass/compass_virtual.c \
            io/gps_virtual.c \
            blackbox/blackbox_virtual.c
