// SPDX-License-Identifier: GPL-3.0-or-later
// Device libm: thin wrappers over CUDA libdevice (linked as bitcode).
// The device build compiles everything with -fno-builtin, so libm calls
// stay calls and resolve here instead of becoming LLVM intrinsics the
// NVPTX backend can't select.

double __nv_exp(double);
double __nv_log(double);
double __nv_log2(double);
double __nv_log10(double);
double __nv_pow(double, double);
double __nv_sqrt(double);
double __nv_sin(double);
double __nv_cos(double);
double __nv_tan(double);
double __nv_asin(double);
double __nv_acos(double);
double __nv_atan(double);
double __nv_atan2(double, double);
double __nv_fmod(double, double);
double __nv_floor(double);
double __nv_ceil(double);
double __nv_round(double);
double __nv_trunc(double);
double __nv_rint(double);
double __nv_fabs(double);
double __nv_fmin(double, double);
double __nv_fmax(double, double);
double __nv_copysign(double, double);
double __nv_hypot(double, double);
double __nv_ldexp(double, int);
double __nv_cbrt(double);
double __nv_expm1(double);
double __nv_log1p(double);
double __nv_sinh(double);
double __nv_cosh(double);
double __nv_tanh(double);
long long __nv_llrint(double);
float __nv_expf(float);
float __nv_logf(float);
float __nv_log2f(float);
float __nv_log10f(float);
float __nv_powf(float, float);
float __nv_sqrtf(float);
float __nv_sinf(float);
float __nv_cosf(float);
float __nv_tanf(float);
float __nv_asinf(float);
float __nv_acosf(float);
float __nv_atanf(float);
float __nv_atan2f(float, float);
float __nv_fmodf(float, float);
float __nv_floorf(float);
float __nv_ceilf(float);
float __nv_roundf(float);
float __nv_truncf(float);
float __nv_rintf(float);
float __nv_fabsf(float);
float __nv_fminf(float, float);
float __nv_fmaxf(float, float);
float __nv_copysignf(float, float);
float __nv_hypotf(float, float);
float __nv_ldexpf(float, int);
float __nv_cbrtf(float);
long long __nv_llrintf(float);

double exp(double x) { return __nv_exp(x); }
double log(double x) { return __nv_log(x); }
double log2(double x) { return __nv_log2(x); }
double log10(double x) { return __nv_log10(x); }
double pow(double x, double y) { return __nv_pow(x, y); }
double sqrt(double x) { return __nv_sqrt(x); }
double sin(double x) { return __nv_sin(x); }
double cos(double x) { return __nv_cos(x); }
double tan(double x) { return __nv_tan(x); }
double asin(double x) { return __nv_asin(x); }
double acos(double x) { return __nv_acos(x); }
double atan(double x) { return __nv_atan(x); }
double atan2(double y, double x) { return __nv_atan2(y, x); }
double fmod(double x, double y) { return __nv_fmod(x, y); }
double floor(double x) { return __nv_floor(x); }
double ceil(double x) { return __nv_ceil(x); }
double round(double x) { return __nv_round(x); }
double trunc(double x) { return __nv_trunc(x); }
double rint(double x) { return __nv_rint(x); }
double nearbyint(double x) { return __nv_rint(x); }
double fabs(double x) { return __nv_fabs(x); }
double fmin(double x, double y) { return __nv_fmin(x, y); }
double fmax(double x, double y) { return __nv_fmax(x, y); }
double copysign(double x, double y) { return __nv_copysign(x, y); }
double hypot(double x, double y) { return __nv_hypot(x, y); }
double ldexp(double x, int e) { return __nv_ldexp(x, e); }
double cbrt(double x) { return __nv_cbrt(x); }
double expm1(double x) { return __nv_expm1(x); }
double log1p(double x) { return __nv_log1p(x); }
double sinh(double x) { return __nv_sinh(x); }
double cosh(double x) { return __nv_cosh(x); }
double tanh(double x) { return __nv_tanh(x); }
long lrint(double x) { return (long)__nv_llrint(x); }
long long llrint(double x) { return __nv_llrint(x); }
long lround(double x) { return (long)__nv_llrint(__nv_round(x)); }

float expf(float x) { return __nv_expf(x); }
float exp2f(float x) { return __nv_powf(2.0f, x); }
float logf(float x) { return __nv_logf(x); }
float log2f(float x) { return __nv_log2f(x); }
float log10f(float x) { return __nv_log10f(x); }
float powf(float x, float y) { return __nv_powf(x, y); }
float sqrtf(float x) { return __nv_sqrtf(x); }
float sinf(float x) { return __nv_sinf(x); }
float cosf(float x) { return __nv_cosf(x); }
float tanf(float x) { return __nv_tanf(x); }
float asinf(float x) { return __nv_asinf(x); }
float acosf(float x) { return __nv_acosf(x); }
float atanf(float x) { return __nv_atanf(x); }
float atan2f(float y, float x) { return __nv_atan2f(y, x); }
float fmodf(float x, float y) { return __nv_fmodf(x, y); }
float floorf(float x) { return __nv_floorf(x); }
float ceilf(float x) { return __nv_ceilf(x); }
float roundf(float x) { return __nv_roundf(x); }
float truncf(float x) { return __nv_truncf(x); }
float rintf(float x) { return __nv_rintf(x); }
float nearbyintf(float x) { return __nv_rintf(x); }
float fabsf(float x) { return __nv_fabsf(x); }
float fminf(float x, float y) { return __nv_fminf(x, y); }
float fmaxf(float x, float y) { return __nv_fmaxf(x, y); }
float copysignf(float x, float y) { return __nv_copysignf(x, y); }
float hypotf(float x, float y) { return __nv_hypotf(x, y); }
float ldexpf(float x, int e) { return __nv_ldexpf(x, e); }
float cbrtf(float x) { return __nv_cbrtf(x); }
long lrintf(float x) { return (long)__nv_llrintf(x); }
long long llrintf(float x) { return __nv_llrintf(x); }
long lroundf(float x) { return (long)__nv_llrintf(__nv_roundf(x)); }
