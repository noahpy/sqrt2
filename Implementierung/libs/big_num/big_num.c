
#include "big_num.h"
#include "../utils/utils.h"
#include <errno.h>
#include <stdbool.h>
#include <stddef.h> // do we need that?
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void printBignum(struct bignum *a) {
  printf("size of a: %zu\n", a->size);
  printf("fracSize of a: %zu\n", a->fracSize);
  for (int i = a->size - 1; i >= 0; i--) {
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
  if (__builtin_uaddl_overflow(a.fracSize, b.fracSize, &newFrac)) {
    perror("Could not calculate new fraction size\n");
    exit(EXIT_FAILURE);
  }
  bignumDigits = allocateDigits(newSize);

  struct bignum result = {
      .size = newSize, .digits = bignumDigits, .fracSize = newFrac};

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

/*Both a and b need to have the same fracSize*/
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

int compareBigNum(struct bignum a, struct bignum b) {
  if (a.size > b.size) {
    return 1;
  } else if (a.size < b.size) {
    return -1;
  }
  size_t index = a.size - 1;
  while (a.digits[index] == b.digits[index]) {
    if (index == 0) {
      return 0;
    }
    index--;
  }
  if (a.digits[index] > b.digits[index]) {
    return 1;
  }
  return -1;
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

struct bignum shiftLeft(struct bignum a, size_t n) {
  size_t blockShifts = n / 32;
  n %= 32;
  size_t newSize = blockShifts;
  uint64_t lastBlock = a.digits[a.size - 1];
  if (lastBlock << n > 4294967295)
    newSize++;
  if (__builtin_uaddl_overflow(newSize, a.size, &newSize)) {
    perror("Could not calculate new size");
    exit(EXIT_FAILURE);
  }
  size_t new_fracSize = a.fracSize ? --a.fracSize : 0;
  struct bignum newBigNum = {allocateDigits(newSize), newSize, new_fracSize};
  // zero all elements
  for (size_t i = 0; i < newSize; i++) {
    newBigNum.digits[i] = 0;
  }
  if (!n) {
    // relocate old elements
    for (size_t i = 0; i < a.size; i++) {
      newBigNum.digits[i + blockShifts] = a.digits[i];
    }
    return newBigNum;
  }
  for (size_t i = a.size - 1; i < a.size; i--) {
    uint64_t tmp = a.digits[i];
    *(uint64_t *)(newBigNum.digits + i + blockShifts) += tmp << n;
  }
  return newBigNum;
}

void shiftLeftInplace(struct bignum *a, size_t n) {
  struct bignum tmp = shiftLeft(*a, n);
  free(a->digits);
  a->digits = tmp.digits;
  a->size = tmp.size;
  a->fracSize = tmp.fracSize;
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

  size_t restShifts = number % 32;
  // if (a->size > blockShifts + 1) {
  for (i = 0; i < a->size - blockShifts - 1; i++) {
    *(a->digits + i) = (uint32_t)(*(uint64_t *)(a->digits + i) >> restShifts);
  }
  *(a->digits + i) = (*(a->digits + i) >> restShifts);
  //}
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
  while (compareBigNum(*b, oneShift) == 1) {
    b->fracSize++;
    a->fracSize++;
    oneShift = shiftLeftConstant(oneShift, 1);
  }
  free(oneShift.digits);

  // load constant 32 / 17 (approx. 1.875)
  struct bignum t2 =
      (struct bignum){.digits = allocateDigits(1), .size = 1, .fracSize = 3};
  t2.digits[0] = 0xf;

  // load constant 48 / 17
  size_t numberBlocks = ((t2.fracSize + b->fracSize) / 32) + 2;
  struct bignum t1 = {.digits = allocateDigits(numberBlocks),
                      .size = numberBlocks,
                      .fracSize = (numberBlocks - 1) * 32};
  for (size_t i = 0; i < t1.size - 1; i++) {
    *(t1.digits + i) = 0xD2D2D2D2;
  }
  *(t1.digits + t1.size - 1) = 2;

  // calculate first approximation: t1 + t2 * b
  struct bignum multt2b = multiplicationBignum(t2, *b);
  shiftRight(&t1, t1.fracSize - multt2b.fracSize);
  t1.fracSize -= (t1.fracSize - multt2b.fracSize);

  subtractionBignum(&t1, multt2b);

  free(multt2b.digits);
  free(t2.digits);

  size_t iterationCounter = 0;
  for (size_t i = fracSize; i > 1; i /= 2) {
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

/* This division only works if a < b*/
void divisionBignum2(struct bignum *a, struct bignum *b, size_t fracSize) {
  b->fracSize = fracSize;
  size_t newSize = fracSize / 32 + (fracSize % 32 != 0);
  // allocate result bignum
  uint32_t *digits = allocateDigits(newSize);
  // zero all elements of result bignum
  for (size_t i = 0; i < newSize; i++) {
    digits[i] = 0;
  }
  // shift a by one to the left
  shiftLeftInplace(a, 1);
  uint32_t mask = 1 << ((fracSize % 32) - 1);
  while (fracSize) {
    // decrement fracSize
    fracSize--;
    // if a >= b
    switch (compareBigNum(*a, *b)) {
    case 1:
      // set the bit in the result bignum
      digits[fracSize / 32] |= mask;
      // subtract b from a
      subtractionBignum(a, *b);
      break;
    case 0:
      // set the bit in the result bignum
      digits[fracSize / 32] |= mask;
      goto end;
    case -1:
      break;
    }
    // shift a by one to the left
    shiftLeftInplace(a, 1);
    // shift mask by one to the right
    mask >>= 1;
    if (!mask) {
      mask = 0x80000000;
    }
  }
end:
  // free a
  free(a->digits);
  a->digits = digits;
  a->size = newSize;
  a->fracSize = b->fracSize;
}
