#include "complex.h"

/* returns conjugated complex */
FUNCATTR complex_t* cmplx_conj(complex_t *c)
{
    complex_t *res = malloc(sizeof(complex_t));
    RE(*res) = RE(*c);
    IM(*res) = -IM(*c);
    return res;
}

/* calculates (integer) power of a complex number */
FUNCATTR complex_t* cmplx_pow(complex_t *c, int_t p)
{
    real_t r = sqrt(RE(*c) * RE(*c) + IM(*c) * IM(*c));
    real_t tau = (real_t)atan2(IM(*c), RE(*c));
    complex_t *res = malloc(sizeof(complex_t));
    real_t factor = pow(r, (real_t)p);
    RE(*res) = factor * cos(p * tau);
    IM(*res) = factor * sin(p * tau);
    return res;
}

/* convert exponential form to cos/sin */
FUNCATTR complex_t* cmplx_e2trig(real_t tau)
{
    /* e ^ (i * PI * tau * n) = cos(n * tau) + i * sin(n * tau) */
    complex_t* res = malloc(sizeof(complex_t));
    RE(*res) = cos(tau);
    IM(*res) = sin(tau);
    return res;
}

/* complex exponential */
FUNCATTR complex_t* cmplx_cexp(complex_t *c)
{
    real_t r = exp(RE(*c));
    complex_t *res = malloc(sizeof(complex_t));
    RE(*res) = r * cos(IM(*c));
    IM(*res) = r * sin(IM(*c));
    return res;
}

/* complex addition */
FUNCATTR complex_t* cmplx_add(complex_t* __restrict__ a, complex_t* __restrict__ b)
{
    complex_t* res = malloc(sizeof(complex_t));
    RE(*res) = RE(*a) + RE(*b);
    IM(*res) = IM(*a) + IM(*b);
    return res;
}

/* complex subtraction */
FUNCATTR complex_t* cmplx_sub(complex_t* __restrict__ a, complex_t* __restrict__ b)
{
    complex_t* res = malloc(sizeof(complex_t));
    RE(*res) = RE(*a) - RE(*b);
    IM(*res) = IM(*a) - IM(*b);
    return res;
}

/* complex multiplication */
FUNCATTR complex_t* cmplx_mul(complex_t* __restrict__ a, complex_t* __restrict__ b)
{
    complex_t* res = malloc(sizeof(*res));
    RE(*res) = RE(*a) * RE(*b) - IM(*a) * IM(*b);
    IM(*res) = RE(*a) * IM(*b) + IM(*a) * RE(*b);
    return res;
}
