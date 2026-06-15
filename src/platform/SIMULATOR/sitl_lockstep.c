/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// Platform glue for the SITL_LOCKSTEP target. Derived from sitl.c with
// every wall-clock, thread and UDP dependency replaced by a virtual
// clock and direct-call injection/readback (see sitl_lockstep.h).

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "common/maths.h"

#include "build/debug.h"

#include "drivers/exti.h"
#include "drivers/io.h"
#include "drivers/dma.h"
#include "drivers/motor_impl.h"
#include "drivers/serial.h"
#include "drivers/serial_tcp.h"
#include "drivers/system.h"
#include "drivers/time.h"
#include "drivers/usb_io.h"
#include "drivers/pwm_output.h"
#include "drivers/servo_impl.h"
#include "drivers/pwm_output_impl.h"
#include "drivers/light_led.h"

#include "drivers/timer.h"
#include "timer_def.h"

#include "drivers/accgyro/accgyro_virtual.h"
#include "drivers/barometer/barometer_virtual.h"
#include "flight/imu.h"

#include "cli/cli.h"

#include "config/feature.h"
#include "config/config.h"
#include "config/config_streamer.h"
#include "config/config_streamer_impl.h"
#include "config/config_eeprom_impl.h"

#include "flight/mixer.h"

#include "scheduler/scheduler.h"

#include "pg/rx.h"
#include "pg/motor.h"

#include "rx/rx.h"

#include "fc/rc_modes.h"
#include "fc/rc.h"
#include "fc/runtime_config.h"
#include "flight/pid.h"
#include "sensors/gyro.h"
#include "sensors/sensors.h"

#include "sitl_lockstep.h"

uint32_t SystemCoreClock;

// ===========================================================================
// Virtual clock
// ===========================================================================

static uint64_t vClockNs = 0;

uint64_t nanos64_real(void)
{
    return vClockNs;
}

uint64_t micros64_real(void)
{
    return vClockNs / 1000;
}

uint64_t millis64_real(void)
{
    return vClockNs / 1000000;
}

uint64_t micros64(void)
{
    return vClockNs / 1000;
}

uint64_t millis64(void)
{
    return vClockNs / 1000000;
}

uint32_t micros(void)
{
    return micros64() & 0xFFFFFFFF;
}

uint32_t millis(void)
{
    return millis64() & 0xFFFFFFFF;
}

uint64_t bflMicros(void)
{
    return micros64();
}

int32_t clockCyclesToMicros(int32_t clockCycles)
{
    return clockCycles;
}

int32_t clockCyclesTo10thMicros(int32_t clockCycles)
{
    return clockCycles;
}

int32_t clockCyclesTo100thMicros(int32_t clockCycles)
{
    return clockCycles;
}

uint32_t clockMicrosToCycles(uint32_t micros)
{
    return micros;
}

// The scheduler busy-polls getCycleCounter() to hit the gyro timing
// boundary (scheduler.c). Each poll must consume virtual time or that
// loop never terminates; tick a fixed amount per call, like a real CPU
// burning cycles. Time stays a pure function of the call sequence, so
// runs remain bit-deterministic.
#define BFL_CYCLE_TICK_NS 100

uint32_t getCycleCounter(void)
{
    vClockNs += BFL_CYCLE_TICK_NS;
    return (uint32_t)(micros64() & 0xFFFFFFFF);
}

// Delays advance the virtual clock; nothing sleeps in lockstep.
void delayMicroseconds(uint32_t us)
{
    vClockNs += (uint64_t)us * 1000;
}

void delayMicroseconds_real(uint32_t us)
{
    delayMicroseconds(us);
}

void delay(uint32_t ms)
{
    vClockNs += (uint64_t)ms * 1000000;
}

void bflStepUs(uint32_t dtUs)
{
    // Absolute step target: scheduler-internal polling also advances
    // the clock (see getCycleCounter), so track the intended timeline
    // separately to keep overshoot from accumulating into drift.
    static uint64_t simTargetNs = 0;
    if (simTargetNs == 0) {
        simTargetNs = vClockNs;
    }
    simTargetNs += (uint64_t)dtUs * 1000;
    if (simTargetNs < vClockNs) {
        simTargetNs = vClockNs;
    }
    while (vClockNs < simTargetNs) {
        vClockNs += (uint64_t)BFL_SCHED_QUANTUM_US * 1000;
        scheduler();
    }
}

// ===========================================================================
// System
// ===========================================================================

void systemInit(void)
{
    printf("[SITL_LOCKSTEP] System init, deterministic virtual clock\n");

    SystemCoreClock = 500 * 1e6; // virtual 500MHz
}

void systemReset(void)
{
    printf("[SITL_LOCKSTEP] systemReset\n");
    exit(0);
}

void systemResetToBootloader(bootloaderRequestType_e requestType)
{
    UNUSED(requestType);
    printf("[SITL_LOCKSTEP] systemResetToBootloader\n");
    exit(0);
}

void timerInit(void)
{
    // NOOP
}

void failureMode(failureMode_e mode)
{
    fprintf(stderr, "[SITL_LOCKSTEP] failureMode %d\n", mode);
    exit(1);
}

void indicateFailure(failureMode_e mode, int repeatCount)
{
    UNUSED(repeatCount);
    fprintf(stderr, "[SITL_LOCKSTEP] indicateFailure %d\n", mode);
}

int lockMainPID(void)
{
    return 0;
}

// ===========================================================================
// Sensor / RC injection (same scaling and sign conventions as
// updateState() in sitl.c, legacy bridge path)
// ===========================================================================

#define RAD2DEG (180.0 / M_PI)
#define ACC_SCALE (256 / 9.80665)
#define GYRO_SCALE (16.4)

void bflSetGyroAccel(double gx, double gy, double gz, double ax, double ay, double az)
{
    int16_t x, y, z;
    x = constrain(-ax * ACC_SCALE, -32767, 32767);
    y = constrain(-ay * ACC_SCALE, -32767, 32767);
    z = constrain(-az * ACC_SCALE, -32767, 32767);
    virtualAccSet(virtualAccDev, x, y, z);

    x = constrain(gx * GYRO_SCALE * RAD2DEG, -32767, 32767);
    y = constrain(-gy * GYRO_SCALE * RAD2DEG, -32767, 32767);
    z = constrain(-gz * GYRO_SCALE * RAD2DEG, -32767, 32767);
    virtualGyroSet(virtualGyroDev, x, y, z);
}

void bflSetBaro(int32_t pressurePa)
{
    virtualBaroSet(pressurePa, 2500);
}

void bflSetAttitudeQuat(float w, float x, float y, float z)
{
#if !defined(USE_IMU_CALC)
    imuSetAttitudeQuat(w, x, y, z);
#else
    UNUSED(w); UNUSED(x); UNUSED(y); UNUSED(z);
#endif
}

void bflSetRc(const uint16_t *channels, uint8_t channelCount)
{
    rxUpdateUdpChannels(channels, channelCount);
}

// Differentiable RC path: write float channel values [1000;2000] straight
// into rcData, bypassing the uint16 UDP-channel quantization that makes the
// normal bflSetRc() path non-differentiable w.r.t. a continuous action. No
// new-frame flag is raised, so the rx task does not overwrite rcData from the
// (stale) UDP buffer within the step — the injected setpoint persists into
// processRcCommand(). Used by the autodiff core; the forward/eval path still
// uses bflSetRc().
void bflSetRcFloat(const float *channels, uint8_t channelCount)
{
    const uint8_t count = channelCount < MAX_SUPPORTED_RC_CHANNEL_COUNT
                              ? channelCount : MAX_SUPPORTED_RC_CHANNEL_COUNT;
    for (uint8_t i = 0; i < count; i++) {
        rcData[i] = channels[i];
    }
}

// Differentiable rate-control core for the autodiff path. Runs the REAL
// Betaflight stick->motor pipeline directly on the active instance — no
// scheduler, no logging/telemetry/IO tasks — so Enzyme only ever sees the
// control math: rcData -> (expo) rcCommand -> (rates) setpoint -> pidController
// -> mixTable -> motor[]. The inputs (rcData) and outputs (motor[]) are plain
// float arrays Enzyme can shadow; the stateful config/runtime globals
// (currentPidProfile, currentControlRateProfile, pidRuntime, mixerRuntime,
// gyro) are frozen via enzyme_inactive markers on the autodiff side, yielding
// the within-step d(motor)/d(stick) sensitivity. rcChannelsUs: 4 RC channels
// in [1000,2000] us, AETR order (roll, pitch, throttle, yaw).
void bflRateCore(const float *rcChannelsUs)
{
    for (int i = 0; i < 4; i++) {
        rcData[i] = rcChannelsUs[i];
    }
    updateRcCommands();   // rcData -> rcCommand (deadband + expo)
    processRcCommand();   // rcCommand -> setpointRate (rates curve), needs isRxDataNew
    const timeUs_t now = micros();
    pidController(currentPidProfile, now);  // setpoint + gyro -> pidData
    mixTable(now);                          // pidData -> motor[]
    writeMotors();                          // motor[] -> motorsNormalised (read-side)
}

// ===========================================================================
// PWM motors / servos
// ===========================================================================

static pwmOutputPort_t servos[MAX_SUPPORTED_SERVOS];

static int16_t motorsPwm[MAX_SUPPORTED_MOTORS];
static int16_t servosPwm[MAX_SUPPORTED_SERVOS];
static int16_t idlePulse;

static float motorsPwmRaw[BFL_MAX_PWM_CHANNELS];
static float motorsNormalised[MAX_SUPPORTED_MOTORS];
static uint64_t motorUpdateCount = 0;

void servoDevInit(const servoDevConfig_t *servoConfig)
{
    UNUSED(servoConfig);
    for (uint8_t servoIndex = 0; servoIndex < MAX_SUPPORTED_SERVOS; servoIndex++) {
        servos[servoIndex].enabled = true;
    }
}

pwmOutputPort_t *pwmGetMotors(void)
{
    return pwmMotors;
}

static float pwmConvertFromExternal(uint16_t externalValue)
{
    return (float)externalValue;
}

static uint16_t pwmConvertToExternal(float motorValue)
{
    return (uint16_t)motorValue;
}

static void pwmDisableMotors(void)
{
    // NOOP
}

static void pwmWriteMotor(uint8_t index, float value)
{
    if (index < MAX_SUPPORTED_MOTORS) {
        motorsPwm[index] = value - idlePulse;
    }

    if (index < pwmMotorCount && index < BFL_MAX_PWM_CHANNELS) {
        motorsPwmRaw[index] = value;
    }
}

static void pwmWriteMotorInt(uint8_t index, uint16_t value)
{
    pwmWriteMotor(index, (float)value);
}

static void pwmShutdownPulsesForAllMotors(void)
{
    // NOOP
}

static void pwmCompleteMotorUpdate(void)
{
    // same normalisation as sitl.c: [0,1] normal, [-1,1] in 3D mode
    double outScale = 1000.0;
    if (featureIsEnabled(FEATURE_3D)) {
        outScale = 500.0;
    }

    for (int i = 0; i < MAX_SUPPORTED_MOTORS; i++) {
        motorsNormalised[i] = motorsPwm[i] / outScale;
    }

    motorUpdateCount++;
}

void servoWrite(uint8_t index, float value)
{
    servosPwm[index] = value;
    if (index + pwmMotorCount < BFL_MAX_PWM_CHANNELS) {
        motorsPwmRaw[index + pwmMotorCount] = value;
    }
}

static const motorVTable_t vTable = {
    .postInit = motorPostInitNull,
    .convertExternalToMotor = pwmConvertFromExternal,
    .convertMotorToExternal = pwmConvertToExternal,
    .enable = pwmEnableMotors,
    .disable = pwmDisableMotors,
    .isMotorEnabled = pwmIsMotorEnabled,
    .decodeTelemetry = motorDecodeTelemetryNull,
    .write = pwmWriteMotor,
    .writeInt = pwmWriteMotorInt,
    .updateComplete = pwmCompleteMotorUpdate,
    .shutdown = pwmShutdownPulsesForAllMotors,
    .requestTelemetry = NULL,
    .isMotorIdle = NULL,
    .getMotorIO = NULL,
};

bool motorPwmDevInit(motorDevice_t *device, const motorDevConfig_t *motorConfig, uint16_t _idlePulse)
{
    UNUSED(motorConfig);

    if (!device) {
        return false;
    }

    pwmMotorCount = device->count;
    device->vTable = &vTable;

    idlePulse = _idlePulse;

    for (int motorIndex = 0; motorIndex < MAX_SUPPORTED_MOTORS && motorIndex < pwmMotorCount; motorIndex++) {
        pwmMotors[motorIndex].enabled = true;
    }

    return true;
}

// Armed state accessor for the harness. The harness is native (never IR
// instanced) so it must not read firmware globals like armingFlags
// directly: in a multi-instance build that would hit the pristine
// template image instead of the active instance. This function is
// firmware-side, so its access is rewritten with the rest.
bool bflIsArmed(void)
{
    return ARMING_FLAG(ARMED);
}

// Map ARM to AUX1 high (equivalent of CLI: aux 0 0 0 1700 2100 0 0).
// Must live firmware-side: modeActivationConditionsMutable() is a
// static-inline PG accessor, and inlined into the native harness it
// would write the template image instead of the active instance.
void bflConfigureArmSwitch(void)
{
    modeActivationConditionsMutable(0)->modeId = BOXARM;
    modeActivationConditionsMutable(0)->auxChannelIndex = 0;
    modeActivationConditionsMutable(0)->range.startStep = CHANNEL_VALUE_TO_STEP(1700);
    modeActivationConditionsMutable(0)->range.endStep = CHANNEL_VALUE_TO_STEP(2100);
    analyzeModeActivationConditions();
}

// Firmware-eye view of the active instance, for harness debugging.
void bflDebugStatus(void)
{
    printf("[debug] t=%uus rc=%.0f/%.0f/%.0f/%.0f/%.0f armBox=%d gyroCal=%d disable=0x%x armed=%d\n",
           (unsigned)micros64(),
           (double)rcData[0], (double)rcData[1], (double)rcData[2], (double)rcData[3], (double)rcData[4],
           IS_RC_MODE_ACTIVE(BOXARM), gyroIsCalibrationComplete(),
           (unsigned)getArmingDisableFlags(), ARMING_FLAG(ARMED) ? 1 : 0);
}

uint16_t bflGetMotorCount(void)
{
    return pwmMotorCount;
}

void bflGetMotorsPwm(float *out, unsigned maxCount)
{
    for (unsigned i = 0; i < maxCount && i < BFL_MAX_PWM_CHANNELS; i++) {
        out[i] = motorsPwmRaw[i];
    }
}

void bflGetMotorsNormalised(float *out, unsigned maxCount)
{
    for (unsigned i = 0; i < maxCount && i < MAX_SUPPORTED_MOTORS; i++) {
        out[i] = motorsNormalised[i];
    }
}

// Raw float mixer output (motor[]), BEFORE the int16 motorsPwm quantization
// that bflGetMotorsNormalised goes through. The smooth signal the autodiff /
// finite-difference gradient must read so small perturbations aren't lost to
// integer truncation.
void bflGetMotorsRaw(float *out, unsigned maxCount)
{
    for (unsigned i = 0; i < maxCount && i < MAX_SUPPORTED_MOTORS; i++) {
        out[i] = motor[i];
    }
}

uint64_t bflGetMotorUpdateCount(void)
{
    return motorUpdateCount;
}

// Yaw reaction-torque direction for the physics model. With
// yaw_motors_reversed the firmware negates its yaw mixer column because
// the props physically spin opposite to the default direction — the
// physics model must flip its reaction torque signs to match the real
// airframe. Firmware-side so the PG accessor is not inlined into the
// native harness.
bool bflYawMotorsReversed(void)
{
    return mixerConfig()->yaw_motors_reversed;
}

// ===========================================================================
// CLI text execution. Feeds a block of CLI commands (e.g. a manufacturer
// CLI dump) through the real CLI parser against the active instance, so
// loading a config exercises the exact code path a configurator paste
// does. The caller must sanitise reboot-class commands first ('save',
// 'exit', 'defaults' without nosave, 'bl', 'msc'): on this target
// systemReset() exits the process. See bflLoadCliDump() in the harness.
// ===========================================================================

static const char *cliFeedBuf;      // text being fed (caller-owned)
static uint32_t cliFeedLen;
static uint32_t cliFeedPos;
static char cliCapBuf[256];         // CLI output captured into lines
static unsigned cliCapLen;

static void cliCapFlush(void)
{
    if (cliCapLen) {
        cliCapBuf[cliCapLen] = '\0';
        printf("[cli] %s\n", cliCapBuf);
        cliCapLen = 0;
    }
}

static void cliFeedWrite(serialPort_t *instance, uint8_t ch)
{
    UNUSED(instance);
    if (ch == '\n') {
        cliCapFlush();
    } else if (ch >= 0x20 && ch < 0x7f && cliCapLen < sizeof(cliCapBuf) - 1) {
        cliCapBuf[cliCapLen++] = ch;
    }
}

static void cliFeedWriteBuf(serialPort_t *instance, const void *data, int count)
{
    const uint8_t *p = data;
    for (int i = 0; i < count; i++) {
        cliFeedWrite(instance, p[i]);
    }
}

static uint32_t cliFeedRxWaiting(const serialPort_t *instance)
{
    UNUSED(instance);
    return cliFeedLen - cliFeedPos;
}

static uint8_t cliFeedRead(serialPort_t *instance)
{
    UNUSED(instance);
    return cliFeedPos < cliFeedLen ? (uint8_t)cliFeedBuf[cliFeedPos++] : 0;
}

static uint32_t cliFeedTxFree(const serialPort_t *instance)
{
    UNUSED(instance);
    return UINT32_MAX;
}

static bool cliFeedTxBufferEmpty(const serialPort_t *instance)
{
    UNUSED(instance);
    return true;
}

static void cliFeedSetBaudRate(serialPort_t *instance, uint32_t baudRate)
{
    instance->baudRate = baudRate;
}

static void cliFeedSetMode(serialPort_t *instance, portMode_e mode)
{
    instance->mode = mode;
}

static void cliFeedNoopWrite(serialPort_t *instance)
{
    UNUSED(instance);
}

static const struct serialPortVTable cliFeedVTable = {
    .serialWrite = cliFeedWrite,
    .serialTotalRxWaiting = cliFeedRxWaiting,
    .serialTotalTxFree = cliFeedTxFree,
    .serialRead = cliFeedRead,
    .serialSetBaudRate = cliFeedSetBaudRate,
    .isSerialTransmitBufferEmpty = cliFeedTxBufferEmpty,
    .setMode = cliFeedSetMode,
    .setCtrlLineStateCb = NULL,
    .setBaudRateCb = NULL,
    .writeBuf = cliFeedWriteBuf,
    .beginWrite = cliFeedNoopWrite,
    .endWrite = cliFeedNoopWrite,
};

static serialPort_t cliFeedPort;

static void cliFeedRun(const char *text, uint32_t len)
{
    cliFeedBuf = text;
    cliFeedLen = len;
    cliFeedPos = 0;
    // cliProcess drains every waiting byte per call; the loop guards
    // against the non-interactive 2s (virtual) inactivity exit re-arming
    // mid-feed after a command that advances the clock.
    while (cliFeedPos < cliFeedLen) {
        if (!cliProcess()) {
            cliEnter(&cliFeedPort, false);
        }
    }
}

void bflCliExec(const char *text, uint32_t len)
{
    memset(&cliFeedPort, 0, sizeof(cliFeedPort));
    cliFeedPort.vTable = &cliFeedVTable;

    cliEnter(&cliFeedPort, false);
    cliFeedRun(text, len);
    // ETX makes the non-interactive CLI exit cleanly (no reboot, and the
    // non-interactive path never set ARMING_DISABLED_CLI)
    static const char etx = 0x03;
    cliFeedRun(&etx, 1);
    cliCapFlush();
}

// ===========================================================================
// Stack
// ===========================================================================

char _estack;
char _Min_Stack_Size;

// ===========================================================================
// Virtual EEPROM. File backed by default (path overridable for parallel
// instances); BFL_EEPROM_RAM (the GPU build) drops all file I/O and works
// purely on eepromData, which is firmware state and therefore per-instance.
// A NULL path selects the same RAM-only behaviour at runtime.
// ===========================================================================

#ifdef BFL_EEPROM_RAM
static const char *eepromPath = NULL;
#else
static const char *eepromPath = EEPROM_FILENAME;
#endif
static FILE *eepromFd = NULL;

void bflSetEepromPath(const char *path)
{
    eepromPath = path;
}

uint8_t *bflEepromBuffer(void)
{
    return eepromData;
}

uint32_t bflEepromSize(void)
{
    return sizeof(eepromData);
}

bool loadEEPROMFromFile(void)
{
    if (eepromPath == NULL) {
        return true; // RAM-backed: fly with whatever eepromData holds
    }
    if (eepromFd != NULL) {
        fprintf(stderr, "[FLASH_Unlock] eepromFd != NULL\n");
        return false;
    }

    // open or create
    eepromFd = fopen(eepromPath, "r+");
    if (eepromFd != NULL) {
        // obtain file size:
        fseek(eepromFd, 0, SEEK_END);
        size_t lSize = ftell(eepromFd);
        rewind(eepromFd);

        size_t n = fread(eepromData, 1, sizeof(eepromData), eepromFd);
        if (n == lSize) {
            printf("[FLASH_Unlock] loaded '%s', size = %ld / %ld\n", eepromPath, lSize, sizeof(eepromData));
        } else {
            fprintf(stderr, "[FLASH_Unlock] failed to load '%s'\n", eepromPath);
            return false;
        }
    } else {
        printf("[FLASH_Unlock] created '%s', size = %ld\n", eepromPath, sizeof(eepromData));
        if ((eepromFd = fopen(eepromPath, "w+")) == NULL) {
            fprintf(stderr, "[FLASH_Unlock] failed to create '%s'\n", eepromPath);
            return false;
        }

        if (fwrite(eepromData, sizeof(eepromData), 1, eepromFd) != 1) {
            fprintf(stderr, "[FLASH_Unlock] write failed: %s\n", strerror(errno));
            return false;
        }
    }
    return true;
}

void configUnlock(void)
{
    loadEEPROMFromFile();
}

void configLock(void)
{
    if (eepromPath == NULL) {
        return; // RAM-backed: nothing to persist
    }
    // flush & close
    if (eepromFd != NULL) {
        fseek(eepromFd, 0, SEEK_SET);
        fwrite(eepromData, 1, sizeof(eepromData), eepromFd);
        fclose(eepromFd);
        eepromFd = NULL;
        printf("[FLASH_Lock] saved '%s'\n", eepromPath);
    } else {
        fprintf(stderr, "[FLASH_Lock] eeprom is not unlocked\n");
    }
}

configStreamerResult_e configWriteWord(uintptr_t address, config_streamer_buffer_type_t *buffer)
{
    STATIC_ASSERT(CONFIG_STREAMER_BUFFER_SIZE == sizeof(uint32_t), "CONFIG_STREAMER_BUFFER_SIZE does not match written size");

    if ((address >= (uintptr_t)eepromData) && (address + sizeof(uint32_t) <= (uintptr_t)ARRAYEND(eepromData))) {
        memcpy((void*)address, buffer, sizeof(config_streamer_buffer_type_t));
    } else {
        printf("[FLASH_ProgramWord]%p out of range!\n", (void*)address);
    }
    return CONFIG_RESULT_SUCCESS;
}

// ===========================================================================
// IO stubs
// ===========================================================================

void IOConfigGPIO(IO_t io, ioConfig_t cfg)
{
    UNUSED(io);
    UNUSED(cfg);
}

void spektrumBind(rxConfig_t *rxConfig)
{
    UNUSED(rxConfig);
}

void debugInit(void)
{
    // NOOP
}

void unusedPinsInit(void)
{
    // NOOP
}

void IOHi(IO_t io)
{
    UNUSED(io);
}

void IOLo(IO_t io)
{
    UNUSED(io);
}

void IOInitGlobal(void)
{
    // NOOP
}

IO_t IOGetByTag(ioTag_t tag)
{
    UNUSED(tag);
    return NULL;
}

bool usbCableIsInserted(void)
{
    return false;
}

void EXTIInit(void)
{
    // NOOP
}

// SERIAL_TRAIT_PIN_CONFIG is 0 on SIMULATOR so drivers/serial_pinconfig.c
// compiles empty, but init.c still references the serialPinConfig() PG
// accessor under USE_UART. Stock SITL only links because gcc-LTO strips
// the reference through the no-op below; this build doesn't use LTO, so
// provide the parameter group here.
PG_REGISTER(serialPinConfig_t, serialPinConfig, PG_SERIAL_PIN_CONFIG, 0);

void uartPinConfigure(const serialPinConfig_t *pSerialPinConfig)
{
    UNUSED(pSerialPinConfig);
}
