#include "ft_common.h"

int init(struct ft_data_t *d, int_t l)
{
    /* length of 0 makes no sense */
    if(l == 0)
    {
        return EXIT_FAILURE;
    }
    d->len = l;

    /* calculating bit-length */
    for(d->bitlen = -1; l > 0; d->bitlen++, l >>= 1);
    /* check for power of 2 */
    if(d->len != (int_t)(1 << d->bitlen))
    {
        return EXIT_FAILURE;
    }

    d->in_r = calloc(d->len, sizeof(*d->in_r));
    if(!(d->in_r))
    {
        return EXIT_FAILURE;
    }

    d->out_r = calloc(d->len, sizeof(*d->out_r));
    if(!(d->out_r))
    {
        return EXIT_FAILURE;
    }

    d->inout_c = calloc(d->len, sizeof(*d->inout_c));
    if(!(d->inout_c))
    {
        return EXIT_FAILURE;
    }

    d->inout2_c = calloc(d->len, sizeof(*d->inout2_c));
    if(!(d->inout2_c))
    {
        return EXIT_FAILURE;
    }

    d->factors = calloc((int_t)d->bitlen, sizeof(*d->factors));
    l = d->len >> 1;
    for(int_t i = 0; i < (int_t)d->bitlen; ++i)
    {
        d->factors[i] = calloc(l, sizeof(**d->factors));
    }

    return EXIT_SUCCESS;
}

/* deallocates memory */
void cleanup(struct ft_data_t *d)
{
    if(d->inout_c)
    {
        free(d->inout_c);
    }
    if(d->inout2_c)
    {
        free(d->inout2_c);
    }
    if(d->out_r)
    {
        free(d->out_r);
    }
    if(d->in_r)
    {
        free(d->in_r);
    }
    if(d->factors)
    {
        for(int_t i = 0; i < (int_t)d->bitlen; ++i)
        {
            free(d->factors[i]);
        }
        free(d->factors);
    }
}
