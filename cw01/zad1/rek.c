#include <stdio.h>
#include <math.h>
#include <time.h>

int rekurencja_ogonowa(int n) {
        if (n == 0)
                return 1;

        return rekurencja_ogonowa(n-1);
}

int main() {
        struct timespec start={0,0}, end={0,0};
        double dt;

        clock_gettime(CLOCK_REALTIME, &start);
        for (int i = 0; i < 1e9; ++i)
                rekurencja_ogonowa(50);
        clock_gettime(CLOCK_REALTIME, &end);

        //time in seconds
        dt = ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);
        printf("%f\n", dt);

        return 0;
}
