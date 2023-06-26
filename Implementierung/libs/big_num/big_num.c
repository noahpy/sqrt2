
#include "big_num.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Multiply two bignums and store the result in a new bignum
// a.size should be >= b.size
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  if (!(bignumDigits = malloc((a.size + b.size) * sizeof(*bignumDigits)))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  struct bignum result = {.size = a.size + b.size, .digits = bignumDigits};

  // Zero all elements
  for (size_t i = 0; i < result.size; i++) {
    bignumDigits[i] = 0;
  }

  // Multiply every 32bit block of b with the first of a
  // Add the new (64bit) block to the result
  for (size_t i = 0; i < a.size; i++) {
    uint64_t a64 = (uint64_t)a.digits[i];

    for (size_t j = 0; j < b.size; j++) {
      uint64_t b64 = (uint64_t)b.digits[j];
      uint64_t c64 = a64 * b64;

      size_t overflowCount = 1;
      // If there is an addition overflow, increment the third 32bit block
      if (__builtin_uaddl_overflow(c64, *(uint64_t *)(result.digits + j + i),
                                   (uint64_t *)(result.digits + j + i))) {
        while (__builtin_uaddl_overflow(
            1, *(uint64_t *)(result.digits + (2 * overflowCount) + j + i),
            (uint64_t *)(result.digits + (2 * overflowCount) + j + i))) {
          overflowCount++;
        }
      }
    }
  }

  return result;
}

void recKarazubaMultiplication(uint64_t *digits, uint32_t *x, uint32_t *y, size_t n,
        size_t offset, size_t origin_off) {
    if (n == 1) {
        digits[offset] = (uint64_t) x[origin_off] * y[origin_off];
    } else {
        size_t left = n / 2;

        // Compute x0 * y0
        recKarazubaMultiplication(digits, x, y, left, offset);
        recKarazubaMultiplication(digits, x, y, n - left, offset + left);
    }
}

struct bignum karazubaMultiplication(struct bignum x, struct bignum y) {
    // Don't expand small bignums < 16 bit to avoid exponential memory usage
    if (x.size == 1 && y.size == 1) {

    }
    uint32_t *bignumDigits = NULL;
    if (!(bignumDigits = malloc((x.size + y.size) * sizeof(*bignumDigits)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    recKarazubaMultiplication(bignumDigits, x.digits, y.digits, x.size, 0);
    return (struct bignum) {x.size + y.size, bignumDigits};
}

struct bignum additionBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  if (!(bignumDigits = malloc((a.size + 1) * sizeof(*bignumDigits)))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  struct bignum result = {.size = a.size + 1, .digits = bignumDigits};

  // Take bignum a into result and zero the rest
  for (size_t i = 0; i < result.size; i++) {
    if (i < a.size) {
      bignumDigits[i] = a.digits[i];
    } else {
      bignumDigits[i] = 0;
    }
  }

  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {
    uint64_t b64 = (uint64_t)b.digits[i];

    // If there is an addition overflow, increment the third 32bit block
    if (__builtin_uaddl_overflow(b64, *(uint64_t *)(result.digits + i),
                                 (uint64_t *)(result.digits + i))) {
      __builtin_uaddl_overflow(1, *(uint64_t *)(result.digits + 2 + i),
                               (uint64_t *)(result.digits + 2 + i));
    }
  }

  return result;
}
