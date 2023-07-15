#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../../mat_fast_exp.h"

void generate_testset(unsigned int seed, size_t n, size_t stepsize, struct bignum array[][3]) {
    srand(seed);
    for (size_t i = 0; i < n; i++) {
        struct cmp_matrix2x2 base = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};
        base = powCmpMatrix2x2(base, (rand() % 5000), multiplicationBignum);
        array[i][0] = base.xm1;
        array[i][1] = base.x;
        array[i][2] = base.xp1;
    }
}

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

// Source: https://gra.caps.in.tum.de/material/ERA_Blatt_07_Lsg.pdf
static void write_file(const char *path, const char *string) {
    // Write the string to the file specified by path.
    FILE *file;
    if (!(file = fopen(path, "w"))) {
        perror("Error opening file");
        return;
    }

    const size_t stringlen = strlen(string);
    if (fwrite(string, 1, stringlen, file) != stringlen) {
        fprintf(stderr, "Error writing to file\n");
    }
    fclose(file);
}

int main () {
    // Inspired by the example from slide 4
    // https://gra.caps.in.tum.de/b/9a48e342ee6b6a950c3b5102a9e18ddc9b5ccb5c750110e8ee507345d263fb6a/v8-0.pdf
    size_t n = 50;
    int iterations = 100;
    size_t stepsize = 10;
    struct bignum tests[2*(n/stepsize)][3];
    printf("%s\n", "Generating test matrices, this may take some time...");
    generate_testset(1234, 2*n, stepsize, tests);
    printf("Running tests for %zu multiplications %d times: %zu multiplications in total\n", n / stepsize, iterations, n * iterations);
    struct matrix2x2 normalset[2*(n/stepsize)];
    struct cmp_matrix2x2 cmpset[2*(n/stepsize)];
    for (size_t i = 0; i < 2*n; i++) {
        normalset[i] = (struct matrix2x2) {tests[i][0], tests[i][1], tests[i][1], tests[i][2]};
        cmpset[i] = (struct cmp_matrix2x2) {tests[i][0], tests[i][1], tests[i][2]};
    }

    struct matrix2x2 results_normal[n/stepsize];
    struct timespec start_normal;
    clock_gettime(CLOCK_MONOTONIC, &start_normal);
    for (int i = 0; i < n/stepsize * 2; i += 2) {
        results_normal[i/2] = mulMatrix2x2(normalset[i], normalset[i+1], multiplicationBignum, additionBignum);
    }
    struct timespec end_normal;
    clock_gettime(CLOCK_MONOTONIC, &end_normal);
    double time_normal = end_normal.tv_sec - start_normal.tv_sec + 1e-9 * (end_normal.tv_nsec - start_normal.tv_nsec);
    printf("Normal matrix multiplication took %f seconds.\n", time_normal);

    struct cmp_matrix2x2 results_cmp[n*iterations];
    struct timespec start_cmp;
    clock_gettime(CLOCK_MONOTONIC, &start_cmp);
    for (int i = 0; i < iterations; i++) {
        for (size_t j = 0; j < 2*n; j += 2) {
            results_cmp[j/2 + n * i] = mulCmpMatrix2x2(cmpset[j], cmpset[j+1], multiplicationBignum, additionBignum);
        }
    }
    struct timespec end_cmp;
    clock_gettime(CLOCK_MONOTONIC, &end_cmp);
    double time_cmp = end_cmp.tv_sec - start_cmp.tv_sec + 1e-9 * (end_cmp.tv_nsec - start_cmp.tv_nsec);
    printf("Compact matrix multiplication took %f seconds.\n", time_cmp);

    //free allocated space
    for (size_t i = 0; i < 2*(n / stepsize); i++ ) {
        free(tests[i][0].digits);
        free(tests[i][1].digits);
        free(tests[i][2].digits);
    }

    for (size_t i = 0; i < n / stepsize; i++) {
        struct matrix2x2 normal = results_normal[i];
        struct cmp_matrix2x2 compact = results_cmp[i];
        if (!compareBignum(normal.a12, normal.a21)
            | !compareBignum(normal.a11, compact.xm1)
            | !compareBignum(normal.a12, compact.x)
            | !compareBignum(normal.a22, compact.xp1)
        ) {
            printf("%s\n", "Results do not match.");
            freeResults(n / iterations, results_normal, results_cmp);
            return EXIT_FAILURE;
        }
    }
    printf("%s\n", "Results are the same.");


    printf("%s\n", "Generating benchmark results");
    printf("This will take about %f seconds, because there are %d iterations\n", iterations * (time_normal + time_cmp),
           iterations);
    struct timespec start;
    struct timespec end;
    char results[sizeof (char ) * 25 * (n / stepsize)];
    char converter[100];
    results[0] = 0;
    for (size_t i = 0; i < n / stepsize; i++) {
        // Add n size
        sprintf(converter, "%zu", i * stepsize);
        strcat(results, converter);

        // Calculate normal
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t j = 0; j < iterations; j++) {
            free2x2(mulMatrix2x2(normalset[i], normalset[i+1], multiplicationBignum, additionBignum));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Add normal average
        double avg = (end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec));
        sprintf(converter, ";%f", avg);
        strcat(results, converter);

        // Calculate compact
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t j = 0; j < iterations; j++) {
            freeCmp2x2(mulCmpMatrix2x2(cmpset[i], cmpset[i+1], multiplicationBignum, additionBignum));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Add compact average
        avg = (end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec));
        sprintf(converter, ";%f\n", avg);
        strcat(results, converter);
    }
    write_file("results.csv", results);
    freeResults(n / stepsize, results_normal, results_cmp);
    return EXIT_SUCCESS;
}