#ifndef FT_COMMON_H
#define FT_COMMON_H

#include "ft_tools.h"

struct ft_data_t
{
    real_t *in_r;
    real_t *out_r;
    complex_t *inout_c;
    complex_t *inout2_c;
    complex_t **factors;
    int_t len;
    int bitlen;
    /*
        padding, adapts to the different sizes of int_t,
        prevents compiler warning with clang
    */
    char pad[12 + sizeof(unsigned long) - sizeof(int_t)];
}  __attribute__ ((aligned (__BIGGEST_ALIGNMENT__)));

#define FT_FOR 1
#define FT_BAK -1

int init(struct ft_data_t *d, int_t len);
void cleanup(struct ft_data_t *d);

#endif /* FT_COMMON_H */
