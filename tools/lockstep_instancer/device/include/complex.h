#pragma once
// C99 complex arithmetic is native in clang on any target; only the
// macros/functions need declaring.
#define complex _Complex
#define _Complex_I (__extension__ 1.0iF)
#define I _Complex_I
#define crealf(x) (__real__ (x))
#define cimagf(x) (__imag__ (x))
#define creal(x)  (__real__ (x))
#define cimag(x)  (__imag__ (x))
#define conjf(x)  (__builtin_conjf(x))
#define conj(x)   (__builtin_conj(x))
float cabsf(float _Complex x);
double cabs(double _Complex x);
