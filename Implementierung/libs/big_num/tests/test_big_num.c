
#include "../../utils/utils.h"
#include "../big_num.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int test_cases = 0;
int test_passed = 0;

struct bignum a;
struct bignum b;
struct bignum expected;

void resetBignums(size_t as, size_t bs, size_t expecteds) {
  a = (struct bignum){.size = as, .digits = malloc(16 * sizeof(uint32_t))};
  b = (struct bignum){.size = bs, .digits = malloc(16 * sizeof(uint32_t))};
  expected = (struct bignum){.size = expecteds,
                             .digits = malloc(16 * sizeof(uint32_t))};

  for (size_t i = 0; i < 16; i++) {
    a.digits[i] = 0;
    b.digits[i] = 0;
    expected.digits[i] = 0;
  }
}

void test_multiplication(struct bignum a, struct bignum b,
                         struct bignum expected,
                         struct bignum multiply(struct bignum, struct bignum)) {
  test_cases++;
  printf("Test: multiplicationBignum(0x");
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x", a.digits[0]);
  printf(", 0x");
  for (size_t j = b.size - 1; j > 0; j--) {
    printf("%08x_", b.digits[j]);
  }
  printf("%08x)", b.digits[0]);
  struct bignum result = multiply(a, b);
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], result.digits[i]);
        free(result.digits);
        free(a.digits);
        free(b.digits);
        free(expected.digits);
        return;
      }
    }
    test_passed++;
    printf(" passed\n");
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size,
           result.size);
  }
  free(result.digits);
  free(a.digits);
  free(b.digits);
  free(expected.digits);
}

void test_addition(struct bignum a, struct bignum b, struct bignum expected,
                   bool simd) {
  test_cases++;
  if (simd) {
    printf("Test: additionBignumSIMD(0x");
  } else {
    printf("Test: additionBignum(0x");
  }
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x", a.digits[0]);
  printf(", 0x");
  for (size_t j = b.size - 1; j > 0; j--) {
    printf("%08x_", b.digits[j]);
  }
  printf("%08x)", b.digits[0]);
  if (simd) {
    additionBignumSIMD(&a, b);
  } else {
    additionBignum(&a, b);
  }
  if (a.size == expected.size) {
    for (size_t i = 0; i < a.size; i++) {
      if (a.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], a.digits[i]);
        return;
      }
    }
    test_passed++;
    printf(" passed\n");
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size, a.size);
  }
  free(expected.digits);
  free(a.digits);
  free(b.digits);
}

void test_subtraction(struct bignum a, struct bignum b, struct bignum expected,
                      bool simd) {
  test_cases++;
  if (simd) {
    printf("Test: subtractionBignumSIMD(0x");
  } else {
    printf("Test: subtractionBignum(0x");
  }
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x", a.digits[0]);
  printf(", 0x");
  for (size_t j = b.size - 1; j > 0; j--) {
    printf("%08x_", b.digits[j]);
  }
  printf("%08x)", b.digits[0]);
  if (simd) {
    subtractionBignumSIMD(&a, b);
  } else {
    subtractionBignum(&a, b);
  }
  struct bignum result = a;
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], result.digits[i]);
        free(a.digits);
        free(b.digits);
        free(expected.digits);
        return;
      }
    }
    test_passed++;
    printf(" passed\n");
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size,
           result.size);
  }
  free(a.digits);
  free(b.digits);
  free(expected.digits);
}

void test_division(struct bignum a, struct bignum b, size_t number,
                   struct bignum expected) {
  test_cases++;
  printf("Test: divisionBignum(0x");
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x", a.digits[0]);
  printf(", 0x");
  for (size_t j = b.size - 1; j > 0; j--) {
    printf("%08x_", b.digits[j]);
  }
  printf("%08x, with a fraction size of %zu)", b.digits[0], number);
  divisionBignum(&a, &b, number, subtractionBignum);
  struct bignum result = a;
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], result.digits[i]);
        free(a.digits);
        free(b.digits);
        free(expected.digits);
        return;
      }
    }
    printf(" passed\n");
    test_passed++;
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size,
           result.size);
  }
  free(a.digits);
  free(b.digits);
  free(expected.digits);
}

void test_shift_left(struct bignum a, size_t number, struct bignum expected) {
  test_cases++;
  printf("Test: shiftLeft(0x");
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x, ", a.digits[0]);
  printf("%zu)", number);
  struct bignum result = shiftLeft(a, number);
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], result.digits[i]);
        free(result.digits);
        free(expected.digits);
        return;
      }
    }
    printf(" passed\n");
    test_passed++;
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size,
           result.size);
  }
  free(a.digits);
  free(result.digits);
  free(expected.digits);
}

void test_shift_right(struct bignum a, size_t number, struct bignum expected) {
  test_cases++;
  printf("Test: shiftRight(0x");
  for (size_t j = a.size - 1; j > 0; j--) {
    printf("%08x_", a.digits[j]);
  }
  printf("%08x, ", a.digits[0]);
  printf("%zu)", number);
  shiftRight(&a, number);
  if (a.size == expected.size) {
    for (size_t i = 0; i < a.size; i++) {
      if (a.digits[i] != expected.digits[i]) {
        printf(" failed! digits[%zu] should be 0x%08x, but was 0x%08x\n", i,
               expected.digits[i], a.digits[i]);
        free(a.digits);
        free(expected.digits);
        return;
      }
    }
    printf(" passed\n");
    test_passed++;
  } else {
    printf(" failed! size should be %zu, but was %zu\n", expected.size, a.size);
  }
  free(a.digits);
  free(expected.digits);
}

int main(void) {

  // TESTS FOR NORMAL MULTIPLICATION

  resetBignums(1, 1, 1);
  // 4 * 8 = 32
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(1, 1, 1);
  // 4 * 8 = 32, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(1, 1, 1);
  // 9 * 1 = 9
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 9;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(1, 1, 1);
  // 1 * 9 = 9
  *a.digits = 1;
  *b.digits = 9;
  *expected.digits = 9;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(1, 1, 1);
  // 754 * 754 = 568516
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 568516;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(2, 1, 2);
  // 4294967296 * 1 = 4294967296
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0;
  *(expected.digits + 1) = 1;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(2, 2, 4);
  // 0x13214ab1_13214ab1 * 0x13214ab1_13214ab1 =
  // 0x16df56a_a048b936_3c47922c_9d6cce61
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x9d6cce61;
  *(expected.digits + 1) = 0x3c47922c;
  *(expected.digits + 2) = 0xa048b936;
  *(expected.digits + 3) = 0x16df56a;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(3, 3, 6);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0x00000000;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(2, 1, 3);
  // 0x5234ad_94724362 * 0x3abf = 0x12_dd44a123_a4847a1e
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0xa4847a1e;
  *(expected.digits + 1) = 0xdd44a123;
  *(expected.digits + 2) = 0x12;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(3, 2, 5);
  // 0xadf_ebcfefef_beaaa420 * 0xadcbef_afafef69 =
  // 0x7_61fc1a06_bad8d17d_fc98d30e_95173120
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x95173120;
  *(expected.digits + 1) = 0xfc98d30e;
  *(expected.digits + 2) = 0xbad8d17d;
  *(expected.digits + 3) = 0x61fc1a06;
  *(expected.digits + 4) = 0x7;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(b, a, expected, multiplicationBignum);

  resetBignums(2, 4, 6);
  // test one-time overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0x0;
  *(expected.digits + 5) = 0x1;
  test_multiplication(b, a, expected, multiplicationBignum);

  resetBignums(2, 5, 7);
  // test multiple overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0x0;
  *(expected.digits + 6) = 0x1;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(2, 11, 13);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 1;
  *(a.digits + 1) = 1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *(b.digits + 6) = 0xffffffff;
  *(b.digits + 7) = 0xffffffff;
  *(b.digits + 8) = 0xffffffff;
  *(b.digits + 9) = 0xffffffff;
  *(b.digits + 10) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xffffffff;
  *(expected.digits + 7) = 0xffffffff;
  *(expected.digits + 8) = 0xffffffff;
  *(expected.digits + 9) = 0xffffffff;
  *(expected.digits + 10) = 0xffffffff;
  *(expected.digits + 11) = 0;
  *(expected.digits + 12) = 1;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(3, 6, 9);
  // 0x00000001_00000001_00000001 *
  // 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000001_00000000_ffffffff_ffffffff_ffffffff_fffffffe_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *(a.digits + 2) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0x0;
  *(expected.digits + 7) = 0x1;
  *(expected.digits + 8) = 0x1;
  test_multiplication(a, b, expected, multiplicationBignum);

  resetBignums(5, 1, 5);
  *a.digits = 0xe1e1e1e1;
  *(a.digits + 1) = 0xe1e1e1e1;
  *(a.digits + 2) = 0xe1e1e1e1;
  *(a.digits + 3) = 0xe1e1e1e1;
  *(a.digits + 4) = 1;
  *b.digits = 5;
  *expected.digits = 0x69696965;
  *(expected.digits + 1) = 0x69696969;
  *(expected.digits + 2) = 0x69696969;
  *(expected.digits + 3) = 0x69696969;
  *(expected.digits + 4) = 0x9;
  test_multiplication(a, b, expected, multiplicationBignum);

  // TESTS FOR KARAZUBA MULTIPLICATION

  resetBignums(1, 1, 1);
  // 4 * 8 = 32
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(1, 1, 1);
  // 4 * 8 = 32, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(1, 1, 1);
  // 9 * 1 = 9
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 9;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(1, 1, 1);
  // 1 * 9 = 9
  *a.digits = 1;
  *b.digits = 9;
  *expected.digits = 9;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(1, 1, 1);
  // 754 * 754 = 568516
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 568516;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(2, 1, 2);
  // 4294967296 * 1 = 4294967296
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0;
  *(expected.digits + 1) = 1;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(2, 2, 4);
  // 0x13214ab1_13214ab1 * 0x13214ab1_13214ab1 =
  // 0x16df56a_a048b936_3c47922c_9d6cce61
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x9d6cce61;
  *(expected.digits + 1) = 0x3c47922c;
  *(expected.digits + 2) = 0xa048b936;
  *(expected.digits + 3) = 0x16df56a;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(3, 3, 6);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0x00000000;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(2, 1, 3);
  // 0x5234ad_94724362 * 0x3abf = 0x12_dd44a123_a4847a1e
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0xa4847a1e;
  *(expected.digits + 1) = 0xdd44a123;
  *(expected.digits + 2) = 0x12;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(3, 2, 5);
  // 0xadf_ebcfefef_beaaa420 * 0xadcbef_afafef69 =
  // 0x7_61fc1a06_bad8d17d_fc98d30e_95173120
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x95173120;
  *(expected.digits + 1) = 0xfc98d30e;
  *(expected.digits + 2) = 0xbad8d17d;
  *(expected.digits + 3) = 0x61fc1a06;
  *(expected.digits + 4) = 0x7;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(b, a, expected, karazubaMultiplication);

  resetBignums(2, 4, 6);
  // test one-time overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0x0;
  *(expected.digits + 5) = 0x1;
  test_multiplication(b, a, expected, karazubaMultiplication);

  resetBignums(2, 5, 7);
  // test multiple overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0x0;
  *(expected.digits + 6) = 0x1;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(2, 11, 13);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 1;
  *(a.digits + 1) = 1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *(b.digits + 6) = 0xffffffff;
  *(b.digits + 7) = 0xffffffff;
  *(b.digits + 8) = 0xffffffff;
  *(b.digits + 9) = 0xffffffff;
  *(b.digits + 10) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xffffffff;
  *(expected.digits + 7) = 0xffffffff;
  *(expected.digits + 8) = 0xffffffff;
  *(expected.digits + 9) = 0xffffffff;
  *(expected.digits + 10) = 0xffffffff;
  *(expected.digits + 11) = 0;
  *(expected.digits + 12) = 1;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(3, 6, 9);
  // 0x00000001_00000001_00000001 *
  // 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000001_00000000_ffffffff_ffffffff_ffffffff_fffffffe_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *(a.digits + 2) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0x0;
  *(expected.digits + 7) = 0x1;
  *(expected.digits + 8) = 0x1;
  test_multiplication(a, b, expected, karazubaMultiplication);

  resetBignums(5, 1, 5);
  *a.digits = 0xe1e1e1e1;
  *(a.digits + 1) = 0xe1e1e1e1;
  *(a.digits + 2) = 0xe1e1e1e1;
  *(a.digits + 3) = 0xe1e1e1e1;
  *(a.digits + 4) = 1;
  *b.digits = 5;
  *expected.digits = 0x69696965;
  *(expected.digits + 1) = 0x69696969;
  *(expected.digits + 2) = 0x69696969;
  *(expected.digits + 3) = 0x69696969;
  *(expected.digits + 4) = 0x9;
  test_multiplication(a, b, expected, karazubaMultiplication);

  // TEST SIMD MULTIPLIKATION

  resetBignums(1, 1, 1);
  // 4 * 8 = 32
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(1, 1, 1);
  // 4 * 8 = 32, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(1, 1, 1);
  // 9 * 1 = 9
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 9;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(1, 1, 1);
  // 1 * 9 = 9
  *a.digits = 1;
  *b.digits = 9;
  *expected.digits = 9;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(1, 1, 1);
  // 754 * 754 = 568516
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 568516;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(2, 1, 2);
  // 4294967296 * 1 = 4294967296
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0;
  *(expected.digits + 1) = 1;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(2, 2, 4);
  // 0x13214ab1_13214ab1 * 0x13214ab1_13214ab1 =
  // 0x16df56a_a048b936_3c47922c_9d6cce61
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x9d6cce61;
  *(expected.digits + 1) = 0x3c47922c;
  *(expected.digits + 2) = 0xa048b936;
  *(expected.digits + 3) = 0x16df56a;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(3, 3, 6);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0x00000000;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(2, 1, 3);
  // 0x5234ad_94724362 * 0x3abf = 0x12_dd44a123_a4847a1e
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0xa4847a1e;
  *(expected.digits + 1) = 0xdd44a123;
  *(expected.digits + 2) = 0x12;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(3, 2, 5);
  // 0xadf_ebcfefef_beaaa420 * 0xadcbef_afafef69 =
  // 0x7_61fc1a06_bad8d17d_fc98d30e_95173120
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x95173120;
  *(expected.digits + 1) = 0xfc98d30e;
  *(expected.digits + 2) = 0xbad8d17d;
  *(expected.digits + 3) = 0x61fc1a06;
  *(expected.digits + 4) = 0x7;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(3, 2, 5);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x00000000;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xffffffff;
  test_multiplication(b, a, expected, multiplicationBignumSIMD);

  resetBignums(2, 4, 6);
  // test one-time overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0x0;
  *(expected.digits + 5) = 0x1;
  test_multiplication(b, a, expected, multiplicationBignumSIMD);

  resetBignums(2, 5, 7);
  // test multiple overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_ffffffff_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0x0;
  *(expected.digits + 6) = 0x1;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(2, 11, 13);
  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a.digits = 1;
  *(a.digits + 1) = 1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *(b.digits + 6) = 0xffffffff;
  *(b.digits + 7) = 0xffffffff;
  *(b.digits + 8) = 0xffffffff;
  *(b.digits + 9) = 0xffffffff;
  *(b.digits + 10) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xffffffff;
  *(expected.digits + 7) = 0xffffffff;
  *(expected.digits + 8) = 0xffffffff;
  *(expected.digits + 9) = 0xffffffff;
  *(expected.digits + 10) = 0xffffffff;
  *(expected.digits + 11) = 0;
  *(expected.digits + 12) = 1;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(3, 6, 9);
  // 0x00000001_00000001_00000001 *
  // 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000001_00000000_ffffffff_ffffffff_ffffffff_fffffffe_fffffffe_ffffffff
  *a.digits = 0x1;
  *(a.digits + 1) = 0x1;
  *(a.digits + 2) = 0x1;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *(b.digits + 5) = 0xffffffff;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0x0;
  *(expected.digits + 7) = 0x1;
  *(expected.digits + 8) = 0x1;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  resetBignums(5, 1, 5);
  *a.digits = 0xe1e1e1e1;
  *(a.digits + 1) = 0xe1e1e1e1;
  *(a.digits + 2) = 0xe1e1e1e1;
  *(a.digits + 3) = 0xe1e1e1e1;
  *(a.digits + 4) = 1;
  *b.digits = 5;
  *expected.digits = 0x69696965;
  *(expected.digits + 1) = 0x69696969;
  *(expected.digits + 2) = 0x69696969;
  *(expected.digits + 3) = 0x69696969;
  *(expected.digits + 4) = 0x9;
  test_multiplication(a, b, expected, multiplicationBignumSIMD);

  // TESTS FOR ADDITION

  resetBignums(1, 1, 1);
  // 4 + 8 = 12
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected, false);

  resetBignums(1, 1, 1);
  // 4 + 8 = 12, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected, false);

  resetBignums(1, 1, 1);
  // 9 + 1 = 10
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 10;
  test_addition(a, b, expected, false);

  resetBignums(1, 1, 1);
  // 754 + 754 = 1508
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 1508;
  test_addition(a, b, expected, false);

  resetBignums(2, 1, 2);
  // 4294967296 + 1 = 4294967297
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 1;
  *(expected.digits + 1) = 1;
  test_addition(a, b, expected, false);

  resetBignums(2, 2, 2);
  // 0x13214ab1_13214ab1 + 0x13214ab1_13214ab1 = 0x26429562_26429562
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x26429562;
  *(expected.digits + 1) = 0x26429562;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected, false);

  resetBignums(3, 3, 4);
  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff_ffffffff =
  // 0x1_ffffffff_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0x1;
  test_addition(a, b, expected, false);

  resetBignums(3, 2, 4);
  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff =
  // 0x1_00000000_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0x00000000;
  *(expected.digits + 3) = 0x1;
  test_addition(a, b, expected, false);

  resetBignums(2, 1, 2);
  // 0x5234ad_94724362 + 0x3abf = 0x5234ad_94727e21
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x94727e21;
  *(expected.digits + 1) = 0x5234ad;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected, false);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 + 0xadcbef_afafef69 = 0xadf_ec7dbbdf_6e5a9389
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x6e5a9389;
  *(expected.digits + 1) = 0xec7dbbdf;
  *(expected.digits + 2) = 0xadf;
  *(expected.digits + 3) = 0x0;
  test_addition(a, b, expected, false);

  resetBignums(4, 4, 5);
  // multiple overflow (addition)
  // 0xffffffff_ffffffff_ffffffff_ffffffff +
  // 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 0x1_ffffffff_ffffffff_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *(a.digits + 3) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0x1;
  test_addition(a, b, expected, false);

  resetBignums(2, 1, 3);
  // addition size overflow
  // 0xffffffff_ffffffff + 0x1 = 0x1_00000000_00000000
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *b.digits = 0x1;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0x1;
  test_addition(a, b, expected, false);

  // TESTS FOR ADDITION WITH SIMD

  resetBignums(1, 1, 1);
  // 4 + 8 = 12
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected, true);

  resetBignums(1, 1, 1);
  // 4 + 8 = 12, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected, true);

  resetBignums(1, 1, 1);
  // 9 + 1 = 10
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 10;
  test_addition(a, b, expected, true);

  resetBignums(1, 1, 1);
  // 754 + 754 = 1508
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 1508;
  test_addition(a, b, expected, true);

  resetBignums(2, 1, 2);
  // 4294967296 + 1 = 4294967297
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 1;
  *(expected.digits + 1) = 1;
  test_addition(a, b, expected, true);

  resetBignums(2, 2, 2);
  // 0x13214ab1_13214ab1 + 0x13214ab1_13214ab1 = 0x26429562_26429562
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x26429562;
  *(expected.digits + 1) = 0x26429562;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected, true);

  resetBignums(3, 3, 4);
  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff_ffffffff =
  // 0x1_ffffffff_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0x1;
  test_addition(a, b, expected, true);

  resetBignums(3, 2, 4);
  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff =
  // 0x1_00000000_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0x00000000;
  *(expected.digits + 3) = 0x1;
  test_addition(a, b, expected, true);

  resetBignums(2, 1, 2);
  // 0x5234ad_94724362 + 0x3abf = 0x5234ad_94727e21
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x94727e21;
  *(expected.digits + 1) = 0x5234ad;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected, true);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 + 0xadcbef_afafef69 = 0xadf_ec7dbbdf_6e5a9389
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x6e5a9389;
  *(expected.digits + 1) = 0xec7dbbdf;
  *(expected.digits + 2) = 0xadf;
  *(expected.digits + 3) = 0x0;
  test_addition(a, b, expected, true);

  resetBignums(4, 4, 5);
  // multiple overflow (addition)
  // 0xffffffff_ffffffff_ffffffff_ffffffff +
  // 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 0x1_ffffffff_ffffffff_ffffffff_fffffffe
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *(a.digits + 3) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0x1;
  test_addition(a, b, expected, true);

  resetBignums(5, 5, 6);
  // addition size overflow
  // 0xffffffff_ffffffff + 0x1 = 0x1_00000000_00000000
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *(a.digits + 3) = 0xffffffff;
  *(a.digits + 4) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *(b.digits + 2) = 0xffffffff;
  *(b.digits + 3) = 0xffffffff;
  *(b.digits + 4) = 0xffffffff;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xffffffff;
  *(expected.digits + 3) = 0xffffffff;
  *(expected.digits + 4) = 0xffffffff;
  *(expected.digits + 5) = 0x00000001;
  test_addition(a, b, expected, true);

  resetBignums(10, 8, 10);
  // addition size overflow
  // 0xffffffff_ffffffff + 0x1 = 0x1_00000000_00000000
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0x0;
  *(a.digits + 3) = 0x0;
  *(a.digits + 4) = 0x0;
  *(a.digits + 5) = 0x0;
  *(a.digits + 6) = 0x0;
  *(a.digits + 7) = 0x0;
  *(a.digits + 8) = 0x0;
  *(a.digits + 9) = 0x0;
  *b.digits = 0x1;
  *(b.digits + 1) = 0x1;
  *(b.digits + 2) = 0x1;
  *(b.digits + 3) = 0x1;
  *(b.digits + 4) = 0x1;
  *(b.digits + 5) = 0x1;
  *(b.digits + 6) = 0x1;
  *(b.digits + 7) = 0x1;
  *expected.digits = 0x1;
  *(expected.digits + 1) = 0x1;
  *(expected.digits + 2) = 0x1;
  *(expected.digits + 3) = 0x1;
  *(expected.digits + 4) = 0x1;
  *(expected.digits + 5) = 0x1;
  *(expected.digits + 6) = 0x1;
  *(expected.digits + 7) = 0x1;
  *(expected.digits + 8) = 0x0;
  *(expected.digits + 9) = 0x0;
  *(expected.digits + 10) = 0x0;
  test_addition(a, b, expected, true);

  // TESTS FOR SUBTRACTION

  resetBignums(1, 1, 1);
  // 9 - 1 = 8
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 8;
  test_subtraction(a, b, expected, false);

  resetBignums(2, 1, 1);
  // 4294967296 - 1 = 4294967295
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0;
  test_subtraction(a, b, expected, false);

  resetBignums(3, 2, 3);
  // 0xffffffff_ffffffff_ffffffff - 0xffffffff_ffffffff =
  // 0xffffffff_00000000_00000000
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0xffffffff;
  test_subtraction(a, b, expected, false);

  resetBignums(2, 1, 2);
  // 0x5234ad_94724362 - 0x3abf = 0x5234ad_947208a3
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x947208a3;
  *(expected.digits + 1) = 0x5234ad;
  test_subtraction(a, b, expected, false);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 - 0xadcbef_afafef69 = 0xadf_eb222400_0efab4b7
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x0efab4b7;
  *(expected.digits + 1) = 0xeb222400;
  *(expected.digits + 2) = 0xadf;
  test_subtraction(a, b, expected, false);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 - 0xffffffff_ffffffff = 0xade_ebcfefef_beaaa421
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0xbeaaa421;
  *(expected.digits + 1) = 0xebcfefef;
  *(expected.digits + 2) = 0xade;
  test_subtraction(a, b, expected, false);

  resetBignums(3, 1, 3);
  // //test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000 - 0x1 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0x1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xfffffffe;
  test_subtraction(a, b, expected, false);

  resetBignums(7, 5, 7);
  // //test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000_00000000_00000000_00000000_00000000 -
  // 0x00000001_00000001_00000001_00000001_00000001 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0x0;
  *(a.digits + 3) = 0x0;
  *(a.digits + 4) = 0x0;
  *(a.digits + 5) = 0x0;
  *(a.digits + 6) = 0xffffffff;
  *b.digits = 0x1;
  *(b.digits + 1) = 0x1;
  *(b.digits + 2) = 0x1;
  *(b.digits + 3) = 0x1;
  *(b.digits + 4) = 0x1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xfffffffe;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xfffffffe;
  test_subtraction(a, b, expected, false);

  // SUBTRACTION WITH SIMD

  resetBignums(1, 1, 1);
  // 9 - 1 = 8
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 8;
  test_subtraction(a, b, expected, true);

  resetBignums(2, 1, 1);
  // 4294967296 - 1 = 4294967295
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0;
  test_subtraction(a, b, expected, true);

  resetBignums(3, 2, 3);
  // 0xffffffff_ffffffff_ffffffff - 0xffffffff_ffffffff =
  // 0xffffffff_00000000_00000000
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0xffffffff;
  test_subtraction(a, b, expected, true);

  resetBignums(2, 1, 2);
  // 0x5234ad_94724362 - 0x3abf = 0x5234ad_947208a3
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x947208a3;
  *(expected.digits + 1) = 0x5234ad;
  test_subtraction(a, b, expected, true);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 - 0xadcbef_afafef69 = 0xadf_eb222400_0efab4b7
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xafafef69;
  *(b.digits + 1) = 0xadcbef;
  *expected.digits = 0x0efab4b7;
  *(expected.digits + 1) = 0xeb222400;
  *(expected.digits + 2) = 0xadf;
  test_subtraction(a, b, expected, true);

  resetBignums(3, 2, 3);
  // 0xadf_ebcfefef_beaaa420 - 0xffffffff_ffffffff = 0xade_ebcfefef_beaaa421
  *a.digits = 0xbeaaa420;
  *(a.digits + 1) = 0xebcfefef;
  *(a.digits + 2) = 0xadf;
  *b.digits = 0xffffffff;
  *(b.digits + 1) = 0xffffffff;
  *expected.digits = 0xbeaaa421;
  *(expected.digits + 1) = 0xebcfefef;
  *(expected.digits + 2) = 0xade;
  test_subtraction(a, b, expected, true);

  resetBignums(3, 1, 3);
  // //test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000 - 0x1 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0x1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xfffffffe;
  test_subtraction(a, b, expected, true);

  resetBignums(7, 5, 7);
  // //test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000_00000000_00000000_00000000_00000000 -
  // 0x00000001_00000001_00000001_00000001_00000001 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0x0;
  *(a.digits + 3) = 0x0;
  *(a.digits + 4) = 0x0;
  *(a.digits + 5) = 0x0;
  *(a.digits + 6) = 0xffffffff;
  *b.digits = 0x1;
  *(b.digits + 1) = 0x1;
  *(b.digits + 2) = 0x1;
  *(b.digits + 3) = 0x1;
  *(b.digits + 4) = 0x1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xfffffffe;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xfffffffe;
  test_subtraction(a, b, expected, true);

  resetBignums(7, 5, 7);
  // //test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000_00000000_00000000_00000000_00000000 -
  // 0x00000001_00000001_00000001_00000001_00000001 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *(a.digits + 2) = 0xffffffff;
  *(a.digits + 3) = 0xffffffff;
  *(a.digits + 4) = 0xffffffff;
  *(a.digits + 5) = 0xffffffff;
  *(a.digits + 6) = 0xffffffff;
  *b.digits = 0x1;
  *(b.digits + 1) = 0x1;
  *(b.digits + 2) = 0x1;
  *(b.digits + 3) = 0x1;
  *(b.digits + 4) = 0x1;
  *expected.digits = 0xfffffffe;
  *(expected.digits + 1) = 0xfffffffe;
  *(expected.digits + 2) = 0xfffffffe;
  *(expected.digits + 3) = 0xfffffffe;
  *(expected.digits + 4) = 0xfffffffe;
  *(expected.digits + 5) = 0xffffffff;
  *(expected.digits + 6) = 0xffffffff;
  test_subtraction(a, b, expected, true);

  // TEST DIVISION

  resetBignums(1, 1, 1);
  // 2 / 5 = 0.4 with 10 fraction
  *a.digits = 2;
  *b.digits = 5;
  *expected.digits = 409;
  test_division(a, b, 10, expected);

  resetBignums(1, 1, 1);
  // 2 / 5 = 0.4 with 32 fraction
  *a.digits = 2;
  *b.digits = 5;
  *expected.digits = 0x66666666;
  test_division(a, b, 32, expected);

  resetBignums(1, 1, 3);
  // 2 / 5 = 0.4 with 96 fraction
  *a.digits = 2;
  *b.digits = 5;
  *expected.digits = 0x66666666;
  *(expected.digits + 1) = 0x66666666;
  *(expected.digits + 2) = 0x66666666;
  test_division(a, b, 96, expected);

  resetBignums(1, 1, 2);
  // 5 / 12 = 0.41667 with 64 fraction
  *a.digits = 5;
  *b.digits = 12;
  *expected.digits = 0xAAAAAAAA;
  *(expected.digits + 1) = 0x6AAAAAAA;
  test_division(a, b, 64, expected);

  resetBignums(1, 1, 3);
  // 5 / 12 = 0.41667 with 76 fraction
  *a.digits = 5;
  *b.digits = 12;
  *expected.digits = 0xAAAAAAAA;
  *(expected.digits + 1) = 0xAAAAAAAA;
  *(expected.digits + 2) = 0x6AA;
  test_division(a, b, 76, expected);

  resetBignums(1, 1, 4);
  // 985 / 2378 = 0.41421362 with 128 fraction
  *a.digits = 985;
  *b.digits = 2378;
  *expected.digits = 0x2E1360AC;
  *(expected.digits + 1) = 0x4857D862;
  *(expected.digits + 2) = 0x7B27D4F0;
  *(expected.digits + 3) = 0x6A09E774;
  test_division(a, b, 128, expected);

  resetBignums(1, 1, 8);
  // 985 / 2378 = 0.41421356421 with 256 fraction
  *a.digits = 985;
  *b.digits = 2378;
  *expected.digits = 0x8622E136;
  *(expected.digits + 1) = 0x4F04857D;
  *(expected.digits + 2) = 0x7747B27D;
  *(expected.digits + 3) = 0x3EDEA09E;
  *(expected.digits + 4) = 0x2E1360AC;
  *(expected.digits + 5) = 0x4857D862;
  *(expected.digits + 6) = 0x7B27D4F0;
  *(expected.digits + 7) = 0x6A09E774;
  test_division(a, b, 256, expected);

  resetBignums(1, 1, 1);
  // 985 / 2378 = 0.41421356421 with 32 fraction
  *a.digits = 985;
  *b.digits = 2378;
  *expected.digits = 0x6A09E774;
  test_division(a, b, 32, expected);

  resetBignums(1, 1, 1);
  // 1 / 2 = 0.5 with 7 fraction
  *a.digits = 1;
  *b.digits = 2;
  *expected.digits = 0x40;
  test_division(a, b, 7, expected);

  resetBignums(1, 1, 16);
  // 5741 / 13860 with 512 fraction
  *a.digits = 5741;
  *b.digits = 13860;
  *expected.digits = 0x2A09E66F;
  *(expected.digits + 1) = 0xFDB5ACCF;
  *(expected.digits + 2) = 0xF2A09E66;
  *(expected.digits + 3) = 0x6FDB5ACC;
  *(expected.digits + 4) = 0xCF2A09E6;
  *(expected.digits + 5) = 0x66FDB5AC;
  *(expected.digits + 6) = 0xCCF2A09E;
  *(expected.digits + 7) = 0xE66FDB5A;
  *(expected.digits + 8) = 0xACCF2A09;
  *(expected.digits + 9) = 0x9E66FDB5;
  *(expected.digits + 10) = 0x5ACCF2A0;
  *(expected.digits + 11) = 0x09E66FDB;
  *(expected.digits + 12) = 0xB5ACCF2A;
  *(expected.digits + 13) = 0xA09E66FD;
  *(expected.digits + 14) = 0xDB5ACCF2;
  *(expected.digits + 15) = 0x6A09E66F;
  test_division(a, b, 512, expected);

  // TEST SHIFT LEFT
  resetBignums(1, 1, 1);
  *a.digits = 1;
  *expected.digits = 2;
  test_shift_left(a, 1, expected);
  free(b.digits);

  resetBignums(1, 1, 2);
  *a.digits = 1;
  *expected.digits = 0;
  *(expected.digits + 1) = 1;
  test_shift_left(a, 32, expected);
  free(b.digits);

  resetBignums(1, 1, 2);
  *a.digits = 0xabcddcba;
  *expected.digits = 0xdcba0000;
  *(expected.digits + 1) = 0xabcd;
  test_shift_left(a, 16, expected);
  free(b.digits);

  resetBignums(1, 1, 3);
  *a.digits = 0xabcddcba;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0xdcba0000;
  *(expected.digits + 2) = 0xabcd;
  test_shift_left(a, 48, expected);
  free(b.digits);

  resetBignums(2, 1, 3);
  *a.digits = 0xabcddcba;
  *(a.digits + 1) = 0x12344321;
  *expected.digits = 0xdcba0000;
  *(expected.digits + 1) = 0x4321abcd;
  *(expected.digits + 2) = 0x1234;
  test_shift_left(a, 16, expected);
  free(b.digits);

  resetBignums(2, 1, 5);
  *a.digits = 0xabcddcba;
  *(a.digits + 1) = 0x12344321;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0x0;
  *(expected.digits + 3) = 0xabcddcba;
  *(expected.digits + 4) = 0x12344321;
  test_shift_left(a, 96, expected);
  free(b.digits);

  // 0xab5_5f8911dc_c9411d7c << 57 = 15_6abf1223_b992823a_f8000000_00000000
  resetBignums(3, 1, 5);
  *a.digits = 0xc9411d7c;
  *(a.digits + 1) = 0x5f8911dc;
  *(a.digits + 2) = 0xab5;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0xf8000000;
  *(expected.digits + 2) = 0xb992823a;
  *(expected.digits + 3) = 0x6abf1223;
  *(expected.digits + 4) = 0x15;
  test_shift_left(a, 57, expected);
  free(b.digits);

  // 0xfd4f5d67_7d3c2a13_53757828_781787c7_878a787f_8787d87d_78d8d7d8_7a87f87c_87e7eee2
  // << 123 =
  // 7ea7aeb_3be9e150_9a9babc1_43c0bc3e_3c3c53c3_fc3c3ec3_ebc6c6be_c3d43fc3_e43f3f77_10000000_00000000_00000000_00000000
  resetBignums(9, 1, 13);
  *a.digits = 0x87e7eee2;
  *(a.digits + 1) = 0x7a87f87c;
  *(a.digits + 2) = 0x78d8d7d8;
  *(a.digits + 3) = 0x8787d87d;
  *(a.digits + 4) = 0x878a787f;
  *(a.digits + 5) = 0x781787c7;
  *(a.digits + 6) = 0x53757828;
  *(a.digits + 7) = 0x7d3c2a13;
  *(a.digits + 8) = 0xfd4f5d67;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0x0;
  *(expected.digits + 3) = 0x10000000;
  *(expected.digits + 4) = 0xe43f3f77;
  *(expected.digits + 5) = 0xc3d43fc3;
  *(expected.digits + 6) = 0xebc6c6be;
  *(expected.digits + 7) = 0xfc3c3ec3;
  *(expected.digits + 8) = 0x3c3c53c3;
  *(expected.digits + 9) = 0x43c0bc3e;
  *(expected.digits + 10) = 0x9a9babc1;
  *(expected.digits + 11) = 0x3be9e150;
  *(expected.digits + 12) = 0x7ea7aeb;
  test_shift_left(a, 123, expected);
  free(b.digits);
  // TEST SHIFT RIGHT

  resetBignums(1, 1, 1);
  *a.digits = 8;
  *expected.digits = 1;
  test_shift_right(a, 3, expected);
  free(b.digits);

  // TEST DIVISION 2

  // 0x3333 / 0x8acd = 0x0.5
  resetBignums(1, 1, 1);
  *a.digits = 0x3333;
  *b.digits = 0x8acd;
  *expected.digits = 0x5;
  expected.fracSize = 4;
  test_division(a, b, 4, expected);

  // 0x3333 / 0x8acd = 0x0.5e6e_286aea73_7d405f72
  resetBignums(1, 1, 3);
  *a.digits = 0x3333;
  *b.digits = 0x8acd;
  *expected.digits = 0x7d405f72;
  *(expected.digits + 1) = 0x286aea73;
  *(expected.digits + 2) = 0x5e6e;
  expected.fracSize = 80;
  test_division(a, b, 80, expected);

  // 0x11111111_00000001 / 0x1_00000000_00000000 = 0x0.11111111_00000001
  resetBignums(2, 3, 2);
  *a.digits = 0x00000001;
  *(a.digits + 1) = 0x11111111;
  *b.digits = 0x00000000;
  *(b.digits + 1) = 0x00000000;
  *(b.digits + 2) = 0x1;
  *expected.digits = 0x00000001;
  *(expected.digits + 1) = 0x11111111;
  expected.fracSize = 64;
  test_division(a, b, 64, expected);

  // 0x289d8_c98e98f8_98a8aaa8_98f98482 / 0x11111111111111111111111111111111 =
  // 0x0.002613b3_cf5af690_f1e1ffe0_f69ec39e_002613b3_cf5af690_f1e1ffe0_f69ec39e_002613b3_cf5af690_f1e1ffe0_f69ec39e_002613b3_cf5af690_f1e1ffe0_f69ec39e
  resetBignums(4, 4, 16);
  *a.digits = 0x98f98482;
  *(a.digits + 1) = 0x98a8aaa8;
  *(a.digits + 2) = 0xc98e98f8;
  *(a.digits + 3) = 0x289d8;

  *b.digits = 0x11111111;
  *(b.digits + 1) = 0x11111111;
  *(b.digits + 2) = 0x11111111;
  *(b.digits + 3) = 0x11111111;

  *expected.digits = 0xf69ec39e;
  *(expected.digits + 1) = 0xf1e1ffe0;
  *(expected.digits + 2) = 0xcf5af690;
  *(expected.digits + 3) = 0x002613b3;
  *(expected.digits + 4) = 0xf69ec39e;
  *(expected.digits + 5) = 0xf1e1ffe0;
  *(expected.digits + 6) = 0xcf5af690;
  *(expected.digits + 7) = 0x002613b3;
  *(expected.digits + 8) = 0xf69ec39e;
  *(expected.digits + 9) = 0xf1e1ffe0;
  *(expected.digits + 10) = 0xcf5af690;
  *(expected.digits + 11) = 0x002613b3;
  *(expected.digits + 12) = 0xf69ec39e;
  *(expected.digits + 13) = 0xf1e1ffe0;
  *(expected.digits + 14) = 0xcf5af690;
  *(expected.digits + 15) = 0x002613b3;
  expected.fracSize = 512;
  test_division(a, b,512, expected);

  // print overall resulte
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);

  return 0;
}
