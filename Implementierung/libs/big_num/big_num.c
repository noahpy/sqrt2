
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

  // Multiply every 32bit block of b with the first of a
  // Add the new (64bit) block to the result
  for (size_t i = 0; i < a.size; i++) {
    uint64_t a64 = (uint64_t) a.digits[i];

    for (size_t j = 0; j < b.size; j++) {
      uint64_t b64 = (uint64_t) b.digits[j];
      uint64_t c64 = a64 * b64;
      
      // If there is an addition overflow, increment the third 32bit block
      if(__builtin_uaddll_overflow(c64, *(uint64_t*)(result.digits+j+i), (uint64_t*)(result.digits+j+i))) {
        __builtin_uaddll_overflow(1, *(uint64_t*)(result.digits+2+j+i), (uint64_t*)(result.digits+2+j+i));
      }
    }
  }

  return result;
}
