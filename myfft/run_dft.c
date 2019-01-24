#include "dft.h"

int main(int argc, char* argv[])
{
    #ifdef N
        const int_t len = (int_t) N;
    #else
        int_t len = 0;
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
        const int_t reps = (int_t) R;
    #else
        int_t reps = 0;
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
    /* doubles for time measurement */
    double t1, t2, total1, total2;
    t1 = get_time();
    struct ft_data_t *data = calloc (1, sizeof(*data));

    if(init(data, len) != EXIT_SUCCESS)
    {
        fprintf(stderr, "ERROR: initialisation failed.\n");
        cleanup(data);
        free(data);
        return EXIT_FAILURE;
    }
    /* initialising input array with some signal */
    setdist_c(data->inout_c, data->len);
    t2 = get_time();

    printf("compiler: %s, problem size: %lu (elements) with real_t = %lu bytes, iterations: %lu\n", COMPILER, (unsigned long) data->len, sizeof(real_t), (unsigned long) reps);
    printf("init: %.16f\n", t2 - t1);

    #ifdef PRINT_RESULTS
        char *f_o = "out/dft_original.txt";
        c2r(data->inout_c, data->in_r, data->len);
        PRINTR(data->in_r, data->len, f_o);
    #endif

    /* dft forward, dft backward, normalisation */
    total1 = get_time();
    for(int_t i = 0; i < reps; ++i)
    {
        t1 = get_time();
        dft(data->inout_c, data->inout2_c, data->len);
        dft(data->inout2_c, data->inout_c, data->len);
        norm_c(data->inout_c, data->len, data->len);
        t2 = get_time();
        printf("rep: %lu, time: %.16f\n", (unsigned long)i, (t2 - t1));
    }
    total2 = get_time();
    printf("total dft runtime: %.16f\n", (total2 - total1));

    /* print result for verification */
    #ifdef PRINT_RESULTS
        char *f_r = "out/dft_restored.txt";
        c2r(data->inout_c, data->out_r, data->len);
        PRINTR(data->out_r, data->len, f_r);
    #endif

    /* cleaning up */
    cleanup(data);
    free(data);

    return EXIT_SUCCESS;
}
