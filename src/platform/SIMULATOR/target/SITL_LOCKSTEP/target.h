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

// SITL_LOCKSTEP: deterministic lockstep variant of the SITL target.
// Inherits the full SITL configuration so the compiled flight code is
// identical; only the platform glue (time, I/O threads) differs.

#pragma once

#include "../SITL/target.h"

#undef TARGET_BOARD_IDENTIFIER
#define TARGET_BOARD_IDENTIFIER "SLCK"

// The lockstep build runs single threaded with a virtual clock; no
// worker threads exist, so the multithread sync hooks must stay off.
#undef ENABLE_SIMULATOR_MULTITHREAD
#define ENABLE_SIMULATOR_MULTITHREAD 0

// Stock SITL takes attitude from the simulator (#undef USE_IMU_CALC);
// the lockstep build closes the loop through real sensors instead, so
// the firmware must run its own attitude estimation as on hardware.
#define USE_IMU_CALC

// Simulator targets drop the dynamic notch in common_post.h; sim-to-real
// needs the gyro filter chain to match hardware, so opt back in.
#define USE_DYN_NOTCH_FILTER
#define SIMULATOR_DYN_NOTCH

// Upstream slows the virtual gyro to 1 kHz because a wall-clock SITL only
// re-reads stale simulator data at 8 kHz. Lockstep time is virtual, so
// sampling costs nothing — keep the pre-2026.6 8 kHz real-sensor default
// so the PID loop rate and filter discretization match hardware.
#define VIRTUAL_GYRO_SAMPLE_RATE_HZ 8000

// OSD, rendered by the firmware into a per-instance character grid via
// the framebuffer-OSD displayport (io/displayport_fb_osd.c); the
// fb_osd_impl backend is sitl_lockstep_osd.c. SD only: with USE_OSD_HD
// the config defaults route the displayport to MSP (not built here) and
// OSD silently disables itself. AUTO device selection in init.c then
// falls through to FBOSD.
#define USE_OSD
#undef USE_OSD_HD
#define ENABLE_FB_OSD 1

// Stock SITL defaults plus OSD so the OSD task runs out of the box;
// real-quad CLI dumps agree ('feature OSD').
#undef DEFAULT_FEATURES
#define DEFAULT_FEATURES (FEATURE_GPS | FEATURE_TELEMETRY | FEATURE_OSD)
