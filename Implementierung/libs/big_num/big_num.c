
#include "../utils/utils.h"
#include <emmintrin.h>
#include <smmintrin.h>
#include <errno.h>
#include <immintrin.h>
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
    if (newSize < 1 || result.digits[newSize] != 0) {
      result.size = newSize + 1;
      break;
    }
  }

  return result;
}

// Multiply two bignums and store the result in a new bignum
// a.size should be >= b.size
struct bignum multiplicationBignumSIMD(struct bignum a, struct bignum b) {
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

  size_t i = 0;

  // SIMD multiplication
  for (; i < a.size && a.size - i >= 2 && b.size >= 2; i += 2) {
    // load 2 32bit blocks from a
    __m128i a128 = _mm_cvtepu32_epi64(_mm_loadl_epi64((__m128i *)(a.digits + i)));
    __m128i a128_reversed = _mm_shuffle_epi32(a128, _MM_SHUFFLE(1, 0, 3, 2));
    size_t j = 0;
    for (; j < b.size && b.size - j >= 2; j += 2) {
      // calculate all 4 products and add them to the result, whilst considering overflows
      __m128i b128 = _mm_cvtepu32_epi64(_mm_loadl_epi64((__m128i *)(b.digits + j)));
      __m128i mul = _mm_mul_epu32(a128, b128);
      size_t overflowCount = 2;
      if (__builtin_uaddl_overflow(_mm_extract_epi64(mul, 0), *(uint64_t *)(result.digits + j + i),
                                   (uint64_t *)(result.digits + j + i))) {
        while ((overflowCount) + j + i < result.size &&
               __builtin_uadd_overflow(
                   1, *(result.digits + (overflowCount) + j + i),
                   (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }
      overflowCount = 4;
      if (__builtin_uaddl_overflow(_mm_extract_epi64(mul, 1), *(uint64_t *)(result.digits + j + i + 2),
                                   (uint64_t *)(result.digits + j + i + 2))) {
        while ((overflowCount) + j + i < result.size &&
               __builtin_uadd_overflow(
                   1, *(result.digits + (overflowCount) + j + i),
                   (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }

      mul = _mm_mul_epu32(a128_reversed, b128);
      overflowCount = 3;
      if (__builtin_uaddl_overflow(_mm_extract_epi64(mul, 0), *(uint64_t *)(result.digits + j + i + 1),
                                   (uint64_t *)(result.digits + j + i + 1))) {
        while ((overflowCount) + j + i < result.size &&
               __builtin_uadd_overflow(
                   1, *(result.digits + (overflowCount) + j + i),
                   (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }
      overflowCount = 3;
      if (__builtin_uaddl_overflow(_mm_extract_epi64(mul, 1), *(uint64_t *)(result.digits + j + i + 1),
                                   (uint64_t *)(result.digits + j + i + 1))) {
        while ((overflowCount) + j + i < result.size &&
               __builtin_uadd_overflow(
                   1, *(result.digits + (overflowCount) + j + i),
                   (result.digits + (overflowCount) + j + i))) {
          overflowCount++;
        }
      }

    }
    // calculate remainder
    if(j == b.size-1){
        uint64_t al = (uint64_t)a.digits[i];
        uint64_t ah = (uint64_t)a.digits[i+1];
        uint64_t b64 = (uint64_t)b.digits[j];
        uint64_t cl = al * b64;
        uint64_t ch = ah * b64;
    
        size_t overflowCount = 2;
        // If there is an addition overflow, increment the third 32bit block
        if (__builtin_uaddl_overflow(cl, *(uint64_t *)(result.digits + j + i),
                                     (uint64_t *)(result.digits + j + i))) {
            while ((overflowCount) + j + i < result.size &&
                 __builtin_uadd_overflow(
                     1, *(result.digits + (overflowCount) + j + i),
                     (result.digits + (overflowCount) + j + i))) {
            overflowCount++;
            }
        }
        overflowCount = 3;
        if (__builtin_uaddl_overflow(ch, *(uint64_t *)(result.digits + j + i + 1),
                                     (uint64_t *)(result.digits + j + i + 1))) {
            while ((overflowCount) + j + i < result.size &&
                 __builtin_uadd_overflow(
                     1, *(result.digits + (overflowCount) + j + i),
                     (result.digits + (overflowCount) + j + i))) {
            overflowCount++;
            }
        }
    }
  }

  // Multiply every 32bit block of b with the first of a
  // Add the new (64bit) block to the result
  for (; i < a.size; i++) {
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
    if (newSize < 1 || result.digits[newSize] != 0) {
      result.size = newSize + 1;
      break;
    }
  }

  return result;
}

void additionBignumSIMD(struct bignum *a, struct bignum b) {
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

  // load constant '1' into all doublewords
  int j = 1;
  __m128i ov = _mm_loadu_si32(&j);
  ov = _mm_shuffle_epi32(ov, _MM_SHUFFLE(0, 0, 0, 0));

  // calculate loopcounter for simd
  size_t i = 0;
  size_t size;
  bool subOverflow = __builtin_usubl_overflow(b.size, 4, &size);
  for (; !subOverflow && i <= size; i += 4) {
    // calculate the sum of doublewords
    __m128i am = _mm_loadu_si128((__m128i *)(a->digits + i));
    __m128i bm = _mm_loadu_si128((__m128i *)(b.digits + i));

    __m128i sum = _mm_add_epi32(am, bm);

    // check in which doublewords there is an overflow
    // check for overflow in each doubleword
    __m128i pow = _mm_set1_epi32(-2147483648);
    __m128i amu = _mm_add_epi32(am, pow);
    __m128i bmu = _mm_add_epi32(bm, pow);
    __m128i sumu = _mm_add_epi32(sum, pow);
    __m128i overflow =
        _mm_or_si128(_mm_cmpgt_epi32(amu, sumu), _mm_cmpgt_epi32(bmu, sumu));

    overflow = _mm_and_si128(overflow, ov);

    // if there is an overflow in the highest block, add the overflow sequential
    __m128i zw = _mm_shuffle_epi32(overflow, _MM_SHUFFLE(0, 0, 0, 3));
    uint32_t lastBlockOverflow;
    _mm_storeu_si32(&lastBlockOverflow, zw);
    if (lastBlockOverflow) {
      size_t overflowCount = 4;
      while ((overflowCount) + i < a->size &&
             __builtin_uadd_overflow(1, *(a->digits + (overflowCount) + i),
                                     (a->digits + (overflowCount) + i))) {
        overflowCount++;
      }
    }

    // add overflow to the result and store variable
    overflow = _mm_slli_si128(overflow, 4);
    sum = _mm_add_epi32(sum, overflow);
    _mm_storeu_si128((__m128i *)(a->digits + i), sum);
  }

  // do the last computations without simd
  for (; i < b.size; i++) {
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

void addVectors(struct bignum *a, struct bignum b) {
  // Add the 32bit blocks of b to the corresponding blocks of a
  for (size_t i = 0; i < b.size; i++) {
    size_t overflowCount = 1;
    // If there is an addition overflow, increment the third 32bit block
    if (i < a->size && __builtin_uadd_overflow(b.digits[i], *(a->digits + i),
                                               (a->digits + i))) {
      while ((overflowCount) + i < a->size &&
             __builtin_uadd_overflow(1, *(a->digits + (overflowCount) + i),
                                     (a->digits + (overflowCount) + i))) {
        overflowCount++;
      }
    }
  }
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
  addVectors(a, b);
}

void subtractionBignumSIMD(struct bignum *a, struct bignum b) {
  // load constant '1' into all doublewords
  int j = 1;
  __m128i ov = _mm_loadu_si32(&j);
  ov = _mm_shuffle_epi32(ov, _MM_SHUFFLE(0, 0, 0, 0));

  // compute loop counter
  size_t i = 0;
  size_t size;
  bool subOverflow = __builtin_usubl_overflow(b.size, 4, &size);
  for (; !subOverflow && i <= size; i += 4) {
    // calculate subtraction
    __m128i am = _mm_loadu_si128((__m128i *)(a->digits + i));
    __m128i bm = _mm_loadu_si128((__m128i *)(b.digits + i));

    __m128i sub = _mm_sub_epi32(am, bm);

    // check for overflow in each doubleword
    __m128i pow = _mm_set1_epi32(-2147483648);
    __m128i amu = _mm_add_epi32(am, pow);
    __m128i subu = _mm_add_epi32(sub, pow);
    __m128i overflow = _mm_cmplt_epi32(amu, subu);

    overflow = _mm_and_si128(overflow, ov);

    // if there is an overflow in the highest block, subtract the overflow
    // sequential
    __m128i zw = _mm_shuffle_epi32(overflow, _MM_SHUFFLE(0, 0, 0, 3));
    uint32_t lastBlockOverflow;
    _mm_storeu_si32(&lastBlockOverflow, zw);
    if (lastBlockOverflow) {
      size_t overflowCount = 4;
      while (overflowCount + i < a->size &&
             __builtin_usub_overflow(*(a->digits + (overflowCount) + i), 1,
                                     (a->digits + (overflowCount) + i))) {
        overflowCount++;
      }
    }

    // subtract the overflows and store result
    overflow = _mm_slli_si128(overflow, 4);
    sub = _mm_sub_epi32(sub, overflow);
    _mm_storeu_si128((__m128i *)(a->digits + i), sub);
  }

  // do the rest without simd
  for (; i < b.size; i++) {
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
  for (int newSize = a->size - 1; newSize >= 0; newSize--) {
    if (a->digits[newSize] != 0) {
      a->size = newSize + 1;
      break;
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
  for (int newSize = a->size - 1; newSize >= 0; newSize--) {
    if (a->digits[newSize] != 0) {
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
  // catch edge case
  if (newBigNum.size == 1) {
    newBigNum.digits[0] = lastBlock << n;
    return newBigNum;
  }
  // shift last block seprarately to avoid unallowed writes
  if (a.size - 1 + blockShifts == newBigNum.size - 1) {
    newBigNum.digits[newBigNum.size - 1] = lastBlock << n;
  } else {
    newBigNum.digits[newBigNum.size - 1] = lastBlock >> (32 - n);
    newBigNum.digits[newBigNum.size - 2] = lastBlock << n;
  }
  // shift remaining elements
  for (size_t i = a.size - 2; i < a.size; i--) {
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
  for (i = 0; i < a->size - blockShifts - 1; i++) {
    *(a->digits + i) = (uint32_t)(*(uint64_t *)(a->digits + i) >> restShifts);
  }
  *(a->digits + i) = (*(a->digits + i) >> restShifts);
}

struct bignum splitAndAdd(struct bignum a, size_t split) {
  if (a.size <= split) {
    // Return a copy of a
    uint32_t *digits = allocateDigits(a.size);
    for (size_t i = 0; i < a.size; i++) {
      digits[i] = a.digits[i];
    }
    return (struct bignum){digits, a.size, 0};
  } else {
    // Copy the least significant bits into a bignum and add a zero, so that
    // result.size >= a.size-split (Important for the addition, the first bignum
    // must be larger or equal)
    uint32_t *digits = allocateDigits(split + 2);
    for (size_t i = 0; i < split; i++) {
      digits[i] = a.digits[i];
    }
    digits[split] = digits[split + 1] = 0;

    struct bignum result = {digits, split + 2, 0};

    // Shift the most significant bits to the right place
    struct bignum most = a;
    most.size -= split;
    most.digits += split;

    // Add the most significant
    addVectors(&result, most);

    // Remove leading zeros
    while (result.size > 1 && result.digits[result.size - 1] == 0) {
      result.size--;
    }

    return result;
  }
}

struct bignum karazubaMultiplication(struct bignum x, struct bignum y) {
  if (x.size == 0 || y.size == 0) {
    // If one of the factors is zero, return zero
    // The NULL pointer can always be freed;
    return (struct bignum){NULL, 1, 0};
  } else if (x.size <= 8 && y.size <= 8) {
      return multiplicationBignum(x, y);
  } else {
    // the maximum result size is x.size + y.size
    uint32_t *digits = allocateDigits(x.size + y.size);

    // Swap if necessary, so that x.size >= y.size
    if (x.size < y.size) {
      struct bignum tmp = x;
      x = y;
      y = tmp;
    }

    // Select the split size by the largest factor and divide into m and most
    // significant bits x = x0 + x1 * 2^m, y = y0 + y1 * 2^m
    size_t m = x.size / 2;

    // Calculate x0y0 and x1y1 recursive
    struct bignum x0 = x;
    x0.size = m;
    struct bignum y0 = y;
    y0.size = y.size < m ? y.size : m;
    struct bignum x0y0 = karazubaMultiplication(x0, y0);

    struct bignum x1y1;
    if (y.size <= m) {
      // x1y1 will be zero
      x1y1 = (struct bignum){NULL, 0, 0};
    } else {
      struct bignum x1 = x;
      x1.size = x.size - m;
      x1.digits += m;
      struct bignum y1 = y;
      y1.size = y.size - m;
      y1.digits += m;
      x1y1 = karazubaMultiplication(x1, y1);
    }

    // Copy the results of x0y0 and x1y1 into digits
    for (size_t i = 0; i < 2 * m; i++) {
      if (i < x0y0.size) {
        digits[i] = x0y0.digits[i];
      } else {
        digits[i] = 0;
      }
    }
    for (size_t i = 0; i + 2 * m < x.size + y.size; i++) {
      if (i < x1y1.size) {
        digits[i + 2 * m] = x1y1.digits[i];
      } else {
        digits[i + 2 * m] = 0;
      }
    }

    // Calculate (x0+x1) * (y0+y1)
    struct bignum x0x1 = splitAndAdd(x, m);
    struct bignum y0y1 = splitAndAdd(y, m);

    struct bignum middle_value = karazubaMultiplication(x0x1, y0y1);

    // Subtract x0y0 and x1y1 from middle value and add the result at 2^m
    subtractionBignum(&middle_value, x0y0);
    subtractionBignum(&middle_value, x1y1);

    struct bignum offset =
        (struct bignum){digits + m, (x.size + y.size) - m, 0};
    addVectors(&offset, middle_value);

    struct bignum result = {digits, x.size + y.size, 0};
    // Remove leading zeros
    while (result.size > 1 && result.digits[result.size - 1] == 0) {
      result.size--;
    }

    // Free heap bignums
    free(x0y0.digits);
    free(x1y1.digits);
    free(x0x1.digits);
    free(y0y1.digits);
    free(middle_value.digits);

    return result;
  }
}

/* This division only works if a < b*/
void divisionBignum(struct bignum *a, struct bignum *b, size_t fracSize,
                     void subtract(struct bignum *, struct bignum)) {
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
      subtract(a, *b);
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
