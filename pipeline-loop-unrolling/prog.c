#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#ifndef UNROLL
#define UNROLL 8
#endif
enum {N = 100000000};

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

int vec_sum(int *vec, int n)
{
    int s = 0;
    for (int i = 0; i < n; i++) {
        s += vec[i];
    }
    return s;
}

int vec_sum_unroll(int *vec, int n)
{
    unsigned int i = 0;

#if UNROLL == 1
    int s = 0;
    for (; i < (unsigned)n; i++) s += vec[i];
    return s;

#elif UNROLL == 2
    int s0 = 0, s1 = 0;
    for (; i + 1 < (unsigned)n; i += 2) {
        s0 += vec[i];
        s1 += vec[i + 1];
    }
    int s = s0 + s1;
    for (; i < (unsigned)n; i++) s += vec[i];
    return s;

#elif UNROLL == 4
    int s0 = 0, s1 = 0, s2 = 0, s3 = 0;
    for (; i + 3 < (unsigned)n; i += 4) {
        s0 += vec[i];
        s1 += vec[i + 1];
        s2 += vec[i + 2];
        s3 += vec[i + 3];
    }
    int s = (s0 + s1) + (s2 + s3);
    for (; i < (unsigned)n; i++) s += vec[i];
    return s;

#elif UNROLL == 8
    int s0=0,s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0;
    for (; i + 7 < (unsigned)n; i += 8) {
        s0 += vec[i];
        s1 += vec[i + 1];
        s2 += vec[i + 2];
        s3 += vec[i + 3];
        s4 += vec[i + 4];
        s5 += vec[i + 5];
        s6 += vec[i + 6];
        s7 += vec[i + 7];
    }
    int s = (s0+s1) + (s2+s3) + (s4+s5) + (s6+s7);
    for (; i < (unsigned)n; i++) s += vec[i];
    return s;

#elif UNROLL == 16
    int s0=0,s1=0,s2=0,s3=0,s4=0,s5=0,s6=0,s7=0;
    int s8=0,s9=0,s10=0,s11=0,s12=0,s13=0,s14=0,s15=0;
    for (; i + 15 < (unsigned)n; i += 16) {
        s0  += vec[i];
        s1  += vec[i + 1];
        s2  += vec[i + 2];
        s3  += vec[i + 3];
        s4  += vec[i + 4];
        s5  += vec[i + 5];
        s6  += vec[i + 6];
        s7  += vec[i + 7];
        s8  += vec[i + 8];
        s9  += vec[i + 9];
        s10 += vec[i + 10];
        s11 += vec[i + 11];
        s12 += vec[i + 12];
        s13 += vec[i + 13];
        s14 += vec[i + 14];
        s15 += vec[i + 15];
    }
    int s = (s0+s1) + (s2+s3) + (s4+s5) + (s6+s7)
          + (s8+s9) + (s10+s11) + (s12+s13) + (s14+s15);
    for (; i < (unsigned)n; i++) s += vec[i];
    return s;

#else
#error "Unsupported UNROLL value. Use 1,2,4,8,16"
#endif
}


void run(int (*fn)(int *, int))
{
    int *vec = malloc(sizeof(*vec) * N);
    for (int i = 0; i < N; i++) {
        vec[i] = 1;
    }

    double t = wtime();
    int s = fn(vec, N);
    t = wtime() - t;

    printf("Time %.6f, sum %d\n", t, s);

    free(vec);
}

int main()
{
    printf("UNROLL=%d\n", UNROLL);
    run(vec_sum_unroll);
    return 0;
}
