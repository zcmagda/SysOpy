#include<stdio.h>
#include <time.h>

int main()  {
        struct timespec start={0,0}, end={0,0};
        double dt;

        volatile long long i;

        clock_gettime(CLOCK_REALTIME, &start);
        for (i=0; i<1e9; i++) {};
        clock_gettime(CLOCK_REALTIME, &end);

        //time in seconds
        dt = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
        printf("%f\n", dt);

    return 0;
}
