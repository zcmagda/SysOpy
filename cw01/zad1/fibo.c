#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main () {
        struct timespec start={0,0}, end={0,0};
        double dt;

        volatile long long n = 1e9;

        int first = 0, second = 1, next, c,d;

        clock_gettime(CLOCK_REALTIME, &start);
        for (d = 0; d < 10; d++) {
                for (c = 0; c < n; c++) {
                        if (c <= 1)
                        next = c;
                        else {
                                next = first + second;
                                first = second;
                                second = next;
                        }
                }
        }
        clock_gettime(CLOCK_REALTIME, &end);

        //time in seconds
        dt = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
        printf("%f\n", dt);

   return 0;
}
