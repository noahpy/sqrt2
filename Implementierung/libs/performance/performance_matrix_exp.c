#include <time.h>
#include <stdio.h>
#include <string.h>
#include "../mat_fast_exp.h"

int compareBignum(struct bignum a, struct bignum b) {
    if (a.size != b.size) return 0;
    for (size_t i = 0; i < a.size; i++) {
        if (a.digits[i] != b.digits[i]) return 0;
    }
    return 1;
}

void freeResults(size_t n, struct matrix2x2 normal[], struct cmp_matrix2x2 compact[], struct cmp_matrix2x2 sequence[]) {
    for (size_t i = 0; i < n; i++) {
        free2x2(normal[i]);
        freeCmp2x2(compact[i]);
        freeCmp2x2(sequence[i]);
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
    size_t n = 10001;
    size_t iterations = 1000;
    size_t stepsize = 100;
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

    struct cmp_matrix2x2 results_s[n];
    struct timespec start_f;
    clock_gettime(CLOCK_MONOTONIC, &start_f);
    for (int i = 0; i < n / stepsize; i++) {
        results_s[i] = sequence(i * stepsize);
    }
    struct timespec end_f;
    clock_gettime(CLOCK_MONOTONIC, &end_f);
    double time_f = end_f.tv_sec - start_f.tv_sec + 1e-9 * (end_f.tv_nsec - start_f.tv_nsec);
    printf("Sequence took %f seconds.\n", time_f);

    for (size_t i = 0; i < n / stepsize; i++) {
        struct matrix2x2 normal = results_normal[i];
        struct cmp_matrix2x2 compact = results_cmp[i];
        struct cmp_matrix2x2 folge = results_s[i];
        if (!compareBignum(normal.a12, normal.a21)
            | !compareBignum(normal.a11, compact.xm1)
            | !compareBignum(normal.a12, compact.x)
            | !compareBignum(normal.a22, compact.xp1)
              | !compareBignum(compact.xm1, folge.xm1)
              | !compareBignum(compact.x, folge.x)
              | !compareBignum(compact.xp1, folge.xp1)
                ) {
            printf("%s\n", "Results do not match.");
            freeResults(n / stepsize, results_normal, results_cmp, results_s);
            return EXIT_FAILURE;
        }
    }
    printf("%s\n", "Results are the same.");
    freeResults(n / stepsize, results_normal, results_cmp, results_s);

    printf("%s\n", "Generating benchmark results");
    printf("This will take about %f seconds, because there are %zu iterations\n", iterations * (time_normal + time_cmp),
            iterations);
    struct timespec start;
    struct timespec end;
    char results[sizeof (char ) * 35 * (n / stepsize)];
    char converter[100];
    results[0] = 0;
    for (size_t i = 0; i < n / stepsize; i++) {
        // Add n size
        sprintf(converter, "%zu", i * stepsize);
        strcat(results, converter);

        // Calculate normal
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t j = 0; j < iterations; j++) {
            struct matrix2x2 base = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(1), bignumOfInt(2)};
            free2x2(powMatrix2x2(base, i * stepsize, multiplicationBignum));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Add normal average
        double avg = (end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec));
        sprintf(converter, ";%f", avg);
        strcat(results, converter);

        // Calculate sequence
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t j = 0; j < iterations; j++) {
            freeCmp2x2(sequence(i * stepsize));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Add sequence average
        avg = (end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec));
        sprintf(converter, ";%f", avg);
        strcat(results, converter);

        // Calculate compact
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t j = 0; j < iterations; j++) {
            struct cmp_matrix2x2 base = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};
            freeCmp2x2(powCmpMatrix2x2(base, i * stepsize, multiplicationBignum));
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Add compact average
        avg = (end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec));
        sprintf(converter, ";%f\n", avg);
        strcat(results, converter);
    }
    write_file("results.csv", results);
    return EXIT_SUCCESS;
}