
#include "big_num.h"
#include <errno.h>
#include <stdbool.h>
#include <stddef.h> // do we need that?
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/utils.h"

void printBignum(struct bignum *a) {
    printf("size of a: %zu\n", a->size);
    printf("fracSize of a: %zu\n", a->fracSize);
    for (int i = a->size-1; i >= 0; i--) {
        printf("digits[%d]: %o\n", i, *(a->digits + i));
    }
}

uint32_t *allocateDigits(size_t number) {
  uint32_t *bignumDigits;
  size_t newSize;
  if (__builtin_umull_overflow(number, sizeof(*bignumDigits), &newSize)) {
    perror("Could not calculate new size\n");
    exit(EXIT_FAILURE);
  }
  if (!(bignumDigits = malloc(newSize))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }
  return bignumDigits;
}

// Creates a bignum with value n on the heap
struct bignum bignumOfInt(uint32_t n) {
  uint32_t *digit = NULL;
  if (!(digit = malloc(sizeof(uint32_t)))) {
    fprintf(stderr, "Could not allocate memory\n");
    exit(EXIT_FAILURE);
  }

  // Set the value to n
  *digit = n;

  return (struct bignum){digit, 1, 0};
}

// Multiply two bignums and store the result in a new bignum
// a.size should be >= b.size
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
  uint32_t *bignumDigits = NULL;
  size_t newSize;
  if (__builtin_uaddl_overflow(a.size, b.size, &newSize)) {
    perror("Could not calculate new size\n");
    exit(EXIT_FAILURE);
  }
  size_t newFrac;
  if(__builtin_uaddl_overflow(a.fracSize, b.fracSize, &newFrac)){
    perror("Could not calculate new fraction size\n");
    exit(EXIT_FAILURE);
  }
  bignumDigits = allocateDigits(newSize);

  struct bignum result = {.size = newSize,
                          .digits = bignumDigits,
                          .fracSize = newFrac};

  // Zero all elements
  for (size_t i = 0; i < result.size; i++) {
    *(result.digits + i) = 0;
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
        while ((overflowCount) + j + i < result.size &&
               __builtin_uadd_overflow(
                   1, *(result.digits + (overflowCount) + j + i),
                   (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }
    }
  }

  // Remove leading zeros
  for (int newSize = result.size - 1; newSize >= -1; newSize--) {
    if (newSize < 0 || result.digits[newSize] != 0) {
      result.size = newSize + 1;
      break;
    }
  }

  return result;
}

void additionBignum(struct bignum *a, struct bignum b) {
  size_t newSize;
  uint32_t overflowCheck;
  uint32_t *newDigits;

  // check if extra space is needed
  if (__builtin_uadd_overflow(a->digits[a->size - 1], b.digits[b.size - 1],
                              &overflowCheck)) {
    a->size++;
    if (__builtin_umull_overflow(a->size, sizeof(*a->digits), &newSize)) {
      perror("Could not calculate new size\n");
      exit(EXIT_FAILURE);
    }

    if (!(newDigits = realloc(a->digits, newSize))) {
      fprintf(stderr, "Could not allocate memory\n");
      free(a->digits);
      exit(EXIT_FAILURE);
    }
    a->digits = newDigits;
    a->digits[a->size - 1] = 0;
  }

  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {
    uint64_t b64 = (uint64_t)b.digits[i];

    size_t overflowCount = 2;
    // If there is an addition overflow, increment the third 32bit block
    if (__builtin_uaddl_overflow(b64, *(uint64_t *)(a->digits + i),
                                 (uint64_t *)(a->digits + i))) {
      while ((overflowCount) + i < a->size &&
             __builtin_uadd_overflow(1, *(a->digits + (overflowCount) + i),
                                     (a->digits + (overflowCount) + i))) {
        overflowCount++;
      }
    }
  }
}

void subtractionBignum(struct bignum *a, struct bignum b) {

  // Subtract the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {

    size_t overflowCount = 1;
    // If there is an subtraction overflow, decrement the third 32bit block
    if (__builtin_usub_overflow(*(a->digits + i), *(b.digits + i),
                                (a->digits + i))) {
      while (overflowCount + i < a->size &&
             __builtin_usub_overflow(*(a->digits + (overflowCount) + i), 1,
                                     (a->digits + (overflowCount) + i))) {
        overflowCount++;
      }
    }
  }

  // Remove leading zeros
  for (int newSize = a->size - 1; newSize >= -1; newSize--) {
    if (newSize < 0 || a->digits[newSize] != 0) {
      a->size = newSize + 1;
      break;
    }
  }
}

int compareHighestDigits(struct bignum a, struct bignum b) {
  size_t highestDigitA = a.size - 1;
  while (a.digits[highestDigitA] == 0)
    highestDigitA--;
  if (highestDigitA == b.size - 1) {
    if (a.digits[highestDigitA] == b.digits[b.size - 1])
      return 0;
    if (a.digits[highestDigitA] > b.digits[b.size - 1])
      return 1;
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
    free(a.digits);
    size_t newSize;
    if (__builtin_uaddl_overflow(a.size, numberNewBlocks, &newSize)) {
      perror("Could not calculate new size\n");
      exit(EXIT_FAILURE);
    }
    newDigits = allocateDigits(newSize);
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
      *(a.digits + i + 1) = *(a.digits + i);
      *(a.digits + i) = 0;
      number -= 32;
    } else {
      if (restNeedsBlock) {
        *(uint64_t *)(a.digits + i) = *(uint64_t *)(a.digits + i) << number;
      } else {
        *(a.digits + i) = *(a.digits + i) << number;
      }
      break;
    }
  }

  return a;
}

void shiftRight(struct bignum *a, size_t number) {
  size_t blockShifts = number / 32;
  size_t i = 0;
  if (a->size > blockShifts) {
    for (; i < a->size - blockShifts; i++) {
      a->digits[i] = a->digits[i + blockShifts];
    }
  }

  for (; i < a->size; i++) {
    a->digits[i] = 0;
  }
  //a->size = i + 1;

  size_t restShifts = number % 32;
  if (a->size > blockShifts + 1) {
    for (i = 0; i < a->size - blockShifts - 1; i++) {
      *(a->digits + i) = (uint32_t)(*(uint64_t *)(a->digits + i) >> restShifts);
    }
    *(a->digits + i) = (*(a->digits + i) >> restShifts);
  }
}

// Calculate a/b with newton-raphson: result is in *a
void divisionBignum(struct bignum *a, struct bignum *b, size_t fracSize) {

  // Treat 1/2 as a edge case
  if (a->digits[0] == 1 && b->digits[0] == 2) {
    free(a->digits);
    *a = shiftLeftConstant(bignumOfInt(1), fracSize - 1);
    return;
  }

  a->fracSize = 1;
  b->fracSize = 1;
  struct bignum oneShift = shiftLeftConstant(bignumOfInt(1), b->fracSize);

  // Calculates b * 0.5 until b < 1
  while (compareHighestDigits(*b, oneShift) == 1) {
    b->fracSize++;
    a->fracSize++;
    oneShift = shiftLeftConstant(oneShift, 1);
  }
  free(oneShift.digits);

  // load constant 32 / 17
  struct bignum t2 = (struct bignum){.digits = allocateDigits(5), .size = 5, .fracSize = 128};
  t2.digits[0] = 0xe1e1e1e1;
  t2.digits[1] = 0xe1e1e1e1;
  t2.digits[2] = 0xe1e1e1e1;
  t2.digits[3] = 0xe1e1e1e1;
  t2.digits[4] = 0x1;

  // load constant 48 / 17
  size_t numberBlocks = ((t2.fracSize + b->fracSize) / 32) + 2;
  struct bignum t1 = {.digits = allocateDigits(numberBlocks), .size = numberBlocks, .fracSize = (numberBlocks - 1) * 32};
  for (size_t i = 0; i < t1.size - 1; i++) {
    *(t1.digits + i) = 0xD2D2D2D2;
  }
  *(t1.digits + t1.size - 1) = 2;

  // calculate first approximation: t1 + t2 * b
  struct bignum multt2b = multiplicationBignum(t2, *b);
  shiftRight(&t1, t1.fracSize - multt2b.fracSize);
  t1.fracSize -= (t1.fracSize - multt2b.fracSize);

  subtractionBignum(&t1, multt2b);

  print_bignum_dec(&t1, multiplicationBignum, false); 

  free(multt2b.digits);
  free(t2.digits);

  size_t iterationCounter = 2;
  for (size_t i = fracSize; i >= 32; i /= 2) {
    iterationCounter++;
  }

  // iterations to approximate the values with: t1 = t1 * (2 - b * t1)
  for (size_t i = 0; i < iterationCounter; i++) {
    struct bignum t1t = multiplicationBignum(t1, *b);
    struct bignum two = shiftLeftConstant(bignumOfInt(2), t1t.fracSize);
    subtractionBignum(&two, t1t);

    free(t1t.digits);
    t1t = multiplicationBignum(t1, two);

    free(t1.digits);
    free(two.digits);

    t1 = t1t;
  }

  // multiply a with the approximated value to get a/b
  struct bignum at = multiplicationBignum(*a, t1);
  free(a->digits);
  free(t1.digits);
  *a = at;

  // Shift bignum right, until we have our desired number of fraction size
  shiftRight(a, a->fracSize - fracSize);
  a->fracSize -= a->fracSize - fracSize;

  // Remove leading zeros
  for (int newSize = a->size - 1; newSize >= 0; newSize--) {
    if (a->digits[newSize] != 0) {
      a->size = newSize + 1;
      break;
    }
  }
}
