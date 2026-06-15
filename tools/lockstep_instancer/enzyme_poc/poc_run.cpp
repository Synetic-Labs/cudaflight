// Driver-API runner for the Enzyme NVPTX PoC. Loads poc.cubin, runs the
// Enzyme-generated gradient kernel over a handful of inputs, and checks it
// against the analytic derivative of f:
//   x < 1 : d/dx (x^3 + 2x) = 3x^2 + 2
//   x >= 1: 0 (clamped, flat)
#include <cuda.h>
#include <cmath>
#include <cstdio>
#include <vector>

static bool ck(CUresult r, const char *what)
{
    if (r == CUDA_SUCCESS) return true;
    const char *m = nullptr; cuGetErrorString(r, &m);
    fprintf(stderr, "%s: %s\n", what, m ? m : "?");
    return false;
}
#define OK(c) do { if (!ck((c), #c)) return 1; } while (0)

int main()
{
    OK(cuInit(0));
    CUdevice dev; OK(cuDeviceGet(&dev, 0));
    CUcontext ctx; OK(cuDevicePrimaryCtxRetain(&ctx, dev)); OK(cuCtxPushCurrent(ctx));
    CUmodule mod; OK(cuModuleLoad(&mod, "poc.cubin"));
    CUfunction grad, primal;
    OK(cuModuleGetFunction(&grad, mod, "poc_grad"));
    OK(cuModuleGetFunction(&primal, mod, "poc_primal"));

    std::vector<double> xs = {-2.0, -0.5, 0.0, 0.3, 0.9, 1.5, 3.0};
    uint32_t n = xs.size();
    CUdeviceptr dxs, dg, dy;
    OK(cuMemAlloc(&dxs, n * 8)); OK(cuMemAlloc(&dg, n * 8)); OK(cuMemAlloc(&dy, n * 8));
    OK(cuMemcpyHtoD(dxs, xs.data(), n * 8));

    void *ga[] = { &dxs, &dg, &n };
    void *pa[] = { &dxs, &dy, &n };
    OK(cuLaunchKernel(primal, 1, 1, 1, 32, 1, 1, 0, 0, pa, nullptr));
    OK(cuLaunchKernel(grad,   1, 1, 1, 32, 1, 1, 0, 0, ga, nullptr));
    OK(cuCtxSynchronize());

    std::vector<double> g(n), y(n);
    OK(cuMemcpyDtoH(g.data(), dg, n * 8));
    OK(cuMemcpyDtoH(y.data(), dy, n * 8));

    int bad = 0;
    printf("   x        f(x)       grad      expected\n");
    for (uint32_t i = 0; i < n; i++) {
        double exp = xs[i] < 1.0 ? 3.0 * xs[i] * xs[i] + 2.0 : 0.0;
        bool okg = std::fabs(g[i] - exp) < 1e-6;
        bad += !okg;
        printf("  %5.2f   %8.4f   %8.4f   %8.4f  %s\n",
               xs[i], y[i], g[i], exp, okg ? "ok" : "MISMATCH");
    }
    printf(bad ? "\nFAIL: %d mismatch(es)\n" : "\nPASS: Enzyme NVPTX gradients correct\n", bad);
    return bad ? 1 : 0;
}
