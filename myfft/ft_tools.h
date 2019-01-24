#ifndef FT_TOOLS_H
#define FT_TOOLS_H

/* prevent data type collision */
#ifdef FFTW
#include "my_fftw_complex.h"
#else
#include "complex.h"
#endif

#ifdef PRINT_RESULTS
#define PRINTC(x, y, z) print_c(x, y, z);
void print_c(complex_t data[], int_t len, char fname[]);
#else
#define PRINTC(x, y, z) ;
#endif

void r2c(real_t in[], complex_t out[], int_t len);
void c2r(complex_t in[], real_t out[], int_t len);
void conj_arr(complex_t a[], int_t n);
void norm_c(complex_t a[], int_t n, int_t factor);
void setdist_c(complex_t *X, int_t len);

#endif /* FT_TOOLS_H */
