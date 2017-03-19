#include "common.h"

static __inline__ int atomic_inc(volatile int *ptr) {
    unsigned char c;
    __asm__ __volatile__(
            "lock       ;\n"
            "incl %0; sete %1"
            : "+m" (*(ptr)), "=qm" (c)
              : : "memory"
    );
    return c != 0;
}

static __inline__ int atomic_dec(volatile int *ptr) {
    unsigned char rt;
    __asm__ __volatile__(
            "lock;\n"
            "decl %0; sete %1"
            : "+m" (*(ptr)), "=qm" (rt)
              : : "memory"
    );
    return rt != 0;
}

static __inline__ int atomic_cas(volatile int *ptr, int ag, int x) {
    int tmp;
    __asm__ __volatile__("lock;\n"
                         "cmpxchgl %1,%3"
                         : "=a" (tmp) /* %0 EAX, return value */
                         : "r"(x), /* %1 reg, new value */
                         "0" (ag), /* %2 EAX, compare value */
                         "m" (*(ptr)) /* %3 mem, destination operand */
                         : "memory" /*, "cc" content changed, memory and cond register */
                         );
    return tmp;
}

int main (int argc, char **argv) {
#if defined __x86_64 || __i686__

    int nthreads;
#pragma omp parallel
#pragma omp single
    {
        nthreads = omp_get_num_threads();
    }

    int *shared = (int *)malloc(sizeof(int));
    assert(shared);
    *shared = 0;

    unsigned long long start, end, elapsed;
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
            atomic_inc(shared);
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
            atomic_dec(shared);
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
            atomic_cas(shared, 0, 1);
        }
#pragma omp barrier
#pragma omp master
        end = current_time_ns();
    }
    printf("CAS : %f ops/ns\n",
            (double)(REPEAT * nthreads) / (double)(end - start));
#else
    printf("Unsupported architecture\n");
#endif
    return 0;
}
