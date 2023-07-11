#include <time.h>
#include <stdio.h>
#include "../../mat_fast_exp.h"


uint32_t *allocateDigits(size_t number);

void generate_testset(unsigned int seed, size_t n, struct bignum array[], int maxsize) {
    srand(seed);
    for (size_t i = 0; i < n; i++) {
        size_t size = (rand() % maxsize) + 1;
        uint32_t *digits = allocateDigits(size);
        for (size_t j = 0; j < size; j++) {
            digits[j] = (uint32_t) rand();
        }
        array[i] = (struct bignum){digits, size, 0};
    }
}

void generate_ascending(unsigned int seed, size_t n, struct bignum array[], int maxsize) {
    srand(seed);
    size_t stepsize = maxsize / n - 1;
    size_t size = 1;
    for (size_t i = 1; i < n; i++) {
        uint32_t *digits = allocateDigits(size);
        for (size_t j = 0; j < size; j++) {
            digits[j] = (uint32_t) rand();
        }
        array[i] = (struct bignum){digits, size, 0};
        size += stepsize;
    }
}

void sortPairs(size_t n, struct bignum array[], int firstlarge) {
    for (size_t i = 0; i < n; i += 2) {
        if (firstlarge) {
            if (array[i].size < array[i + 1].size) {
                struct bignum tmp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = tmp;
            }
        } else {
            if (array[i].size > array[i + 1].size) {
                struct bignum tmp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = tmp;
            }
        }
    }
}

void copyArray(size_t n, struct bignum origin[], struct bignum target[]) {
    for (size_t i = 0; i < n; i++) {
        size_t size = origin[i].size;
        uint32_t *digits = allocateDigits(size);
        for (size_t j = 0; j < size; j++) {
            digits[j] = origin[i].digits[j];
        }
        target[i] = (struct bignum){digits, size, 0};
    }
}

int compareBignum(struct bignum a, struct bignum b) {
    if (a.size != b.size) return 0;
    for (size_t i = 0; i < a.size; i++) {
        if (a.digits[i] != b.digits[i]) return 0;
    }
    return 1;
}

void freeArray(size_t n, struct bignum array[]) {
    for (size_t i = 0; i < n; i++) {
        free(array[i].digits);
    }
}

int main () {
    // Inspired by the example from slide 4
    // https://gra.caps.in.tum.de/b/9a48e342ee6b6a950c3b5102a9e18ddc9b5ccb5c750110e8ee507345d263fb6a/v8-0.pdf
    size_t n_mul = 100;
    size_t n_add = 50000;
    size_t n_div = 300;
    struct bignum tests[2 * n_mul];
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_ascending(1234, 2 * n_mul, tests, 10000);
    printf("Running tests for %zu multiplications\n", n_mul);

    // Normal Multiplication
    struct bignum results_nmul[n_mul];
    struct timespec start_nmul;
    clock_gettime(CLOCK_MONOTONIC, &start_nmul);
    for (size_t i = 0; i < 2 * n_mul; i += 2) {
        results_nmul[i/2] = multiplicationBignum(tests[i], tests[i+1]);
    }
    struct timespec end_nmul;
    clock_gettime(CLOCK_MONOTONIC, &end_nmul);
    double time_nmul = end_nmul.tv_sec - start_nmul.tv_sec + 1e-9 * (end_nmul.tv_nsec - start_nmul.tv_nsec);
    printf("Normal multiplication took %f seconds.\n", time_nmul);

    // Karazuba Multiplication
    struct bignum results_kmul[n_mul];
    struct timespec start_kmul;
    clock_gettime(CLOCK_MONOTONIC, &start_kmul);
    for (size_t i = 0; i < 2 * n_mul; i += 2) {
        results_kmul[i/2] = karazubaMultiplication(tests[i], tests[i+1]);
    }
    struct timespec end_kmul;
    clock_gettime(CLOCK_MONOTONIC, &end_kmul);
    double time_kmul = end_kmul.tv_sec - start_kmul.tv_sec + 1e-9 * (end_kmul.tv_nsec - start_kmul.tv_nsec);
    printf("Karazuba multiplication took %f seconds.\n", time_kmul);
    for (size_t i = 0; i < n_mul; i++) {
        if (!compareBignum(results_nmul[i], results_kmul[i])
                ) {
            printf("%s\n", "Results do not match.");
            freeArray(2 * n_mul, tests);
            freeArray(n_mul, results_nmul);
            freeArray(n_mul, results_kmul);
            return EXIT_FAILURE;
        }
    }
    printf("%s\n", "Results are the same.");
    freeArray(2 * n_mul, tests);
    freeArray(n_mul, results_nmul);
    freeArray(n_mul, results_kmul);

    struct bignum add_tests[2 * n_add];
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, 2 * n_add, add_tests, 10000);
    sortPairs(2 * n_add, add_tests, 1);
    printf("Running tests for %zu additions\n", n_add);
    // Normal Addition
    struct timespec start_nadd;
    clock_gettime(CLOCK_MONOTONIC, &start_nadd);
    for (size_t i = 0; i < 2 * n_add; i += 2) {
        additionBignum(&(add_tests[i]), add_tests[i+1]);
    }
    struct timespec end_add;
    clock_gettime(CLOCK_MONOTONIC, &end_add);
    double time_nadd = end_add.tv_sec - start_nadd.tv_sec + 1e-9 * (end_add.tv_nsec - start_nadd.tv_nsec);
    printf("Normal addition took %f seconds.\n", time_nadd);
    freeArray(2 * n_add, add_tests);

    /*
    struct bignum results_ndiv[2 * n_div];
    struct bignum results_sdiv[2 * n_div];
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, 2 * n_div, results_ndiv, 1000);
    sortPairs(2 * n_div, results_ndiv, 0);
    copyArray(2* n_div, results_ndiv, results_sdiv);
    printf("Running tests for %zu divisions with newton raphson\n", n_add);
    // Normal Division
    struct timespec start_ndiv;
    clock_gettime(CLOCK_MONOTONIC, &start_ndiv);
    for (size_t i = 0; i < 2 * n_mul; i += 2) {
        divisionBignum2(&results_ndiv[i], &results_ndiv[i+1], 50);
    }
    struct timespec end_ndiv;
    clock_gettime(CLOCK_MONOTONIC, &end_ndiv);
    double time_ndiv = end_ndiv.tv_sec - start_ndiv.tv_sec + 1e-9 * (end_ndiv.tv_nsec - start_ndiv.tv_nsec);
    printf("Newton-Raphson divisiom took %f seconds.\n", time_ndiv);
    */

    return EXIT_SUCCESS;
}