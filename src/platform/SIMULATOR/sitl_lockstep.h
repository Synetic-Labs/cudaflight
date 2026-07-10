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

// Deterministic lockstep API for the SITL_LOCKSTEP target.
//
// The firmware runs against a virtual clock that only advances inside
// bflStepUs(); there are no worker threads and no UDP/TCP I/O on the
// flight path. Sensor and RC data are injected by direct call, motor
// outputs are read back by direct call. Identical call sequences
// produce bit-identical firmware state.

#pragma once

#include <stdbool.h>
#include <stdint.h>

#define BFL_MAX_RC_CHANNELS  16
#define BFL_MAX_PWM_CHANNELS 16

// Virtual clock, microseconds since boot.
uint64_t bflMicros(void);

// Advance the virtual clock by dtUs, invoking the scheduler once per
// BFL_SCHED_QUANTUM_US of simulated time (mirrors the free-running
// scheduler() poll loop of the stock firmware).
#define BFL_SCHED_QUANTUM_US 10
void bflStepUs(uint32_t dtUs);

// Sensor injection. Units and sign conventions are identical to the
// fdm_packet handling in sitl.c (legacy bridge path):
//   gyro: rad/s body rates (RPY), acc: m/s^2 NED body frame (1g level
//   flight is az = -9.80665), quat: body-to-world w,x,y,z.
void bflSetGyroAccel(double gx, double gy, double gz, double ax, double ay, double az);
void bflSetBaro(int32_t pressurePa);
void bflSetAttitudeQuat(float w, float x, float y, float z);

// RC injection (1000..2000 us channel values, AETR order by default map).
void bflSetRc(const uint16_t *channels, uint8_t channelCount);

// Differentiable RC injection: float channel values written straight into
// rcData, bypassing the uint16 quantization (for the autodiff core).
void bflSetRcFloat(const float *channels, uint8_t channelCount);

// Differentiable rate-control core: real stick->motor pipeline (no scheduler),
// rcChannelsUs = 4 channels [1000,2000] us AETR. For the Enzyme autodiff path.
void bflRateCore(const float *rcChannelsUs);

// Raw float mixer output (motor[]), pre int16 quantization — the smooth signal
// the finite-difference gradient reads.
void bflGetMotorsRaw(float *out, unsigned maxCount);

// Armed state of the active instance (safe for the native harness,
// unlike reading armingFlags directly).
bool bflIsArmed(void);

// Map ARM to AUX1 high in the active instance's config.
void bflConfigureArmSwitch(void);

// Map ANGLE (self-levelling) to AUX2 high; AUX2 low = acro (rate) mode.
void bflConfigureModeSwitch(void);

// Print the active instance's RC/arming/calibration state (debug aid).
void bflDebugStatus(void);

// Motor readback.
uint16_t bflGetMotorCount(void);
// Raw PWM values as written by the mixer (typically 1000..2000).
void bflGetMotorsPwm(float *out, unsigned maxCount);
// Normalised [0,1] ([-1,1] in 3D mode) outputs, as sent to simulators.
void bflGetMotorsNormalised(float *out, unsigned maxCount);
// Number of completed motor updates (one per PID loop) since boot.
uint64_t bflGetMotorUpdateCount(void);

// Override the EEPROM backing file path (default: EEPROM_FILENAME).
// NULL selects RAM-only mode (no file I/O; eepromData is firmware state
// and therefore per-instance). Must be called before init.
void bflSetEepromPath(const char *path);

// Direct access to the EEPROM backing store of the active instance, for
// host-side preloading in RAM-only mode.
uint8_t *bflEepromBuffer(void);
uint32_t bflEepromSize(void);

// Execute CLI commands against the active instance through the real CLI
// parser (the code path a configurator paste takes); output is captured
// and printed with a [cli] prefix. The text must not contain reboot-class
// commands ('save' without noreboot, 'exit', plain 'defaults', 'bl',
// 'msc') — systemReset() exits the process on this target. Use the
// harness-side dump loader for raw configurator/manufacturer dumps.
void bflCliExec(const char *text, uint32_t len);

// mixerConfig()->yaw_motors_reversed of the active instance, for the
// physics model's reaction torque signs.
bool bflYawMotorsReversed(void);

// OSD character grid of the active instance (sitl_lockstep_osd.c):
// rows*cols MAX7456-style font indices, row-major, 0x20 = blank, plus a
// parallel displayport-attribute grid (severity bits, DISPLAYPORT_BLINK
// in bit 7). Live firmware state — copy out before stepping again if a
// stable frame is needed.
const uint8_t *bflOsdScreen(void);
const uint8_t *bflOsdAttrs(void);
unsigned bflOsdRows(void);
unsigned bflOsdCols(void);
// Completed OSD draw cycles since boot (0 = OSD never ran).
uint32_t bflOsdDrawCount(void);
// Set the craft name (OSD_CRAFT_NAME) unless the config already has one.
void bflOsdDefaultCraftName(const char *name);
// Enable a classic FPV element layout if the config enables no elements.
void bflOsdApplyDemoLayoutIfBlank(void);
