#include <stdio.h>
#include <stdlib.h>

typedef char int_t;
typedef unsigned long print_t;

void to_bin(int_t n, int bitlen, char** s);

void to_bin(int_t n, int bitlen, char** s)
{
    for(int i = 0; i < bitlen; ++i)
    {
        (*s)[bitlen-i-1] = (n & 1)? '1': '0';
        n >>= 1;
    }
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "ERROR: parameter missing.\n");
        return EXIT_FAILURE;
    }

    int_t len = (int_t) atoi(argv[1]);
    int_t zeros;
    /* to hold bitwise negated or odd values */
    int_t noddrev;

    /* frequently used 'constants' */
    int_t halfn = len >> 1;
    int_t quartn = len >> 2;
    int_t nmin1 = len - 1;

    /* variable initialisations */
    int_t forward = halfn;
    int_t rev = 1;


    int bitlen = sizeof(len) * 8;
    char* s = calloc((print_t)bitlen, sizeof(char));

    /* start of bitreversed permutation loop, N/4 iterations */
    for(int_t i = quartn; i > 0; i--)
    {
        /* Gray code generator for even values: */
        /* counting ones is easier */
        int_t nodd = ~i;
        to_bin(i, bitlen, &s);
        printf("i = %s, ", s);
        to_bin(nodd, bitlen, &s);
        printf("negated i = %s\n", s);
        /* right-shift until first bit is not 1 - in non-negated counting 0s */
        for(zeros = 0; nodd & 1; zeros++)
        {
            /* find trailing zeroes in i */
            nodd >>= 1;
        }
        printf("trailing zeros: %lu\n", (print_t)zeros);

        /* toggle one bit of forward */
        printf("forward %lu, XOR with %lu\n", (print_t)forward, (print_t)((int_t)2 << zeros));
        forward ^= (int_t)2 << zeros;
        printf("forward now: %lu\n", (print_t)forward);

        /* toggle one bit of reversed */
        printf("rev %lu XOR with %lu\n", (print_t) rev, (print_t)(quartn >> zeros));
        rev ^= quartn >> zeros;
        printf("rev now: %lu\n if forward < rev, swap\n", (print_t) rev);

        /* swap even and ~even conditionally */
        if(forward < rev)
        {
            printf("swap X[%lu] with X[%lu]\n", (print_t) forward, (print_t) rev);

            /* compute the bitwise negations */
            nodd = nmin1 ^ forward;
            noddrev = nmin1 ^ rev;

            /* swap bitwise-negated pairs */
            printf("swap bitwise negated X[%lu] with X[%lu]\n", (print_t) nodd, (print_t) noddrev);
        }

        /* compute the odd values from the even */
        printf("generating odd values\n");
        nodd = forward ^ 1;
        noddrev = rev ^ halfn;

        /* swap odd unconditionally */
        printf("swap X[%lu] with X[%lu]\n\n", (print_t) nodd, (print_t) noddrev);
    }

    free(s);
    return EXIT_SUCCESS;
}
