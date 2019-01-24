#ifndef MY_FFTW_COMPLEX_H
#define MY_FFTW_COMPLEX_H

#include <fftw3.h>
#include "common.h"

#ifndef INTERNAL_DOUBLE
    typedef fftwf_complex complex_t;
    typedef fftwf_plan plan_t;
    #define MALLOC(x) fftwf_malloc(x)
    #define PLAN_1D(a, b, c, d, e) fftwf_plan_dft_1d(a, b, c, d, e)
    #define RUN(x) fftwf_execute(x)
    #define CLEANUP(x) fftwf_destroy_plan(x)
    #define FREE(x) fftwf_free(x)
#else
    typedef fftw_complex complex_t;
    typedef fftw_plan plan_t;
    #define MALLOC(x) fftw_malloc(x)
    #define PLAN_1D(a, b, c, d, e) fftw_plan_dft_1d(a, b, c, d, e)
    #define RUN(x) fftw_execute(x)
    #define CLEANUP(x) fftw_destroy_plan(x)
    #define FREE(x) fftw_free(x)
#endif

#define RE(x) (x)[0]
#define IM(x) (x)[1]

#endif /* MY_FFTW_COMPLEX_H */
