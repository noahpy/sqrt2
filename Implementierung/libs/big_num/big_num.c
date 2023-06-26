
#include "big_num.h"
#include <stddef.h> // do we need that?
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Creates a bignum with value n on the heap TODO: Move to bignum
struct bignum bignumOfInt(uint32_t n) {
    uint32_t *digit = NULL;
    if (!(digit = malloc(sizeof(uint32_t)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Set the value to n
    *digit = n;

    return (struct bignum) {1, digit};
}

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

/* struct bignum additionBignum(struct bignum a, struct bignum b) { */
/*   uint32_t *bignumDigits = NULL; */
/*   if (!(bignumDigits = malloc((a.size + 1) * sizeof(*bignumDigits)))) { */
/*     fprintf(stderr, "Could not allocate memory\n"); */
/*     exit(EXIT_FAILURE); */
/*   } */
/*  */
/*   struct bignum result = {.size = a.size + 1, .digits = bignumDigits}; */
/*  */
/*   // Take bignum 'a' into result and zero the rest */
/*   for (size_t i = 0; i < result.size; i++) { */
/*     if (i < a.size) { */
/*       bignumDigits[i] = a.digits[i]; */
/*     } else { */
/*       bignumDigits[i] = 0; */
/*     } */
/*   } */
/*  */
/*   // Add the 32bit blocks of b to the corresponding blocks of a */
/*   for (size_t i = 0; i < b.size; i++) { */
/*     uint64_t b64 = (uint64_t) b.digits[i]; */
/*  */
/*     size_t overflowCount = 1; */
/*     // If there is an addition overflow, increment the third 32bit block */
/*     if (__builtin_uaddl_overflow(b64, *(uint64_t *)(result.digits + i), */
/*                                  (uint64_t *)(result.digits + i))) { */
/*       while(__builtin_uaddl_overflow(1, *(uint64_t *)(result.digits + (2*overflowCount) + i), */
/*                                (uint64_t *)(result.digits + (2*overflowCount) + i))){ */
/*         overflowCount++; */
/*       } */
/*     } */
/*   } */
/*  */
/*   return result; */
/* } */

struct bignum additionBignum(struct bignum a, struct bignum b) {
  size_t newSize;
  if(__builtin_umull_overflow(a.size, sizeof(*a.digits), &newSize)){
    perror("Could not calculate new size");
    exit(EXIT_FAILURE);
  }
  void* result = realloc(a.digits, newSize);
  (void) result;

  a.size++;

  a.digits[a.size - 1] = 0;
 
  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {
    uint64_t b64 = (uint64_t) b.digits[i];

    size_t overflowCount = 1;
    // If there is an addition overflow, increment the third 32bit block
    if (__builtin_uaddl_overflow(b64, *(uint64_t *)(a.digits + i),
                                 (uint64_t *)(a.digits + i))) {
      while(__builtin_uaddl_overflow(1, *(uint64_t *)(a.digits + (2*overflowCount) + i),
                               (uint64_t *)(a.digits + (2*overflowCount) + i))){
        overflowCount++;
      }
    }
  }

  return a;
}


struct bignum subtractionBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  if (!(bignumDigits = malloc((a.size) * sizeof(*bignumDigits)))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  struct bignum result = {.size = a.size , .digits = bignumDigits};

  // Take bignum 'a' into result and zero the rest
  for (size_t i = 0; i < result.size; i++) {
    bignumDigits[i] = a.digits[i];
  }

  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {

    size_t overflowCount = 1;
    // If there is an subtraction overflow, increment the third 32bit block
    if (__builtin_usub_overflow(*(result.digits + i), *(b.digits + i),
                                 (result.digits + i))) {
      while(__builtin_usub_overflow(*(result.digits + (1 * overflowCount) + i), 1,
                               (result.digits + (1 * overflowCount) + i))) {
        overflowCount++;
      }
    }

  }

  return result;
}
