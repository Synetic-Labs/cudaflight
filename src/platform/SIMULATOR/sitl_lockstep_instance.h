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

// Per-instance firmware state manager for SITL_LOCKSTEP multi-instance
// builds (see tools/lockstep_instancer/). Each instance is a heap copy
// of the firmware's mutable image [__bf_inst_start, __bf_inst_end);
// in-range pointers inside the copy are rebased using the relocation
// records the linker kept in the executable (--emit-relocs). Activating
// an instance sets __bf_delta, which instanced firmware code adds to
// every state access.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// True when this binary was built through the IR instancer (i.e. firmware
// state accesses honour __bf_delta and N > 1 is meaningful).
bool bflInstancingAvailable(void);

// Allocate and initialise `count` pristine instance images. Call once,
// before any firmware init. Returns 0 on success.
int bflInstancesCreate(unsigned count);

// Route all subsequent firmware state accesses to instance `idx`.
void bflInstanceActivate(unsigned idx);

size_t bflInstanceImageSize(void);
