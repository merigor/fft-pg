#include <stdio.h>

#define BIT_SCAN_IFZERO 0

int bsr(int input);
int bsf(int input);

int bsr(int input)
{
    int pos, ifzero = BIT_SCAN_IFZERO;
    __asm__("bsrl    %%eax, %1; "
            "cmovzl  %%eax, %2; "
            "movl    %0,    %%eax;"
            : "=a" ( pos )
            : "a" ( input ), "b" ( ifzero )
            :);
    return pos;
}

int bsf(int input)
{
    int pos, ifzero = BIT_SCAN_IFZERO;
    __asm__("bsfl    %%eax, %1;"
            "cmovzl  %%eax, %2;"
            "movl    %0,    %%eax;"
            : "=a" ( pos )
            : "a" ( input ), "b" ( ifzero )
            :);

    return pos;
}

int main(void)
{
    int len = 8;
    for(int i = 0; i < len; ++i)
    {
        printf("bsr = %d, bsf = %d\n", bsr(i), bsf(i));
    }
    int jp = 0;
    int jn = 0;
    int s = bsr(len) - 1;
    printf("s = %d \n", s);
    for(int i = 1; i < len; ++i)
    {
        int k = bsf(i);
        jp ^= (1 << k);
        jn ^= (1 << (s-k));
        printf("i = %d, k = %d, jp = %d, jn = %d\n", i, k, jp, jn);
    }

    return 0;
}
