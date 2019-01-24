#include <stdio.h>

#define LEN 64
typedef unsigned long int_t;

void precomp_bitperm(int_t* old, int_t* new, int_t oldlen);

void precomp_bitperm(int_t* __restrict__ old, int_t* __restrict__ new, int_t oldlen)
{
    int_t i, j, newlen = 2*oldlen;
    for(i = 0; i < oldlen; ++i)
    {
        new[i] = old[i] * 2;
    }
    for(i = oldlen, j = 0; i < newlen; ++i, ++j)
    {
        new[i] = old[j] * 2 + 1;
    }
}

int main(void)
{
    // int_t old[LEN] = {0, 2, 1, 3};
    // int_t old[LEN] = {0, 4, 2, 6, 1, 5, 3, 7};
    // int_t old[LEN] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    // int_t old[LEN] = {0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30, 1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31};
    int_t old[LEN] = {0, 32, 16, 48, 8, 40, 24, 56, 4, 36, 20, 52, 12, 44, 28, 60, 2, 34, 18, 50, 10, 42, 26, 58, 6, 38, 22, 54, 14, 46, 30, 62, 1, 33, 17, 49, 9, 41, 25, 57, 5, 37, 21, 53, 13, 45, 29, 61, 3, 35, 19, 51, 11, 43, 27, 59, 7, 39, 23, 55, 15, 47, 31, 63};
    int_t new[2 * LEN];
    precomp_bitperm(old, new, LEN);
    for(int_t i = 0; i < 2 * LEN; ++i)
    {
        printf("%lu, ", new[i]);
    }
    printf("\n");
    return 0;
}
