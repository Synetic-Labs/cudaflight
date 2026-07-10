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

// See sitl_lockstep_instance.h. This file is harness-side: it is always
// compiled natively and never goes through the IR instancer.
//
// The instancer packs all mutable firmware state into one template image
// (@__bf_image) and emits layout tables alongside it; instancing is then
// just "copy the template, rebase the listed pointer slots, point
// __bf_delta at the copy". No ELF parsing, no linker script — the same
// tables drive the GPU runtime.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sitl_lockstep_instance.h"

// Layout tables emitted by the IR instancer; weak so that the plain
// (non-instanced) make build still links.
extern char __bf_image[] __attribute__((weak));
extern const uint64_t __bf_image_size __attribute__((weak));
extern const uint64_t __bf_image_align __attribute__((weak));

typedef struct {
    uint64_t loc;       // offset of the pointer slot within the image
    uint64_t target;    // offset within the image the pointer refers to
} bfReloc_t;

extern const bfReloc_t __bf_relocs[] __attribute__((weak));
extern const uint64_t __bf_reloc_count __attribute__((weak));

// Defined (=1) only in binaries produced by the IR instancer.
extern const int __bf_instanced_build __attribute__((weak));

// The per-instance offset returned by __bf_delta_load() in instanced
// firmware code. This is the single definition; it lives outside the
// image, so every instance reads the value set by bflInstanceActivate().
uint64_t __bf_delta = 0;

static char **blobs;
static unsigned blobCount;

bool bflInstancingAvailable(void)
{
    return &__bf_instanced_build != NULL;
}

void bflInstanceTemplateFixup(void)
{
    // The instancer nulls every in-image pointer slot in the template
    // initializer (PTX cannot emit self-referential initializers), so the
    // template must be patched once before any firmware code runs. The
    // instance blobs are patched separately when they are created.
    if (!bflInstancingAvailable()) {
        return;
    }
    for (uint64_t r = 0; r < __bf_reloc_count; r++) {
        *(uint64_t *)(__bf_image + __bf_relocs[r].loc) = (uint64_t)(__bf_image + __bf_relocs[r].target);
    }
}

size_t bflInstanceImageSize(void)
{
    return bflInstancingAvailable() ? (size_t)__bf_image_size : 0;
}

int bflInstancesCreate(unsigned count)
{
    if (!bflInstancingAvailable()) {
        fprintf(stderr, "[instance] this binary is not instanced (build via tools/lockstep_instancer)\n");
        return -1;
    }

    const size_t size = __bf_image_size;
    // delta must be a multiple of the image alignment so that every
    // global keeps the alignment the layout gave it; the template is
    // align-aligned, so align-aligned blobs suffice.
    size_t align = __bf_image_align < 64 ? 64 : __bf_image_align;
    const size_t stride = (size + align - 1) & ~(align - 1);

    blobs = calloc(count, sizeof(*blobs));
    for (unsigned i = 0; i < count; i++) {
        char *blob = aligned_alloc(align, stride);
        if (!blob) {
            fprintf(stderr, "[instance] allocation failed for instance %u\n", i);
            return -1;
        }
        // pristine template copy + pointer rebasing into this copy
        memcpy(blob, __bf_image, size);
        for (uint64_t r = 0; r < __bf_reloc_count; r++) {
            *(uint64_t *)(blob + __bf_relocs[r].loc) = (uint64_t)(blob + __bf_relocs[r].target);
        }
        blobs[i] = blob;
    }
    blobCount = count;

    printf("[instance] %u instances, image %zu bytes, %llu pointer slots rebased per instance\n",
           count, size, (unsigned long long)__bf_reloc_count);
    return 0;
}

void bflInstanceActivate(unsigned idx)
{
    if (idx < blobCount) {
        __bf_delta = (uint64_t)(blobs[idx] - __bf_image);
    }
}

char *bflInstanceBlob(unsigned idx)
{
    return idx < blobCount ? blobs[idx] : NULL;
}

void bflInstancesDestroy(void)
{
    for (unsigned i = 0; i < blobCount; i++) {
        free(blobs[i]);
    }
    free(blobs);
    blobs = NULL;
    blobCount = 0;
    __bf_delta = 0;
}
