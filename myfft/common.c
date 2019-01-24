#include "common.h"

#ifdef PRINT_RESULTS
void print_r(real_t data[], int_t len, char fname[])
{
    FILE *fp = fopen(fname, "w+");

    for(int_t i = 0; i < len; ++i)
    {
        fprintf(fp, "[%lu] = %f\n", (unsigned long)i, (double)data[i]);
    }
    fprintf(fp, "%s", "\n");

    fclose(fp);
}
#endif

double get_time(void)
{
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    /*clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return ((double)mts.tv_sec + (double)mts.tv_nsec / 1000000000.0);*/
    uint64_t t = mach_absolute_time();
    static mach_timebase_info_data_t sTimebaseInfo;
    if(sTimebaseInfo.denom == 0)
    {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
    return ((double)t * sTimebaseInfo.numer / sTimebaseInfo.denom) / 1000000000.0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return ((double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0);
#endif
}
