// DFT Stefano: N = 8000, ser = 5 sec, omp (2 cores, HT) = 2 sec, optimisation = ?

// void norm(real_t data[], int_t len, int_t factor);
// void setarr(real_t data[], int_t len, real_t value);
// void setrnd(real_t arr[], int_t len);
// void setsin(real_t arr[], int_t len);
// void setdist(real_t arr[], int_t len);

// /* normalise array (divide every element by a factor given as parameter) */
// void norm(real_t data[], int_t len, int_t factor)
// {
//     real_t invfac = (real_t)1.f / factor;
// #ifdef MY_OMP
//     #pragma omp simd
// #endif
//     for(int_t i = 0; i < len; ++i)
//     {
//         data[i] *= invfac;
//     }
// }

// /* set array elements to a constant value (given as parameter) */
// void setarr(real_t arr[], int_t len, real_t value)
// {
// #ifdef MY_OMP
//     #pragma omp simd
// #endif
//     for(int_t i = 0; i < len; ++i)
//     {
//         arr[i] = value;
//     }
// }

// /* set array elements to random values (scaled by length) */
// void setrnd(real_t arr[], int_t len)
// {
//     real_t invlen = (real_t)1.f / len;
// #ifdef MY_OMP
//     #pragma omp simd
// #endif
//     for(int_t i = 0; i < len; ++i)
//     {
//         arr[i] = rand() * invlen;
//     }
// }

// /* set array elements to one period of a sine function */
// void setsin(real_t arr[], int_t len)
// {
//     for(int_t i = 0; i < len; ++i)
//     {
//         arr[i] = sin((real_t)(2 * M_PI * i / len));
//     }
// }

// /* set array elements to a distorted sine function */
// void setdist(real_t arr[], int_t len)
// {
//     real_t scale = 4 * (real_t)M_PI / len;
// #ifdef MY_OMP
//     #pragma omp simd
// #endif
//     for(int_t i = 0; i < len; ++i)
//     {
//         arr[i] = sin(i * scale) + (real_t)0.2f * sin(40 * i * scale);
//     }
// }

// invert bits for each index.
// n is number of samples and a the array of the samples
/*void invert_bits(complex_t a[], int_t n)
{
    int_t mv = n / 2;
    int_t k, rev = 0;
    complex_t b;

    for (int_t i = 1; i < n; i++) // run tru all the indexes from 1 to n
    {
        k = i;
        mv = n / 2;
        rev = 0;
        while (k > 0) // invert the actual index
        {
            if ((k % 2) > 0)
            {
                rev += mv;
            }
            k = k / 2;
            mv = mv / 2;
        }

        // switch the actual sample and the bitinverted one
        if (i < rev)
        {
            b = a[rev];
            a[rev] = a[i];
            a[i] = b;
        }
    }
}*/

// void fft_unroll(struct cmplx_t* d, struct cmplx_t** T, int_t len, int dir);

/* bit-reversal algorithm using Grey Code from Jennifer Elaan */
// void reversey(int* b, int len)
// {
//     int i, jp, jn, k, s;
//     int tmp;
//     jp = 0;
//     jn = 0;

//     /*
//         bsr = bit scan reverse, exists in x86 since Haswell
//         bsf = bit scan forward, --------- '' --------------
//     */
//     s = bsr(len) - 1;
//     for(i = 1; i < len; i++)
//     {
//         k = bsf(i);
//         jp ^= (1<<k);
//         jn ^= (1<<(s-k));
//         if(jp<jn)
//         {
//             tmp = b[jp];
//             b[jp] = b[jn];
//             b[jn] = tmp;
//         }
//     }
// }

/* experimenting with manual loop unrolling - WIP */
// void fft_unroll(struct cmplx_t* __restrict__ d,
//                 struct cmplx_t** __restrict__ T,
//                 int_t len, int dir)
// {
//     int_t ll = len >> 1;

//     for(int_t M = 1, j = 0, len_M = ll; M <= ll; M <<= 1, ++j, len_M >>= 1)
//     {
//         for (int_t i = 0; i < len_M; ++i)
//         {
//             int_t l0 = (i << (j+1));
//             int_t r0 = l0 + M;
//             for (int_t k = 0, l = l0, r = r0;
//                  k < M-UNROLL && l < ll;
//                  k += UNROLL, l += UNROLL, r += UNROLL)
//             {
//                 /* printf("l = %lu, r = %lu\n", l, r); */
//                 real_t Xev_re[UNROLL] = { RE(d[l]),   RE(d[l+1]),
//                                           RE(d[l+2]), RE(d[l+3]) };
//                 real_t Xev_im[UNROLL] = { dir * IM(d[l]),   dir * IM(d[l+1]),
//                                           dir * IM(d[l+2]), dir * IM(d[l+3]) };

//                 real_t tmp[UNROLL] = { dir * IM(d[r]),   dir * IM(d[r+1]),
//                                        dir * IM(d[r+2]), dir * IM(d[r+3]) };
//                 real_t Xod_re[UNROLL] =
//                     { RE(d[r])   * RE(T[j][k])   - tmp[0] * IM(T[j][k]),
//                       RE(d[r+1]) * RE(T[j][k+1]) - tmp[1] * IM(T[j][k+1]),
//                       RE(d[r+2]) * RE(T[j][k+2]) - tmp[2] * IM(T[j][k+2]),
//                       RE(d[r+3]) * RE(T[j][k+3]) - tmp[3] * IM(T[j][k+3]) };
//                 real_t Xod_im[UNROLL] =
//                     { RE(d[r])   * IM(T[j][k])   + tmp[0] * RE(T[j][k]),
//                       RE(d[r+1]) * IM(T[j][k+1]) + tmp[1] * RE(T[j][k+1]),
//                       RE(d[r+2]) * IM(T[j][k+2]) + tmp[2] * RE(T[j][k+2]),
//                       RE(d[r+3]) * IM(T[j][k+3]) + tmp[3] * RE(T[j][k+3]) };

//                 RE(d[l])   = Xev_re[0] + Xod_re[0];
//                 RE(d[l+1]) = Xev_re[1] + Xod_re[1];
//                 RE(d[l+2]) = Xev_re[2] + Xod_re[2];
//                 RE(d[l+2]) = Xev_re[3] + Xod_re[3];

//                 IM(d[l])   = dir * (Xev_im[0] + Xod_im[0]);
//                 IM(d[l+1]) = dir * (Xev_im[1] + Xod_im[1]);
//                 IM(d[l+2]) = dir * (Xev_im[2] + Xod_im[2]);
//                 IM(d[l+3]) = dir * (Xev_im[3] + Xod_im[3]);

//                 RE(d[r])   = Xev_re[0] - Xod_re[0];
//                 RE(d[r+1]) = Xev_re[1] - Xod_re[1];
//                 RE(d[r+2]) = Xev_re[2] - Xod_re[2];
//                 RE(d[r+3]) = Xev_re[3] - Xod_re[3];

//                 IM(d[r])   = dir * (Xev_im[0] - Xod_im[0]);
//                 IM(d[r+1]) = dir * (Xev_im[1] - Xod_im[1]);
//                 IM(d[r+2]) = dir * (Xev_im[2] - Xod_im[2]);
//                 IM(d[r+3]) = dir * (Xev_im[3] - Xod_im[3]);
//             }
//         }
//     }
// }