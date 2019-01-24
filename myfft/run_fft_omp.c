#include "fft.h"
#include <omp.h>
#define MASTER 0
#define NTHREADS 8

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

    #pragma omp parallel // num_threads(NTHREADS) proc_bind(spread)
    {
        /* doubles used for time measurement */
        double t1 = 0.0, t2 = 0.0, total1 = 0.0, total2 = 0.0;

        int tid = omp_get_thread_num();
        if(tid == MASTER)
        {
            t1 = get_time();
        }

        struct ft_data_t *data = malloc(sizeof(struct ft_data_t));
        /* initialisation (N is passed via external define) */
        /* WARNING: CHECK THE LENGTH (N) - SHOULD BE A POWER OF TWO !!! */
        /*
            TODO: implement an "and"-reduction to ensure that all
                  threads have initialised their data properly
        */
        if(init(data, len) == EXIT_SUCCESS)
        {
            /* generate input data */
            setdist_c(data->inout_c, data->len);

            if(tid == MASTER)
            {
                t2 = get_time();

                printf("compiler: %s, problem size: %lu (elements) with real_t = %lu bytes, iterations: %lu\n", COMPILER, (unsigned long) data->len, sizeof(real_t), (unsigned long) reps);
                printf("init: %.16f\n", t2 - t1);

                /* convert complex to real and print original signal */
                #ifdef PRINT_RESULTS
                    char *f_o = "out/fft_original.txt";
                    c2r(data->inout_c, data->in_r, data->len);
                    PRINTR(data->in_r, data->len, f_o);
                #endif

                total1 = get_time();
            }

            /* precomputing the factors for the fft */
            precalc(data->factors, data->len);

            /* running fft - shuffle, fft, shuffle, i-fft, normalisation */
            #pragma omp for
            for(int_t i = 0; i < reps; ++i)
            {
                t1 = get_time();
                shuffle2(data->inout_c, data->len, data->bitlen);
                #ifdef FFT_IN_PLACE
                    fft(data->inout_c, data->factors, data->len, FT_FOR);
                    shuffle2(data->inout_c, data->len, data->bitlen);
                    fft(data->inout_c, data->factors, data->len, FT_BAK);
                #else
                    fft(data->inout_c, data->inout2_c, data->factors,
                        data->len, FT_FOR);
                    shuffle2(data->inout2_c, data->len, data->bitlen);
                    fft(data->inout2_c, data->inout_c, data->factors,
                        data->len, FT_BAK);
                #endif /* FFT_IN_PLACE */
                norm_c(data->inout_c, data->len, data->len);
                t2 = get_time();
            }
            printf("tid: %lu, time: %.16f\n", (unsigned long)tid, (t2 - t1));

            if(tid == MASTER)
            {
                total2 = get_time();
                printf("total fft runtime: %.16f\n", (total2 - total1));

                /* convert complex data back to real for restored signal */
                #ifdef PRINT_RESULTS
                    char *f_r = "out/fft_restored.txt";
                    c2r(data->inout_c, data->out_r, data->len);
                    PRINTR(data->out_r, data->len, f_r);
                #endif
            }

        } /* end of init - success */
        else
        {
            fprintf(stderr, "ERROR: thread %d has failed initialisation.\n", tid);
        }

        /* finalise */
        cleanup(data);
        free(data);
    } /* omp parallel */

    return EXIT_SUCCESS;
}
