// Enzyme-on-NVPTX proof of concept.
//
// Differentiates a small float function with a data-dependent branch and a
// saturation (the shapes that appear all over a flight controller) on the
// GPU, via __enzyme_autodiff, compiled through the SAME pipeline as the
// firmware: clang-20 -> nvptx64 bitcode -> Enzyme opt pass -> ptxas -> cubin.
//
// Verdict: grad[k] must equal the analytic derivative of f at xs[k].

#include <stdint.h>

#define KERNEL __attribute__((nvptx_kernel))

static inline unsigned self(void)
{
    return (unsigned)__nvvm_read_ptx_sreg_ctaid_x() * (unsigned)__nvvm_read_ptx_sreg_ntid_x()
         + (unsigned)__nvvm_read_ptx_sreg_tid_x();
}

// f(x): cubic + linear, a branch, and a clamp — exercises control flow and a
// piecewise-constant region (zero gradient) like a controller's saturation.
//   x < 1 : 3x^2 + 2     (from x^3 + 2x)
//   x >= 1: 0            (clamped, flat)
__attribute__((noinline)) double f(double x)
{
    double y;
    if (x < 1.0) {
        y = x * x * x + 2.0 * x;
    } else {
        y = 3.0; // constant region -> derivative 0
    }
    return y;
}

extern double __enzyme_autodiff(void *, double);

KERNEL void poc_grad(const double *xs, double *grad, uint32_t n)
{
    const unsigned k = self();
    if (k >= n) {
        return;
    }
    grad[k] = __enzyme_autodiff((void *)f, xs[k]);
}

// primal too, so the runner can sanity-check the forward value
KERNEL void poc_primal(const double *xs, double *ys, uint32_t n)
{
    const unsigned k = self();
    if (k >= n) {
        return;
    }
    ys[k] = f(xs[k]);
}
