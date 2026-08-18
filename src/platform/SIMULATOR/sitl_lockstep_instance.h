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
// builds (see tools/lockstep_instancer/). The instancer packs all mutable
// firmware state into one template image (@__bf_image) and emits layout
// tables; each instance is a heap copy of that image with the recorded
// pointer slots rebased. Activating an instance sets __bf_delta, which
// instanced firmware code adds to every state access.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// True when this binary was built through the IR instancer (i.e. firmware
// state accesses honour __bf_delta and N > 1 is meaningful).
bool bflInstancingAvailable(void);

// Patch the template's in-image pointer slots (nulled in the static
// initializer for GPU-backend compatibility). Must run before any
// firmware init. No-op in non-instanced builds.
void bflInstanceTemplateFixup(void);

// Allocate and initialise `count` pristine instance images. Call once,
// before any firmware init. Returns 0 on success.
int bflInstancesCreate(unsigned count);

// Route all subsequent firmware state accesses to instance `idx`.
void bflInstanceActivate(unsigned idx);

size_t bflInstanceImageSize(void);

// Instance `idx`'s image copy (NULL if out of range). CPU blobs never move,
// so a byte snapshot restored to the same blob needs no pointer rebasing —
// the basis of cpuflight's episode reset.
char *bflInstanceBlob(unsigned idx);

// Free every instance image and deactivate instancing (delta back to the
// template). The template itself is never booted, so a subsequent
// bflInstancesCreate() starts from pristine state again.
void bflInstancesDestroy(void);
