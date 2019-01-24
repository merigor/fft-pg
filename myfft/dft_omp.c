#include "dft.h"
#include <omp.h>

void dft(complex_t* __restrict__ in, complex_t* __restrict__ out, const int_t len)
{
    const real_t c = 2 * (real_t)M_PI / len;

#pragma omp parallel for simd default(none) shared(in, out)
    for(int_t k = 0; k < len; ++k)
    {
        real_t rr = (real_t) 0.0;
        real_t ii = (real_t) 0.0;
        real_t tau = c * k;

        for(int_t n = 0; n < len; ++n)
        {
            real_t tau2 = tau * n;
            real_t ct = cos(tau2);
            real_t st = sin(tau2);
            rr += (RE(in[n]) * ct + IM(in[n]) * st);
            ii += (RE(in[n]) * st + IM(in[n]) * ct);
        }

        RE(out[k]) = (real_t)rr;
        IM(out[k]) = (real_t)ii;
    }
}
