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
// compiled natively and never goes through the IR instancer, so its own
// accesses (including __bf_delta itself) hit the template image.

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sitl_lockstep_instance.h"

// Bounds of the firmware's mutable image, placed by sitl_lockstep.ld.
extern char __bf_inst_start[];
extern char __bf_inst_end[];

// The per-instance offset added by instanced firmware code to every state
// access. This is the single definition; the IR pass only declares it.
uint64_t __bf_delta = 0;

// Defined (=1) only in binaries produced by the IR instancer.
extern const int __bf_instanced_build __attribute__((weak));

#define MAX_RELOCS (1u << 16)

typedef struct {
    uint64_t loc;       // offset of the pointer slot within the image
    uint64_t target;    // offset within the image the pointer refers to
} bfReloc_t;

static bfReloc_t *relocs;
static unsigned relocCount;
static char **blobs;
static unsigned blobCount;

bool bflInstancingAvailable(void)
{
    return &__bf_instanced_build != NULL;
}

size_t bflInstanceImageSize(void)
{
    return (size_t)(__bf_inst_end - __bf_inst_start);
}

// Walk our own executable's relocation records (kept by --emit-relocs)
// and collect every absolute pointer slot that both lives in and points
// into the instanced range. These are the slots that must be rebased in
// each instance copy, exactly what a program loader would relocate.
static int loadRelocs(void)
{
    FILE *f = fopen("/proc/self/exe", "rb");
    if (!f) {
        perror("[instance] /proc/self/exe");
        return -1;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return -1;
    }
    long fsize = ftell(f);
    rewind(f);
    char *image = malloc(fsize);
    if (!image || fread(image, 1, fsize, f) != (size_t)fsize) {
        fprintf(stderr, "[instance] failed to read executable image\n");
        free(image);
        fclose(f);
        return -1;
    }
    fclose(f);

    const Elf64_Ehdr *eh = (const Elf64_Ehdr *)image;
    const Elf64_Shdr *sh = (const Elf64_Shdr *)(image + eh->e_shoff);
    const char *shstr = image + sh[eh->e_shstrndx].sh_offset;

    // link-time values of the range bounds, from .symtab
    const Elf64_Sym *symtab = NULL;
    unsigned symCount = 0;
    const char *strtab = NULL;
    for (unsigned i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type == SHT_SYMTAB) {
            symtab = (const Elf64_Sym *)(image + sh[i].sh_offset);
            symCount = sh[i].sh_size / sizeof(Elf64_Sym);
            strtab = image + sh[sh[i].sh_link].sh_offset;
            break;
        }
    }
    if (!symtab) {
        fprintf(stderr, "[instance] no symtab in executable\n");
        free(image);
        return -1;
    }
    uint64_t startLt = 0, endLt = 0;
    for (unsigned i = 0; i < symCount; i++) {
        const char *name = strtab + symtab[i].st_name;
        if (strcmp(name, "__bf_inst_start") == 0) {
            startLt = symtab[i].st_value;
        } else if (strcmp(name, "__bf_inst_end") == 0) {
            endLt = symtab[i].st_value;
        }
    }
    if (!startLt || !endLt || endLt <= startLt) {
        fprintf(stderr, "[instance] missing __bf_inst_start/end symbols\n");
        free(image);
        return -1;
    }
    if ((size_t)(endLt - startLt) != bflInstanceImageSize()) {
        fprintf(stderr, "[instance] link-time/runtime image size mismatch\n");
        free(image);
        return -1;
    }

    relocs = malloc(MAX_RELOCS * sizeof(*relocs));
    if (!relocs) {
        free(image);
        return -1;
    }

    for (unsigned i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type != SHT_RELA) {
            continue;
        }
        const char *secName = shstr + sh[i].sh_name;
        // .rela.dyn/.rela.plt belong to the dynamic loader (and would
        // double-count the static records); only the --emit-relocs copies
        // carry the symbol+addend form we need.
        if (strcmp(secName, ".rela.dyn") == 0 || strcmp(secName, ".rela.plt") == 0) {
            continue;
        }
        const Elf64_Rela *ra = (const Elf64_Rela *)(image + sh[i].sh_offset);
        unsigned n = sh[i].sh_size / sizeof(Elf64_Rela);
        for (unsigned r = 0; r < n; r++) {
            uint64_t loc = ra[r].r_offset;
            if (loc < startLt || loc >= endLt) {
                continue;
            }
            uint32_t type = ELF64_R_TYPE(ra[r].r_info);
            uint32_t symIdx = ELF64_R_SYM(ra[r].r_info);
            if (type != R_X86_64_64) {
                // non-pointer-sized absolute data relocation inside the
                // state image would be silently wrong: refuse loudly
                fprintf(stderr, "[instance] unsupported reloc type %u at %s+0x%llx\n",
                        type, secName, (unsigned long long)(loc - startLt));
                free(image);
                return -1;
            }
            uint64_t target = symtab[symIdx].st_value + (uint64_t)ra[r].r_addend;
            if (target < startLt || target >= endLt) {
                continue; // points at shared text/rodata: copy stays valid
            }
            if (relocCount >= MAX_RELOCS) {
                fprintf(stderr, "[instance] reloc table overflow\n");
                free(image);
                return -1;
            }
            relocs[relocCount].loc = loc - startLt;
            relocs[relocCount].target = target - startLt;
            relocCount++;
        }
    }

    free(image);
    return 0;
}

int bflInstancesCreate(unsigned count)
{
    if (!bflInstancingAvailable()) {
        fprintf(stderr, "[instance] this binary is not instanced (build via tools/lockstep_instancer)\n");
        return -1;
    }
    if (loadRelocs() != 0) {
        return -1;
    }

    const size_t size = bflInstanceImageSize();
    // Each blob must be congruent with the template modulo the page size:
    // compiled code may rely on any alignment the linker gave a global up
    // to 4096, and __bf_delta shifts every state address by the same
    // amount, so only page-congruent placement preserves all alignments.
    const size_t pageOff = (uintptr_t)__bf_inst_start & 4095;
    blobs = calloc(count, sizeof(*blobs));
    for (unsigned i = 0; i < count; i++) {
        char *raw = aligned_alloc(4096, (size + pageOff + 4095) & ~(size_t)4095);
        if (!raw) {
            fprintf(stderr, "[instance] allocation failed for instance %u\n", i);
            return -1;
        }
        blobs[i] = raw + pageOff;
        // pristine template copy + pointer rebasing into this copy
        memcpy(blobs[i], __bf_inst_start, size);
        for (unsigned r = 0; r < relocCount; r++) {
            *(uint64_t *)(blobs[i] + relocs[r].loc) = (uint64_t)(blobs[i] + relocs[r].target);
        }
    }
    blobCount = count;

    printf("[instance] %u instances, image %zu bytes, %u pointer slots rebased per instance\n",
           count, size, relocCount);
    return 0;
}

void bflInstanceActivate(unsigned idx)
{
    if (idx < blobCount) {
        __bf_delta = (uint64_t)(blobs[idx] - __bf_inst_start);
    }
}
