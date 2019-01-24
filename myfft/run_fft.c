#include "fft.h"

int main(int argc, char* argv[])
{
    int_t len = 0, reps = 0;
    #ifdef N
        len = (int_t) N;
    #else
        if(argc > 1)
        {
            len = (int_t) atoi(argv[1]);
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
    /* doubles used for time measurement */
    double t1, t2, total1, total2;

    /* initialisation (N is passed via external define) */
    /* WARNING: CHECK THE LENGTH (N) - SHOULD BE A POWER OF TWO !!! */
    t1 = get_time();
    struct ft_data_t *data = malloc(sizeof(struct ft_data_t));
    if(init(data, len) != EXIT_SUCCESS)
    {
        fprintf(stderr, "ERROR: initialisation failed.\n");
        cleanup(data);
        free(data);
        return EXIT_FAILURE;
    }
    /* generate input data */
    setdist_c(data->inout_c, data->len);
    t2 = get_time();

    printf("compiler: %s, problem size: %lu (elements) with real_t = %lu bytes, iterations: %lu\n", COMPILER, (unsigned long) data->len, sizeof(real_t), (unsigned long) reps);
    printf("init: %.16f\n", t2 - t1);

    /* convert complex to real and print original signal */
    #ifdef PRINT_RESULTS
        char *f_o = "out/fft_original.txt";
        c2r(data->inout_c, data->in_r, data->len);
        PRINTR(data->in_r, data->len, f_o);
    #endif

    /* running fft - precompute, shuffle, fft, shuffle, i-fft, normalisation */
    total1 = get_time();
    /* precomputing the factors for the fft */
    precalc(data->factors, data->len);
    for(int_t i = 0; i < reps; ++i)
    {
        t1 = get_time();
        shuffle2(data->inout_c, data->len, data->bitlen);
#ifdef FFT_IN_PLACE
        fft(data->inout_c, data->factors, data->len, FT_FOR);

        shuffle2(data->inout_c, data->len, data->bitlen);
        fft(data->inout_c, data->factors, data->len, FT_BAK);
#else
        fft(data->inout_c, data->inout2_c, data->factors, data->len, FT_FOR);

        shuffle2(data->inout2_c, data->len, data->bitlen);
        fft(data->inout2_c, data->inout_c, data->factors, data->len, FT_BAK);
#endif /* FFT_IN_PLACE */
        norm_c(data->inout_c, data->len, data->len);
        t2 = get_time();
        printf("rep: %lu, time: %.16f\n", (unsigned long)i, (t2 - t1));
    }

    total2 = get_time();
    printf("total fft runtime: %.16f\n", (total2 - total1));

    /* convert complex data back to real for restored signal */
    #ifdef PRINT_RESULTS
        char *f_r = "out/fft_restored.txt";
        c2r(data->inout_c, data->out_r, data->len);
        PRINTR(data->out_r, data->len, f_r);
    #endif

    /* finalise */
    cleanup(data);
    free(data);

    return EXIT_SUCCESS;
}
