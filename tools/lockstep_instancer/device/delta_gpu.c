// GPU definition of the per-instance state offset used by instanced
// firmware code. One GPU thread == one firmware instance, everywhere and
// for the lifetime of the module: every kernel derives the instance
// index the same way, so __bf_delta_load() is a pure function of the
// thread id. Inlines to a few ALU ops after linking.

#include <stdint.h>

extern char __bf_image[];

// Set by the host (cuModuleGetGlobal + cuMemcpyHtoD) before any launch.
char *__bf_inst_base;
uint64_t __bf_inst_stride;
uint32_t __bf_inst_count;

static inline unsigned bfThreadInstance(void)
{
    return (unsigned)__nvvm_read_ptx_sreg_ctaid_x() * (unsigned)__nvvm_read_ptx_sreg_ntid_x()
         + (unsigned)__nvvm_read_ptx_sreg_tid_x();
}

__attribute__((always_inline)) uint64_t __bf_delta_load(void)
{
    return (uint64_t)(__bf_inst_base + (uint64_t)bfThreadInstance() * __bf_inst_stride)
         - (uint64_t)__bf_image;
}
