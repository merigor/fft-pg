#include "fft.h"

/* precomputes coefficients, saves about a factor of N */
void precalc(complex_t **T, int_t len)
{
    real_t angle0 = (real_t) -M_PI, angle, scale;
    int_t M_2;

    for(int_t M = 2, j = 0; M <= len; M <<= 1, ++j)
    {
        complex_t V = {(real_t) 1.f, (real_t) 0.f};
        scale = 1 / (M - 1);
        angle = angle0 * scale;
        complex_t W = {cos(angle), sin(angle)};
        M_2 = M >> 1;
        for(int_t i = 0; i < M_2; ++i)
        {
            T[j][i] = V;
            RE(V) = RE(V) * RE(W) - IM(V) * IM(W);
            IM(V) = RE(V) * IM(W) + IM(V) * RE(W);
        }
    }
}

/*
    does the index-based array transformation:
    swaps elements with ones that have the bit-reversed index
    WARNING: check len - it must be a power of 2
*/
/* this version of shuffle contributes about half of the runtime */
void shuffle(complex_t *X, int_t len, int bitlen)
{
    int_t r, n2;
    complex_t temp;

    for (int_t n = 0; n < len; n++)
    {
        r = 0;
        n2 = n;
        for (int m = bitlen - 1; m >= 0; m--)
        {
            /* bit-reversal part */
            if ((n2 >> m) == 1)                /* if m-th bit is set */
            {
                r += (1 << (bitlen - 1 - m));  /* set "mirrored" bit in r */
                n2 -= (1 << m);                /* eliminate "used" bit */
            }
        }
        if (r < n) continue;        /* swap only the first half of the ns */
        temp = *(X + n);            /* swap regular and the reverse elements */
        *(X + n) = *(X + r);
        *(X + r) = temp;
    }
}

/*
    faster bit reversal function, taken from
    http://www.katjaas.nl/bitreversal/bitreversal.html
*/
void shuffle2(complex_t *X, int_t len, int bitlen)
{
    int_t zeros;
    /* to hold bitwise negated or odd values */
    int_t nodd, noddrev;
    complex_t temp;

    /* frequently used 'constants' */
    int_t halfn = len >> 1;
    int_t quartn = len >> 2;
    int_t nmin1 = len - 1;

    /* variable initialisations */
    int_t forward = halfn;
    int_t rev = 1;

    /* start of bitreversed permutation loop, N/4 iterations */
    for(int_t i = quartn; i > 0; i--)
    {
        /* Gray code generator for even values: */
        /* counting ones is easier */
        nodd = ~i;
        for(zeros = 0; nodd & 1; zeros++)
        {
            /* find trailing zeroes in i */
            nodd >>= 1;
        }

        /* toggle one bit of forward */
        forward ^= (int_t)2 << zeros;

        /* toggle one bit of reversed */
        rev ^= quartn >> zeros;

        /* swap even and ~even conditionally */
        if(forward < rev)
        {
            temp = X[forward];
            X[forward] = X[rev];
            X[rev] = temp;

            /* compute the bitwise negations */
            nodd = nmin1 ^ forward;
            noddrev = nmin1 ^ rev;

            /* swap bitwise-negated pairs */
            temp = X[nodd];
            X[nodd] = X[noddrev];
            X[noddrev] = temp;
        }

        /* compute the odd values from the even */
        nodd = forward ^ 1;
        noddrev = rev ^ halfn;

        /* swap odd unconditionally */
        temp = X[nodd];
        X[nodd] = X[noddrev];
        X[noddrev] = temp;
    }
}

#ifdef FFT_IN_PLACE
/*
    the actual in-place computation part of FFT
    expects:
        - data array of complex numbers
        - coefficient matrix (from precomputing function)
        - length of the data array
        - direction of transformation
*/
void fft(complex_t* __restrict__ in, complex_t** __restrict__ T,
         int_t len, int dir)
{
    int_t ll = len >> 1;

#pragma omp parallel
    for(int_t M = 1, j = 0, len_M = ll; M <= ll; M <<= 1, ++j, len_M >>= 1)
    {
#pragma omp for
        for (int_t i = 0; i < len_M; ++i)
        {
            int_t l0 = (i << (j+1));
            int_t r0 = l0 + M;
            /* printf("----- l0 = %lu, r0 = %lu -----\n", l0, r0); */
            for (int_t k = 0, l = l0, r = r0; k < M; ++k, ++l, ++r)
            {
                /* printf("l = %lu, r = %lu\n", l, r); */
                real_t Xev_re = RE(in[l]);
                real_t Xev_im = dir * IM(in[l]);
                real_t Xod_re = RE(in[r]) * RE(T[j][k]) -
                                dir * IM(in[r]) * IM(T[j][k]);
                real_t Xod_im = RE(in[r]) * IM(T[j][k]) +
                                dir * IM(in[r]) * RE(T[j][k]);
                RE(in[l]) = Xev_re + Xod_re;
                IM(in[l]) = dir * (Xev_im + Xod_im);
                RE(in[r]) = Xev_re - Xod_re;
                IM(in[r]) = dir * (Xev_im - Xod_im);
            }
        }
    }
}
#else
/*
    simulated out-of-place computation of FFT by copying the data array to the output and doing in-place on the output array
    expects:
        - data array of complex numbers
        - output array
        - coefficient matrix (from precomputing function)
        - length of the data array
        - direction of transformation
*/
void fft(complex_t* __restrict__ in, complex_t* __restrict__ out,
         complex_t** __restrict__ T, int_t len, int dir)
{
#pragma omp parallel
    memcpy(out, in, sizeof(*in)*len);
    int_t ll = len >> 1;

    for(int_t M = 1, j = 0, len_M = ll; M <= ll; M <<= 1, ++j, len_M >>= 1)
    {
#pragma omp for
        for (int_t i = 0; i < len_M; ++i)
        {
            int_t l0 = (i << (j+1));
            int_t r0 = l0 + M;
            /* printf("----- l0 = %lu, r0 = %lu -----\n", l0, r0); */
            for (int_t k = 0, l = l0, r = r0; k < M; ++k, ++l, ++r)
            {
                /* printf("l = %lu, r = %lu\n", l, r); */
                real_t Xev_re = RE(out[l]);
                real_t Xev_im = dir * IM(out[l]);
                real_t Xod_re = RE(out[r]) * RE(T[j][k]) -
                                dir * IM(in[r]) * IM(T[j][k]);
                real_t Xod_im = RE(out[r]) * IM(T[j][k]) +
                                dir * IM(out[r]) * RE(T[j][k]);
                RE(out[l]) = Xev_re + Xod_re;
                IM(out[l]) = dir * (Xev_im + Xod_im);
                RE(out[r]) = Xev_re - Xod_re;
                IM(out[r]) = dir * (Xev_im - Xod_im);
            }
        }
    }
}
#endif /* FFT_IN_PLACE */
