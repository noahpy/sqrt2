
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
    perror("Could not calculate new size");
    exit(EXIT_FAILURE);
  }
  if(__builtin_umull_overflow(newSize, sizeof(*bignumDigits), &newSize)){
    perror("Could not calculate new size");
    exit(EXIT_FAILURE);
  }
  if (!(bignumDigits = malloc(newSize))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  struct bignum result = {.size = a.size + b.size, .digits = bignumDigits, .fracSize = a.fracSize + b.fracSize};

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
  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {

    size_t overflowCount = 1;
    // If there is an subtraction overflow, increment the third 32bit block
    if (__builtin_usub_overflow(*(a.digits + i), *(b.digits + i),
                                 (a.digits + i))) {
      while(__builtin_usub_overflow(*(a.digits + (1 * overflowCount) + i), 1,
                               (a.digits + (1 * overflowCount) + i))) {
        overflowCount++;
      }
    }

  }

  return a;
}

int compareHighestDigits(struct bignum a, struct bignum b) {
  size_t highestDigitA = a.size - 1;
  while (a.digits[highestDigitA] == 0) highestDigitA--;
  if (highestDigitA == b.size - 1) {
    if (a.digits[highestDigitA] == b.digits[b.size - 1]) return 0;
    if (a.digits[highestDigitA] > b.digits[b.size - 1]) return 1;
    return -1;
  } else if (highestDigitA > b.size - 1) {
    return 1;
  } else {
    return -1;
  }
}

// Compact version of shiftLeft, where the constant "2" can be shifted
struct bignum shiftLeftConstant(struct bignum a, size_t number) {
  // Calculate how much new 32Bit blocks are needed
  int numberNewBlocks = number / 32;
  int blockWithOne = a.size - 1;
  uint32_t savedBlock = a.digits[a.size - 1]; 
  bool restNeedsBlock = false;

  if (savedBlock << (number % 32) == 0) {
    numberNewBlocks++;
    restNeedsBlock = true;
  }

  uint32_t *newDigits = NULL;
  if (numberNewBlocks > 0) {
    // printf("%o\n", blockWithOne);
    free(a.digits);
    // TODO: add overflow check
    if (!(newDigits = malloc(sizeof(*newDigits) * (a.size + numberNewBlocks)))) {
      fprintf(stderr, "Could not allocate memory\n");
      exit(EXIT_FAILURE);
    }
    a.size = a.size + numberNewBlocks;
    a.digits = newDigits;
    for (size_t i = 0; i < a.size; i++) {
      a.digits[i] = 0;
    }
    a.digits[blockWithOne] = savedBlock;
  }
  a.fracSize = a.fracSize + number;

  for (int i = blockWithOne; i < blockWithOne + numberNewBlocks + 1; i++) {
    if (number >= 32) {
      *(a.digits+i+1) = *(a.digits+i);
      *(a.digits+i) = 0;
      number -= 32;
    } else {
      if (restNeedsBlock) {
        *(uint64_t*) (a.digits+i) = *(uint64_t*) (a.digits+i) << number;
      } else {
        *(a.digits + i) = *(a.digits + i) << number;
      }
      break;
    }
  }
  
  return a;
}
