#include <time.h>
#include <stdio.h>
#include "../mat_fast_exp.h"

int compareBignum(struct bignum a, struct bignum b) {
    if (a.size != b.size) return 0;
    for (size_t i = 0; i < a.size; i++) {
        if (a.digits[i] != b.digits[i]) return 0;
    }
    return 1;
}

void freeResults(size_t n, struct matrix2x2 normal[], struct cmp_matrix2x2 compact[]) {
    for (size_t i = 0; i < n; i++) {
        free2x2(normal[i]);
        freeCmp2x2(compact[i]);
    }
}

int main () {
    // Inspired by the example from slide 4
    // https://gra.caps.in.tum.de/b/9a48e342ee6b6a950c3b5102a9e18ddc9b5ccb5c750110e8ee507345d263fb6a/v8-0.pdf
    size_t n = 10000;
    size_t stepsize = 5;
    printf("Running tests for matrix exponentiation for n up to %zu in steps of size %zu: %zu exponentiation in total\n", n, stepsize, n / stepsize);

    struct matrix2x2 results_normal[n/stepsize];
    struct timespec start_normal;
    clock_gettime(CLOCK_MONOTONIC, &start_normal);
    for (int i = 0; i < n / stepsize; i++) {
        struct matrix2x2 base = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(1), bignumOfInt(2)};
        results_normal[i] = powMatrix2x2(base, i * stepsize, multiplicationBignum);
    }
    struct timespec end_normal;
    clock_gettime(CLOCK_MONOTONIC, &end_normal);
    double time_normal = end_normal.tv_sec - start_normal.tv_sec + 1e-9 * (end_normal.tv_nsec - start_normal.tv_nsec);
    printf("Normal matrix exponentiation took %f seconds.\n", time_normal);

    struct cmp_matrix2x2 results_cmp[n];
    struct timespec start_cmp;
    clock_gettime(CLOCK_MONOTONIC, &start_cmp);
    for (int i = 0; i < n / stepsize; i++) {
        struct cmp_matrix2x2 base = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};
        results_cmp[i] = powCmpMatrix2x2(base, i * stepsize, multiplicationBignum);
    }
    struct timespec end_cmp;
    clock_gettime(CLOCK_MONOTONIC, &end_cmp);
    double time_cmp = end_cmp.tv_sec - start_cmp.tv_sec + 1e-9 * (end_cmp.tv_nsec - start_cmp.tv_nsec);
    printf("Compact matrix exponentiation took %f seconds.\n", time_cmp);

    for (size_t i = 0; i < n / stepsize; i++) {
        struct matrix2x2 normal = results_normal[i];
        struct cmp_matrix2x2 compact = results_cmp[i];
        if (!compareBignum(normal.a12, normal.a21)
            | !compareBignum(normal.a11, compact.xm1)
            | !compareBignum(normal.a12, compact.x)
            | !compareBignum(normal.a22, compact.xp1)
                ) {
            printf("%s\n", "Results do not match.");
            freeResults(n / stepsize, results_normal, results_cmp);
            return EXIT_FAILURE;
        }
    }
    printf("%s\n", "Results are the same.");
    freeResults(n / stepsize, results_normal, results_cmp);
    return EXIT_SUCCESS;
}