#ifndef FFT_H
#define FFT_H

/* fft based on code from "introduction to signal processing" from rutgers */
/*
    faster bit reversal function, taken from
    http://www.katjaas.nl/bitreversal/bitreversal.html
*/

#include "ft_common.h"

void shuffle(complex_t *X, int_t len, int bitlen);
void shuffle2(complex_t *X, int_t len, int bitlen);

void precalc(complex_t **T, int_t len);
#ifdef FFT_IN_PLACE
void fft(complex_t *in, complex_t **T, int_t len, int dir);
#else
void fft(complex_t *in, complex_t *out, complex_t **T, int_t len, int dir);
#endif

#endif /* FFT_H */
