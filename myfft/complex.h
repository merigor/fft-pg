#ifndef COMPLEX_H
#define COMPLEX_H

#include "common.h"

#ifdef INTERNAL_COMPLEX_STRUCT
struct cmplx_t
{
    real_t re;
    real_t im;
};
typedef struct cmplx_t complex_t;
#define RE(x) (x).re
#define IM(x) (x).im
#else
typedef struct cmplx_t { real_t _vals[2]; } complex_t;
#define RE(x) (x)._vals[0]
#define IM(x) (x)._vals[1]
#endif

// #define FUNCATTR inline __attribute__((pure)) static
#define FUNCATTR

complex_t* cmplx_conj(complex_t *c);
complex_t* cmplx_e2trig(real_t tau);
complex_t* cmplx_cexp(complex_t *c);
complex_t* cmplx_add(complex_t *a, complex_t *b);
complex_t* cmplx_sub(complex_t *a, complex_t *b);
complex_t* cmplx_mul(complex_t *a, complex_t *b);
complex_t* cmplx_pow(complex_t *c, int_t p);

#endif // COMPLEX_H
