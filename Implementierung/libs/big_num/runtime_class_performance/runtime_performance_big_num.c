#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../../mat_fast_exp.h"

uint32_t *allocateDigits(size_t number);

void generate_testset(unsigned int seed, size_t n, struct bignum array[], size_t points) {
    srand(seed);
    
    for (size_t i = 0; i < points*2; i=i+2) {
        uint32_t *digits1 = allocateDigits((i/2) * n/points);
        uint32_t *digits2 = allocateDigits((i/2) * n/points);
        for (size_t j = 0; j < (i/2) * n/points; j++) {
            digits1[j] = (uint32_t) rand();
            digits2[j] = (uint32_t) rand();
        }
        array[i] = (struct bignum){digits1, (i/2) * n/points, 0};
        array[i+1] = (struct bignum){digits2, (i/2) * n/points, 0};
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

// argv[0]: iterations
// argv[1]: maxSize
// int main(int argc, char *argv[]) {
//   int iterations = *argv[1] - '0';
//   int maxSize = *argv[2] - '0';
//   printf("hello\n");
//   uint32_t *digits1 = allocateDigits(maxSize);
//   uint32_t *digits2 = allocateDigits(maxSize);
//   for (size_t j = 0; j < maxSize; j++) {
//       digits1[j] = (uint32_t) rand();
//       digits2[j] = (uint32_t) rand();
//   }
//   struct bignum a = (struct bignum){digits1, maxSize, 0};
//   struct bignum b = (struct bignum){digits2, maxSize, 0};
//   struct bignum c;
//   for (int i = 0; i < iterations; i++) {
//     c = multiplicationBignum(a, b);
//   }
//   free(c.digits);
//   free(a.digits);
//   free(b.digits);
// }

int main() {
    // Inspired by the example from slide 4
    // https://gra.caps.in.tum.de/b/9a48e342ee6b6a950c3b5102a9e18ddc9b5ccb5c750110e8ee507345d263fb6a/v8-0.pdf

    // MULTIPLICATION

    int iterations = 20;
    int maxSize = 10000;
    int points = 20;
    struct bignum tests[2 * points];
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, maxSize, tests, points);
    printf("Running tests for multiplication with max size of %d, %d points distributed even, %d iterations per calculation\n", maxSize, points, iterations);
    struct bignum results_nmul[maxSize];

    for (size_t i = 0; i < 2 * points; i += 2) {
      // Normal Multiplication
      struct timespec start_nmul;
      clock_gettime(CLOCK_MONOTONIC, &start_nmul);

      for (size_t j = 0; j < iterations; j++) {
          multiplicationBignum(tests[i], tests[i+1]);
      }

      struct timespec end_nmul;
      clock_gettime(CLOCK_MONOTONIC, &end_nmul);
      double time_nmul = end_nmul.tv_sec - start_nmul.tv_sec + 1e-9 * (end_nmul.tv_nsec - start_nmul.tv_nsec);
      printf("Normal multiplication with size %lu took %f seconds.\n", (i/2) * maxSize/points, time_nmul);
    }
    freeArray(2 * points, tests);

    // ADDITION
    iterations = 200000;
    maxSize = 10000;
    points = 20;
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, maxSize, tests, points);
    printf("Running tests for addition with max size of %d, %d points distributed even, %d iterations per calculation\n", maxSize, points, iterations);

    for (size_t i = 0; i < 2 * points; i += 2) {
      // Normal Multiplication
      struct timespec start_nmul;
      clock_gettime(CLOCK_MONOTONIC, &start_nmul);

      for (size_t j = 0; j < iterations; j++) {
          additionBignum(&tests[i], tests[i+1]);
      }

      struct timespec end_nmul;
      clock_gettime(CLOCK_MONOTONIC, &end_nmul);
      double time_nmul = end_nmul.tv_sec - start_nmul.tv_sec + 1e-9 * (end_nmul.tv_nsec - start_nmul.tv_nsec);
      printf("Normal addition with size %lu took %f seconds.\n", (i/2) * maxSize/points, time_nmul);
    }
    freeArray(2 * points, tests);

    // SUBTRACTION
    iterations = 200000;
    maxSize = 10000;
    points = 20;
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, maxSize, tests, points);
    printf("Running tests for subtraction with max size of %d, %d points distributed even, %d iterations per calculation\n", maxSize, points, iterations);

    for (size_t i = 0; i < 2 * points; i += 2) {
      // Normal Multiplication
      struct timespec start_nmul;
      clock_gettime(CLOCK_MONOTONIC, &start_nmul);

      for (size_t j = 0; j < iterations; j++) {
          subtractionBignum(&tests[i], tests[i+1]);
      }

      struct timespec end_nmul;
      clock_gettime(CLOCK_MONOTONIC, &end_nmul);
      double time_nmul = end_nmul.tv_sec - start_nmul.tv_sec + 1e-9 * (end_nmul.tv_nsec - start_nmul.tv_nsec);
      printf("Normal subtraction with size %lu took %f seconds.\n", (i/2) * maxSize/points, time_nmul);
    }
    freeArray(2 * points, tests);

    // KARAZUBA 
    iterations = 20;
    maxSize = 10000;
    points = 20;
    printf("%s\n", "Generating test bignums, this may take some time...");
    generate_testset(1234, maxSize, tests, points);
    printf("Running tests for karazuba multiplication with max size of %d, %d points distributed even, %d iterations per calculation\n", maxSize, points, iterations);

    for (size_t i = 0; i < 2 * points; i += 2) {
      // Normal Multiplication
      struct timespec start_nmul;
      clock_gettime(CLOCK_MONOTONIC, &start_nmul);

      for (size_t j = 0; j < iterations; j++) {
          karazubaMultiplication(tests[i], tests[i+1]);
      }

      struct timespec end_nmul;
      clock_gettime(CLOCK_MONOTONIC, &end_nmul);
      double time_nmul = end_nmul.tv_sec - start_nmul.tv_sec + 1e-9 * (end_nmul.tv_nsec - start_nmul.tv_nsec);
      printf("Normal karazuba multiplication with size %lu took %f seconds.\n", (i/2) * maxSize/points, time_nmul);
    }
    freeArray(2 * points, tests);
}
