// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once
#define M_E        2.7182818284590452354
#define M_LOG2E    1.4426950408889634074
#define M_LN2      0.69314718055994530942
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.78539816339744830962
#define M_1_PI     0.31830988618379067154
#define M_SQRT2    1.41421356237309504880
#define INFINITY   (__builtin_inff())
#define NAN        (__builtin_nanf(""))
#define HUGE_VAL   (__builtin_inf())
#define HUGE_VALF  (__builtin_inff())
#define isnan(x)    __builtin_isnan(x)
#define isinf(x)    __builtin_isinf(x)
#define isfinite(x) __builtin_isfinite(x)
#define signbit(x)  __builtin_signbit(x)
float fabsf(float); double fabs(double);
float sqrtf(float); double sqrt(double);
float sinf(float); double sin(double);
float cosf(float); double cos(double);
float tanf(float); double tan(double);
float asinf(float); double asin(double);
float acosf(float); double acos(double);
float atanf(float); double atan(double);
float atan2f(float, float); double atan2(double, double);
float expf(float); double exp(double);
float exp2f(float); double exp2(double);
float logf(float); double log(double);
float log2f(float); double log2(double);
float log10f(float); double log10(double);
float powf(float, float); double pow(double, double);
float fmodf(float, float); double fmod(double, double);
float floorf(float); double floor(double);
float ceilf(float); double ceil(double);
float roundf(float); double round(double);
float truncf(float); double trunc(double);
float rintf(float); double rint(double);
float nearbyintf(float); double nearbyint(double);
long lrintf(float); long lrint(double);
long long llrintf(float); long long llrint(double);
long lroundf(float); long lround(double);
float fminf(float, float); double fmin(double, double);
float fmaxf(float, float); double fmax(double, double);
float copysignf(float, float); double copysign(double, double);
float hypotf(float, float); double hypot(double, double);
float ldexpf(float, int); double ldexp(double, int);
float frexpf(float, int *); double frexp(double, int *);
float scalbnf(float, int); double scalbn(double, int);
float cbrtf(float); double cbrt(double);
float expm1f(float); double expm1(double);
float log1pf(float); double log1p(double);
float sinhf(float); double sinh(double);
float coshf(float); double cosh(double);
float tanhf(float); double tanh(double);
