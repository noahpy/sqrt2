
#include "../big_num.h"
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
  a = (struct bignum) {.size = as, .digits = malloc(16 * sizeof(uint32_t))};
  b = (struct bignum) {.size = bs, .digits = malloc(16 * sizeof(uint32_t))};
  expected = (struct bignum) {.size = expecteds, .digits = malloc(16 * sizeof(uint32_t))};

  for (size_t i = 0; i < 16; i++) {
      a.digits[i] = 0;
      b.digits[i] = 0;
      expected.digits[i] = 0;
  }
}

void test_multiplication(struct bignum a, struct bignum b,
                         struct bignum expected) {
  test_cases++;
  struct bignum result = multiplicationBignum(a, b);
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf("Test failed: multiplicationBignum( ");
        for (size_t j = a.size - 1; j > 0; j--) {
          printf("%o ", a.digits[j]);
        }
        printf("%o ", a.digits[0]);
        printf(", ");
        for (size_t j = b.size - 1; j > 0; j--) {
          printf("%o ", b.digits[j]);
        }
        printf("%o ", b.digits[0]);
        printf(").digits[%zu] == %o, but got %o\n", i, expected.digits[i],
               result.digits[i]);
        free(result.digits);
        free(a.digits);
        free(b.digits);
        free(expected.digits);
        return;
      }
    }
    test_passed++;
  } else {
    printf("Test failed: multiplicationBignum( ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", a.digits[j]);
    }
    printf("%o ", a.digits[0]);
    printf(", ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", b.digits[j]);
    }
    printf("%o ", b.digits[0]);
    printf(") - size should be %zu, but was %zu\n", expected.size, result.size);
  }
  free(result.digits);
  free(a.digits);
  free(b.digits);
  free(expected.digits);
}

void test_addition(struct bignum a, struct bignum b, struct bignum expected) {

  test_cases++;
  additionBignum(&a, b);
  if (a.size == expected.size) {
    for (size_t i = 0; i < a.size; i++) {
      if (a.digits[i] != expected.digits[i]) {
        printf("Test failed: additionBignum( ");
        for (size_t j = a.size - 1; j > 0; j--) {
          printf("%o ", a.digits[j]);
        }
        printf("%o ", a.digits[0]);
        printf(", ");
        for (size_t j = b.size - 1; j > 0; j--) {
          printf("%o ", b.digits[j]);
        }
        printf("%o ", b.digits[0]);
        printf(").digits[%zu] == %o, but got %o\n", i, expected.digits[i],
               a.digits[i]);
        return;
      }
    }
    test_passed++;
  } else {
    printf("Test failed: additionBignum( ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", a.digits[j]);
    }
    printf("%o ", a.digits[0]);
    printf(", ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", b.digits[j]);
    }
    printf("%o ", b.digits[0]);
    printf(") - size should be %zu, but was %zu\n", expected.size, a.size);
  }
  free(expected.digits);
  free(a.digits);
  free(b.digits);
}

void test_subtraction(struct bignum a, struct bignum b, struct bignum expected) {
  test_cases++;
  subtractionBignum(&a, b);
  struct bignum result = a;
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf("Test failed: subtractionBignum( ");
        for (size_t j = a.size - 1; j > 0; j--) {
          printf("%o ", a.digits[j]);
        }
        printf("%o ", a.digits[0]);
        printf(", ");
        for (size_t j = b.size - 1; j > 0; j--) {
          printf("%o ", b.digits[j]);
        }
        printf("%o ", b.digits[0]);
        printf(").digits[%zu] == %o, but got %o\n", i, expected.digits[i],
               result.digits[i]);
        free(a.digits);
        free(b.digits);
        free(expected.digits);
        return;
      }
    }
    test_passed++;
  } else {
    printf("Test failed: subtractionBignum( ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", a.digits[j]);
    }
    printf("%o ", a.digits[0]);
    printf(", ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", b.digits[j]);
    }
    printf("%o ", b.digits[0]);
    printf(") - size should be %zu, but was %zu\n", expected.size, result.size);
  }
  free(a.digits);
  free(b.digits);
  free(expected.digits);
}

void test_division(struct bignum a, struct bignum b, size_t number,
                         struct bignum expected) {
  test_cases++;
  goldschmidt(&a, b, number);
  struct bignum result = a;
  if (result.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
        printf("Test failed: divisionBignum( ");
        for (size_t j = a.size - 1; j > 0; j--) {
          printf("%o ", a.digits[j]);
        }
        printf("%o ", a.digits[0]);
        printf(", ");
        for (size_t j = b.size - 1; j > 0; j--) {
          printf("%o ", b.digits[j]);
        }
        printf("%o ", b.digits[0]);
        printf(").digits[%zu] == %o, but got %o\n", i, expected.digits[i],
               result.digits[i]);
        return;
      }
    }
    test_passed++;
  } else {
    printf("Test failed: divisionBignum( ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", a.digits[j]);
    }
    printf("%o ", a.digits[0]);
    printf(", ");
    for (size_t j = b.size - 1; j > 0; j--) {
      printf("%o ", b.digits[j]);
    }
    printf("%o ", b.digits[0]);
    printf(") - size should be %zu, but was %zu\n", expected.size, result.size);
  }
}

int main(void) {

  resetBignums(1, 1, 2);
  // 4 * 8 = 32
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected);

  resetBignums(1, 1, 2);
  // 4 + 8 = 12
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected);

  resetBignums(1, 1, 2);
  // 4 * 8 = 32, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 32;
  test_multiplication(a, b, expected);

  resetBignums(1, 1, 2);
  // 4 + 8 = 12, but a.size is 2
  *a.digits = 4;
  *b.digits = 8;
  *expected.digits = 12;
  test_addition(a, b, expected);

  resetBignums(1, 1, 2);
  // 9 * 1 = 9
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 9;
  test_multiplication(a, b, expected);

  resetBignums(1, 1, 2);
  // 9 + 1 = 10
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 10;
  test_addition(a, b, expected);

  resetBignums(1, 1, 1);
  // 9 - 1 = 8
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 8;
  test_subtraction(a, b, expected);

  resetBignums(1, 1, 2);
  // 1 * 9 = 9
  *a.digits = 1;
  *b.digits = 9;
  *expected.digits = 9;
  test_multiplication(a, b, expected);

  resetBignums(1, 1, 2);
  // 1 + 9 = 10
  *a.digits = 9;
  *b.digits = 1;
  *expected.digits = 10;
  test_addition(a, b, expected);

  resetBignums(1, 1, 2);
  // 754 * 754 = 568516
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 568516;
  test_multiplication(a, b, expected);

  resetBignums(1, 1, 2);
  // 754 + 754 = 1508
  *a.digits = 754;
  *b.digits = 754;
  *expected.digits = 1508;
  test_addition(a, b, expected);

  resetBignums(2, 1, 3);
  // 4294967296 * 1 = 4294967296
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0;
  *(expected.digits + 1) = 1;
  test_multiplication(a, b, expected);

  resetBignums(2, 1, 3);
  // 4294967296 + 1 = 4294967297
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 1;
  *(expected.digits + 1) = 1;
  test_addition(a, b, expected);

  resetBignums(2, 1, 2);
  // 4294967296 - 1 = 4294967295
  *a.digits = 0;
  *(a.digits + 1) = 1;
  *b.digits = 1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0;
  test_subtraction(a, b, expected);

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
  test_multiplication(a, b, expected);

  resetBignums(2, 2, 3);
  // 0x13214ab1_13214ab1 + 0x13214ab1_13214ab1 = 0x26429562_26429562
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x26429562;
  *(expected.digits + 1) = 0x26429562;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected);

  resetBignums(2, 2, 2);
  // 0x13214ab1_13214ab1 - 0x13214ab1_13214ab1 = 0x0_0
  *a.digits = 0x13214ab1;
  *(a.digits + 1) = 0x13214ab1;
  *b.digits = 0x13214ab1;
  *(b.digits + 1) = 0x13214ab1;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  test_subtraction(a, b, expected);

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
  test_multiplication(a, b, expected);

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
  test_addition(a, b, expected);

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
  test_multiplication(a, b, expected);

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
  test_addition(a, b, expected);

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
  test_subtraction(a, b, expected);

  resetBignums(2, 1, 3);
  // 0x5234ad_94724362 * 0x3abf = 0x12_dd44a123_a4847a1e
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0xa4847a1e;
  *(expected.digits + 1) = 0xdd44a123;
  *(expected.digits + 2) = 0x12;
  test_multiplication(a, b, expected);

  resetBignums(2, 1, 3);
  // 0x5234ad_94724362 + 0x3abf = 0x5234ad_94727e21
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x94727e21;
  *(expected.digits + 1) = 0x5234ad;
  *(expected.digits + 2) = 0x0;
  test_addition(a, b, expected);

  resetBignums(2, 1, 2);
  // 0x5234ad_94724362 - 0x3abf = 0x5234ad_947208a3
  *a.digits = 0x94724362;
  *(a.digits + 1) = 0x5234ad;
  *b.digits = 0x3abf;
  *expected.digits = 0x947208a3;
  *(expected.digits + 1) = 0x5234ad;
  test_subtraction(a, b, expected);

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
  test_multiplication(a, b, expected);

  resetBignums(3, 2, 4);
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
  test_addition(a, b, expected);

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
  test_subtraction(a, b, expected);

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
  test_subtraction(a, b, expected);

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
  test_multiplication(b, a, expected);

  resetBignums(3, 1, 3);
  // test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000 - 0x1 =
  // 0xfffffffe_ffffffff_ffffffff
  *a.digits = 0x0;
  *(a.digits + 1) = 0x0;
  *(a.digits + 2) = 0xffffffff;
  *b.digits = 0x1;
  *expected.digits = 0xffffffff;
  *(expected.digits + 1) = 0xffffffff;
  *(expected.digits + 2) = 0xfffffffe;
  test_subtraction(a, b, expected);

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
  test_multiplication(b, a, expected);

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
  test_multiplication(a, b, expected);

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
  test_multiplication(a, b, expected);

  resetBignums(4, 4, 5);
  // multiple overflow (addition)
  // 0xffffffff_ffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff_ffffffff_ffffffff = 0x1_ffffffff_ffffffff_ffffffff_fffffffe
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
  test_addition(a,b,expected);

  resetBignums(2, 1, 3);
  // addition size overflow
  // 0xffffffff_ffffffff + 0x1 = 0x1_00000000_00000000
  *a.digits = 0xffffffff;
  *(a.digits + 1) = 0xffffffff;
  *b.digits = 0x1;
  *expected.digits = 0x0;
  *(expected.digits + 1) = 0x0;
  *(expected.digits + 2) = 0x1;
  test_addition(a,b,expected);

  // 2 / 5 = 0.4 with 10 fraction 
  // a.size = 1;
  // b.size = 1;
  // *a.digits = 2;
  // *b.digits = 5;
  // expected.size = 1;
  // *expected_digits = 0x199;
  // test_division(a, b, 10, expected);

  // print overall result
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);

  return 0;
}
