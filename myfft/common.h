#ifndef COMMON_H
#define COMMON_H
#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
/* for time measurement */
#include <time.h>
// #include <sys/time.h>

#ifdef __MACH__
/*
    macOS doesn't have the posix clock_gettime() function,
    but provides it's own high-res monotonic timer:
    https://developer.apple.com/library/content/qa/qa1398/_index.html
    or (another implementation, seems to be less precise)
    https://stackoverflow.com/questions/11680461/monotonic-clock-on-osx
*/
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
/* for memcpy */
#include <string.h>

/* for type-adapted trigonometric functions (not in PGI) */
#ifndef CC_PGI
#include <tgmath.h>
#else
#include <math.h>
#endif

/* quick and dirty for printing the compiler string */
#ifdef CC_CLANG
#define COMPILER "Clang"
#elif defined CC_GCC
#define COMPILER "GCC"
#elif defined CC_INTEL
#define COMPILER "Intel"
#elif defined CC_PGI
#define COMPILER "PGI"
#endif

/* clang4 doesn't have it anymore */
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

/* definitions for the internal data types */
#ifndef INTERNAL_LONG
typedef unsigned int int_t;
#else
typedef unsigned long int_t;
#endif
#ifndef INTERNAL_DOUBLE
typedef float real_t;
#else
typedef double real_t;
#endif

/* time measurement */
double get_time(void);

#ifdef PRINT_RESULTS
#define PRINTR(x, y, z) print_r(x, y, z);
void print_r(real_t data[], int_t len, char fname[]);
#else
#define PRINTR(x, y, z) ;
#endif

#endif // COMMON_H
