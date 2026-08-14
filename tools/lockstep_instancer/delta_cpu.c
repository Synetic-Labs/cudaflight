// SPDX-License-Identifier: GPL-3.0-or-later
// CPU definition of the per-instance state offset used by instanced
// firmware code. Linked (as bitcode) into the firmware module after the
// instancer pass so the call inlines down to a single load. The global
// itself is defined by the native instance manager
// (sitl_lockstep_instance.c), outside the instanced image, so all
// instances observe the value set by bflInstanceActivate().
//
// The GPU build links delta_gpu.c instead, which derives the offset from
// the thread index.

#include <stdint.h>

extern uint64_t __bf_delta;

__attribute__((always_inline)) uint64_t __bf_delta_load(void)
{
    return __bf_delta;
}
