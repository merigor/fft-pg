#include "ft_tools.h"

int main(int argc, char* argv[])
{
    int_t reps = 0;
    int len;
    #ifdef N
        len = N;
    #else
        if(argc > 1)
        {
            len = atoi(argv[1]);
        }
        else
        {
            fprintf(stderr, "ERROR: N is undefined and the input length is not submitted.\n");
            return EXIT_FAILURE;
        }
    #endif
    #ifdef R
        reps = (int_t) R;
    #else
        if(argc > 2)
        {
            reps = (int_t) atoi(argv[2]);
        }
        else
        {
            fprintf(stderr, "ERROR: R is undefined and number of repetitions is not submitted.\n");
            return EXIT_FAILURE;
        }
    #endif

    complex_t *in, *out;
    real_t *temp;
    plan_t pf, pb;

    /* doubles used for time measurement */
    double t1, t2, total1, total2;

    /* initialisation */
    t1 = get_time();
    in = (complex_t*) MALLOC(sizeof(*in) * (size_t)len);
    out = (complex_t*) MALLOC(sizeof(*out) * (size_t)len);
    pf = PLAN_1D(len, in, out, FFTW_FORWARD, FFTW_MEASURE);
    pb = PLAN_1D(len, out, in, FFTW_BACKWARD, FFTW_MEASURE);
    setdist_c(in, (int_t)len);
    t2 = get_time();
    printf("compiler: %s, problem size: %d (elements) with real_t = %lu bytes, iterations: %lu\n", COMPILER, len, sizeof(*in[0]), (unsigned long) reps);
    printf("init: %.16f\n", t2 - t1);

    total1 = get_time();
    /* forward, backward, normalisation */
    for(int_t i = 0; i < reps; ++i)
    {
        t1 = get_time();
        RUN(pf);
        RUN(pb);
        norm_c(in, (int_t) len, (int_t) len);
        t2 = get_time();
        printf("rep: %lu, time: %.16f\n", (unsigned long)i, (t2 - t1));
    }

    total2 = get_time();
    printf("total fftw3 runtime: %.16f\n", (total2 - total1));

    /* print resulting array for verification */
    #ifdef PRINT_RESULTS
        char *fname = "out/fft_fftw3.txt";
        temp = malloc(sizeof(*temp) * (size_t)len);
        c2r(in, temp, (int_t) len);
        PRINTR(temp, (int_t) len, fname);
        free(temp);
    #endif

    /* cleaning up */
    CLEANUP(pf);
    CLEANUP(pb);
    FREE(in);
    FREE(out);

    return EXIT_SUCCESS;
}
