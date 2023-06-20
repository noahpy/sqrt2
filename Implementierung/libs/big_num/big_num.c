
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "big_num.h"

// Multiply two bignums and store the result in a new bignum
// a.size should be >= b.size
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  if(!(bignumDigits = (uint32_t*) malloc((a.size+b.size) * sizeof(*bignumDigits)))) {
    fprintf(stderr, "Could not allocate memory");
    exit(EXIT_FAILURE);
  }

  struct bignum result = { .size = a.size+b.size , .digits = bignumDigits };

  // Zero all elements
  for (size_t i = 0; i < result.size; i++) {
    bignumDigits[i] = 0;
  }

  // Multiply every 32bit block of b with the first of a
  // Add the new (64bit) block to the result
  for (size_t i = 0; i < a.size; i++) {
    uint64_t a64 = (uint64_t) a.digits[i];

    for (size_t j = 0; j < b.size; j++) {
      uint64_t b64 = (uint64_t) b.digits[j];
      uint64_t c64 = a64 * b64;
      
      size_t overflowCount = 1;
      // If there is an addition overflow, increment the third 32bit block
      if(__builtin_uaddl_overflow(c64, *(uint64_t*)(result.digits+j+i), (uint64_t*)(result.digits+j+i))) {
        while(__builtin_uaddl_overflow(1, *(uint64_t*)(result.digits+(2*overflowCount)+j+i), (uint64_t*)(result.digits+(2*overflowCount)+j+i))){
            overflowCount++;
        }
      }
    }
  }

  return result;
}

struct bignum additionBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  if(!(bignumDigits = (uint32_t*) malloc((a.size+1) * sizeof(*bignumDigits)))) {
    fprintf(stderr, "Could not allocate memory");
    exit(EXIT_FAILURE);
  }

  struct bignum result = { .size = a.size+1 , .digits = bignumDigits };

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
    uint64_t b64 = (uint64_t) b.digits[i];

    // If there is an addition overflow, increment the third 32bit block
    if(__builtin_uaddl_overflow(b64, *(uint64_t*)(result.digits+i), (uint64_t*)(result.digits+i))) {
      __builtin_uaddl_overflow(1, *(uint64_t*)(result.digits+2+i), (uint64_t*)(result.digits+2+i));
    }
  }

  return result;
}
