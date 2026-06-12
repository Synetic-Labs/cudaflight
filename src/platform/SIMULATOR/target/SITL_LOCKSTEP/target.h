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
