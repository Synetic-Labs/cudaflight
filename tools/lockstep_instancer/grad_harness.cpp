// Minimal standalone launcher for bfFwStepGrad, to isolate the Enzyme
// reverse-mode kernel from JAX/XLA and run it under compute-sanitizer.
//   g++ -O2 grad_harness.cpp -I/opt/cuda/include -L<obj/gpu> -lcudaflight -lcuda -o grad_harness
//   compute-sanitizer --tool memcheck ./grad_harness <cubin> [N]
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cuda.h>

extern "C" {
struct cudaflight;
cudaflight *cudaflight_create_eeprom(const char *cubin, uint32_t n, int dev, uint32_t settle, const char *);
void cudaflight_destroy(cudaflight *);
const char *cudaflight_error(void);
uint32_t cudaflight_num_envs(cudaflight *);
uint64_t cudaflight_ctx(cudaflight *);
uint32_t cudaflight_grid(cudaflight *);
uint32_t cudaflight_block(cudaflight *);
uint64_t cudaflight_grad_kernel(cudaflight *);
int cudaflight_rate_eval(cudaflight *);
}

#define CK(x) do{ CUresult r=(x); if(r!=CUDA_SUCCESS){const char*m;cuGetErrorString(r,&m);printf("CUDA ERR %s: %s\n",#x,m);return 1;} }while(0)

int main(int argc, char **argv)
{
    const char *cubin = argc > 1 ? argv[1] : "obj/gpu/fw.fatbin";
    uint32_t N = argc > 2 ? (uint32_t)atoi(argv[2]) : 8;

    cudaflight *g = cudaflight_create_eeprom(cubin, N, 0, 0, nullptr);
    if (!g) { printf("create failed: %s\n", cudaflight_error()); return 1; }

    CK(cuCtxSetCurrent((CUcontext)cudaflight_ctx(g)));
    // Test stack-overflow hypothesis: the reverse pass needs a far bigger frame.
    size_t stk = argc > 3 ? (size_t)atoi(argv[3]) * 1024 : 32 * 1024;
    CK(cuCtxSetLimit(CU_LIMIT_STACK_SIZE, stk));
    printf("stack limit = %zu bytes\n", stk);
    int re = cudaflight_rate_eval(g);
    printf("cudaflight_rate_eval (same rebasing, no Enzyme) -> %d (%s)\n", re, re?cudaflight_error():"ok");
    CUfunction fn = (CUfunction)cudaflight_grad_kernel(g);
    unsigned grid = cudaflight_grid(g), block = cudaflight_block(g);
    printf("N=%u grid=%u block=%u fn=%p\n", N, grid, block, (void*)fn);

    CUdeviceptr act, seed, da;
    size_t bytes = (size_t)N * 4 * sizeof(float);
    CK(cuMemAlloc(&act, bytes));
    CK(cuMemAlloc(&seed, bytes));
    CK(cuMemAlloc(&da, bytes));

    std::vector<float> ha(N*4, 0.f), hs(N*4, 0.f);
    for (uint32_t k = 0; k < N; k++) { ha[k*4+0]=0.2f; ha[k*4+3]=0.5f; hs[k*4+0]=1.0f; }
    CK(cuMemcpyHtoD(act, ha.data(), bytes));
    CK(cuMemcpyHtoD(seed, hs.data(), bytes));
    CK(cuMemsetD8(da, 0, bytes));

    void *a=(void*)&act, *s=(void*)&seed, *d=(void*)&da;
    void *args[] = { &a, &s, &d };   // (const float*, const float*, float*)
    printf("launching bfFwStepGrad...\n"); fflush(stdout);
    CK(cuLaunchKernel(fn, grid, 1, 1, block, 1, 1, 0, nullptr, args, nullptr));
    CK(cuCtxSynchronize());
    printf("launch + sync OK\n");

    std::vector<float> hd(N*4);
    CK(cuMemcpyDtoH(hd.data(), da, bytes));
    printf("dActions[0] = %g %g %g %g\n", hd[0], hd[1], hd[2], hd[3]);

    cudaflight_destroy(g);
    return 0;
}
