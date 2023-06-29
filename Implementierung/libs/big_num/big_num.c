
#include "big_num.h"
#include <stddef.h> // do we need that?
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

// Creates a bignum with value n on the heap
struct bignum bignumOfInt(uint32_t n) {
    uint32_t *digit = NULL;
    if (!(digit = malloc(sizeof(uint32_t)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Set the value to n
    *digit = n;

    return (struct bignum) {digit, 1, 0};
}

// Multiply two bignums and store the result in a new bignum
// a.size should be >= b.size
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  size_t newSize;
  if(__builtin_uaddl_overflow(a.size, b.size, &newSize)){
    perror("Could not calculate new size\n");
    exit(EXIT_FAILURE);
  }
  if(__builtin_umull_overflow(newSize, sizeof(*bignumDigits), &newSize)){
    perror("Could not calculate new size\n");
    exit(EXIT_FAILURE);
  }
  if (!(bignumDigits = malloc(newSize))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  struct bignum result = {.size = a.size + b.size, .digits = bignumDigits, .fracSize = a.fracSize + b.fracSize};

  // Zero all elements
  for (size_t i = 0; i < result.size; i++) {
    *(result.digits+i) = 0;
  }

  // Multiply every 32bit block of b with the first of a
  // Add the new (64bit) block to the result
  for (size_t i = 0; i < a.size; i++) {
    uint64_t a64 = (uint64_t)a.digits[i];

    for (size_t j = 0; j < b.size; j++) {
      uint64_t b64 = (uint64_t)b.digits[j];
      uint64_t c64 = a64 * b64;

      size_t overflowCount = 2;
      // If there is an addition overflow, increment the third 32bit block
      if (__builtin_uaddl_overflow(c64, *(uint64_t *)(result.digits + j + i),
                                   (uint64_t *)(result.digits + j + i))) {
        while ((overflowCount) + j + i < result.size && __builtin_uadd_overflow(
            1, *(result.digits + (overflowCount) + j + i),
            (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }
    }
  }

  return result;
}


void additionBignum(struct bignum *a, struct bignum b) {
  size_t newSize;
  a->size++;
  if(__builtin_umull_overflow(a->size, sizeof(*a->digits), &newSize)){
    perror("Could not calculate new size\n");
    exit(EXIT_FAILURE);
  }

  uint32_t *newDigits;  
  if (!(newDigits = realloc(a->digits, newSize))) {
    fprintf(stderr, "Could not allocate memory\n");
    free(a->digits);
    exit(EXIT_FAILURE);
  }

  a->digits = newDigits;
  a->digits[a->size - 1] = 0;
 
  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {
    uint64_t b64 = (uint64_t) b.digits[i];

    size_t overflowCount = 2;
    // If there is an addition overflow, increment the third 32bit block
    if (__builtin_uaddl_overflow(b64, *(uint64_t *)(a->digits + i),
                                 (uint64_t *)(a->digits + i))) {
      while((overflowCount) + i < a->size && __builtin_uadd_overflow(1, *(a->digits + (overflowCount) + i),
                               (a->digits + (overflowCount) + i))){
        overflowCount++;
      }
    }
  }

}


void subtractionBignum(struct bignum *a, struct bignum b) {
  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {

    size_t overflowCount = 1;
    // If there is an subtraction overflow, increment the third 32bit block
    if (__builtin_usub_overflow(*(a->digits + i), *(b.digits + i),
                                 (a->digits + i))) {
      while(__builtin_usub_overflow(*(a->digits + (1 * overflowCount) + i), 1,
                               (a->digits + (1 * overflowCount) + i))) {
        overflowCount++;
      }
    }

  }

}

