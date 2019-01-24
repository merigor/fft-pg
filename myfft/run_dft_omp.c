#include "dft.h"
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
        int tid = omp_get_thread_num();
        /* time measurement */
        double t1 = 0.0, t2 = 0.0, total1 = 0.0, total2 = 0.0;

        if(tid == MASTER)
        {
            t1 = get_time();
        }

        struct ft_data_t *data = calloc (1, sizeof(*data));
        /*
            TODO: implement an "and"-reduction to ensure that all
                  threads have initialised their data properly
        */
        if(init(data, len) == EXIT_SUCCESS)
        {
            /* initialising input array with generated data */
            setdist_c(data->inout_c, data->len);

            if(tid == MASTER)
            {
                t2 = get_time();

                printf("OMP-Version with %d threads\n", omp_get_num_threads());
                printf("compiler: %s, problem size: %lu (elements) with real_t = %lu bytes, iterations: %lu\n", COMPILER, (unsigned long) data->len, sizeof(real_t), (unsigned long) reps);
                printf("init: %.16f\n", t2 - t1);

                #ifdef PRINT_RESULTS
                    char *f_o = "out/dft_original.txt";
                    c2r(data->inout_c, data->in_r, data->len);
                    PRINTR(data->in_r, data->len, f_o);
                #endif

                total1 = get_time();
            }

            #pragma omp for
            for(int_t i = 0; i < reps; ++i)
            {
                t1 = get_time();
                dft(data->inout_c, data->inout2_c, data->len);
                dft(data->inout2_c, data->inout_c, data->len);
                norm_c(data->inout_c, data->len, data->len);
                t2 = get_time();
            }
            printf("tid: %lu, time: %.16f\n", (unsigned long)tid, (t2 - t1));

            if(tid == MASTER)
            {
                total2 = get_time();
                printf("total dft runtime: %.16f\n", (total2 - total1));

                #ifdef PRINT_RESULTS
                    char *f_r = "out/dft_restored.txt";
                    c2r(data->inout_c, data->out_r, data->len);
                    PRINTR(data->out_r, data->len, f_r);
                #endif
            }
        } /* end of init-success */
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
