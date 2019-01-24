#include "ft_tools.h"

void r2c(real_t* __restrict__ in, complex_t* __restrict__ out, int_t len)
{
    for(int_t i = 0; i < len; ++i)
    {
        RE(out[i]) = in[i];
        IM(out[i]) = (real_t)0.0;
    }
}

void c2r(complex_t* __restrict__ in, real_t* __restrict__ out, int_t len)
{
    for(int_t i = 0; i < len; ++i)
    {
        out[i] = RE(in[i]);
    }
}

/* converts array to conjugated complex inplace */
void conj_arr(complex_t a[], int_t n)
{
    for(int_t i = 0; i < n; ++i)
    {
        IM(a[i]) *= -1;
    }
}

/* normalisation of a complex array, sets imaginary part to zero */
void norm_c(complex_t a[], int_t n, int_t factor)
{
    real_t invfac = (real_t)1.f / factor;

    for(int_t i = 0; i < n; ++i)
    {
        RE(a[i]) *= invfac;
        IM(a[i]) = (real_t) 0.f;
    }
}

/* set array elements to a distorted sine function */
void setdist_c(complex_t arr[], int_t len)
{
    real_t scale = 4 * (real_t)M_PI / len, angle;

    for(int_t i = 0; i < len; ++i)
    {
        angle = i * scale;
        RE(arr[i]) = sin(angle) + (real_t)0.2f * sin(40 * angle);
        IM(arr[i]) = (real_t) 0.f;
    }
}

#ifdef PRINT_RESULTS
void print_c(complex_t data[], int_t len, char fname[])
{
    FILE *fp = fopen(fname, "w+");

    for(int_t i = 0; i < len; ++i)
    {
        fprintf(fp, "[%lu].re = %f\n", (unsigned long) i, (double)RE(data[i]));
        fprintf(fp, "[%lu].im = %f\n", (unsigned long) i, (double)IM(data[i]));
    }
    fprintf(fp, "%s", "\n");

    fclose(fp);
}
#endif
