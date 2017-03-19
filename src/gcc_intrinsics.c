#include "common.h"

int main (int argc, char **argv) {
    int nthreads;
#pragma omp parallel
#pragma omp single
    {
        nthreads = omp_get_num_threads();
    }

    int *shared = (int *)malloc(sizeof(int));
    assert(shared);
    *shared = 0;

    unsigned long long start, end;
    // Test increment
#pragma omp parallel
    {
        int i;
        for (i = 0; i < WARMUP + REPEAT; i++) {
            if (i == WARMUP) {
#pragma omp barrier
#pragma omp master
                start = current_time_ns();
            }
            __sync_add_and_fetch(shared, 1);
        }
#pragma omp barrier
#pragma omp master
        end = current_time_ns();
    }
    printf("INC : %f ops/ns\n",
            (double)(REPEAT * nthreads) / (double)(end - start));

    // Test decrement
#pragma omp parallel
    {
        int i;
        for (i = 0; i < WARMUP + REPEAT; i++) {
            if (i == WARMUP) {
#pragma omp barrier
#pragma omp master
                start = current_time_ns();
            }
            __sync_sub_and_fetch(shared, 1);
        }
#pragma omp barrier
#pragma omp master
        end = current_time_ns();
    }
    printf("DEC : %f ops/ns\n",
            (double)(REPEAT * nthreads) / (double)(end - start));

    // Test CAS
#pragma omp parallel
    {
        int i;
        for (i = 0; i < WARMUP + REPEAT; i++) {
            if (i == WARMUP) {
#pragma omp barrier
#pragma omp master
                start = current_time_ns();
            }
            __sync_bool_compare_and_swap(shared, 0, 1);
        }
#pragma omp barrier
#pragma omp master
        end = current_time_ns();
    }
    printf("CAS : %f ops/ns\n",
            (double)(REPEAT * nthreads) / (double)(end - start));

    return 0;
}
