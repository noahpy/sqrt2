
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

void addUint32_tVector(uint32_t *result, size_t size, uint32_t *a, size_t al, uint32_t *b, size_t bl) {
    // Take bignum a into result and zero the rest
    for (size_t i = 0; i < size; i++) {
        if (i < al) {
            result[i] = a[i];
        } else {
            result[i] = 0;
        }
    }

    // Add the 32bit blocks of b to the corresponding blocks of a
    for (size_t i = 0; i < bl; i++) {
        uint64_t b64 = (uint64_t) b[i];

        // If there is an addition overflow, increment the third 32bit block
        if (__builtin_uaddl_overflow(b64, *(uint64_t *)(result + i),
                                     (uint64_t *)(result + i))) {
            __builtin_uaddl_overflow(1, *(uint64_t *)(result + 2 + i),
                                     (uint64_t *)(result + 2 + i));
        }
    }
}


struct bignum additionBignum(struct bignum a, struct bignum b) {
    uint32_t *bignumDigits = NULL;
    if (!(bignumDigits = malloc((a.size + 1) * sizeof(*bignumDigits)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    struct bignum result = {.size = a.size + 1, .digits = bignumDigits};

    addUint32_tVector(result.digits, result.size, a.digits, a.size, b.digits, b.size);

    return result;
}


void recKarazubaMultiplication(uint64_t *digits, uint32_t *x, uint32_t *y, size_t n,
        size_t offset) {
    if (n == 1) {
        digits[offset] = (uint64_t) x[offset] * y[offset];
    } else {
        size_t left = n / 2;

        uint32_t x0[left+1];
        addUint32_tVector(x0, left+1, x + offset, left, x + left + offset, left);

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


int main() {
    uint32_t first[4], second[4];
    first[0] = 1;
    first[1] = 0xffffffff;
    first[2] = 3;
    first[3] = 4;
    second[0] = 5;
    second[1] = 6;
    second[2] = 7;
    second[3] = 8;
    struct bignum a;
    a = karazubaMultiplication((struct bignum) {4, first}, (struct bignum) {4, second});
    return 0;
}