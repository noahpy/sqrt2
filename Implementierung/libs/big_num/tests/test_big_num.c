
#include "../big_num.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int test_cases = 0;
int test_passed = 0;

uint32_t *a_digits;
uint32_t *b_digits;
uint32_t *expected_digits;

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
}

void test_addition(struct bignum a, struct bignum b, struct bignum expected) {

  test_cases++;
  additionBignum(&a, b);
  if (a.size == expected.size) {
    for (size_t i = 0; i < result.size; i++) {
      if (result.digits[i] != expected.digits[i]) {
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
               result.digits[i]);
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
    printf(") - size should be %zu, but was %zu\n", expected.size, result.size);
  }
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
  a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  b_digits = (uint32_t *)malloc(16 * sizeof(*b_digits));
  expected_digits = (uint32_t *)malloc(16 * sizeof(*expected_digits));

  if (!a_digits || !b_digits || !expected_digits) {
    printf("Failed allocation");
    return EXIT_FAILURE;
  }

  struct bignum a = {.size = 1, .digits = a_digits};
  struct bignum b = {.size = 1, .digits = b_digits};
  struct bignum expected = {.size = 2, .digits = expected_digits};

  // 4 * 8 = 32
  *a_digits = 4;
  *b_digits = 8;
  *expected_digits = 32;
  test_multiplication(a, b, expected);

  // 4 + 8 = 12
  *a_digits = 4;
  *b_digits = 8;
  *expected_digits = 12;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;

  // 4 * 8 = 32, but a.size is 2
  *a_digits = 4;
  a.size = 2;
  expected.size = 3;
  *expected_digits = 32;
  test_multiplication(a, b, expected);

  // 4 + 8 = 12, but a.size is 2
  *expected_digits = 12;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 9 * 1 = 9
  *a_digits = 9;
  *b_digits = 1;
  *expected_digits = 9;
  a.size = 1;
  expected.size = 2;
  test_multiplication(a, b, expected);

  // 9 + 1 = 10
  *expected_digits = 10;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 9 - 1 = 8
  *a_digits = 9;
  expected.size = 1;
  *expected_digits = 8;
  test_subtraction(a, b, expected);

  // 1 * 9 = 9
  expected.size = 2;
  *a_digits = 1;
  *b_digits = 9;
  *expected_digits = 9;
  test_multiplication(a, b, expected);

  // 1 + 9 = 10
  *expected_digits = 10;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 754 * 754 = 568516
  *a_digits = 754;
  *b_digits = 754;
  *expected_digits = 568516;
  test_multiplication(a, b, expected);

  // 754 + 754 = 1508
  *expected_digits = 1508;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 4294967296 * 1 = 4294967296
  *a_digits = 0;
  *(a_digits + 1) = 1;
  *b_digits = 1;
  *expected_digits = 0;
  *(expected_digits + 1) = 1;
  a.size = 2;
  expected.size = 3;
  test_multiplication(a, b, expected);

  // 4294967296 + 1 = 4294967297
  *expected_digits = 1;
  *(expected_digits + 1) = 1;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;
  
  // 4294967296 - 1 = 4294967295
  *a_digits = 0;
  *(a_digits + 1) = 1;
  expected.size = 2;
  *expected_digits = 0xffffffff;
  *(expected_digits + 1) = 0;
  test_subtraction(a, b, expected);

  // 0x13214ab1_13214ab1 * 0x13214ab1_13214ab1 =
  // 0x16df56a_a048b936_3c47922c_9d6cce61
  *a_digits = 0x13214ab1;
  *(a_digits + 1) = 0x13214ab1;
  *b_digits = 0x13214ab1;
  *(b_digits + 1) = 0x13214ab1;
  *expected_digits = 0x9d6cce61;
  *(expected_digits + 1) = 0x3c47922c;
  *(expected_digits + 2) = 0xa048b936;
  *(expected_digits + 3) = 0x16df56a;
  a.size = 2;
  b.size = 2;
  expected.size = 4;
  test_multiplication(a, b, expected);

  // 0x13214ab1_13214ab1 + 0x13214ab1_13214ab1 = 0x26429562_26429562
  expected.size = 3;
  *expected_digits = 0x26429562;
  *(expected_digits + 1) = 0x26429562;
  *(expected_digits + 2) = 0x0;
  test_addition(a, b, expected);

  // 0x13214ab1_13214ab1 - 0x13214ab1_13214ab1 = 0x0_0
  *a_digits = 0x13214ab1;
  *(a_digits + 1) = 0x13214ab1;
  expected.size = 2;
  *expected_digits = 0x0;
  *(expected_digits + 1) = 0x0;
  test_subtraction(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff_ffffffff =
  // 0xffffffff_ffffffff_fffffffe_00000000_00000000_00000001
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *(a_digits + 2) = 0xffffffff;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *(b_digits + 2) = 0xffffffff;
  *expected_digits = 0x00000001;
  *(expected_digits + 1) = 0x00000000;
  *(expected_digits + 2) = 0x00000000;
  *(expected_digits + 3) = 0xfffffffe;
  *(expected_digits + 4) = 0xffffffff;
  *(expected_digits + 5) = 0xffffffff;
  a.size = 3;
  b.size = 3;
  expected.size = 6;
  test_multiplication(a, b, expected);

  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff_ffffffff =
  // 0x1_ffffffff_ffffffff_fffffffe
  expected.size = 4;
  *expected_digits = 0xfffffffe;
  *(expected_digits + 1) = 0xffffffff;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0x1;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *(a_digits + 2) = 0xffffffff;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *expected_digits = 0x00000001;
  *(expected_digits + 1) = 0x00000000;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0xfffffffe;
  *(expected_digits + 4) = 0xffffffff;
  a.size = 3;
  b.size = 2;
  expected.size = 5;
  test_multiplication(a, b, expected);

  // 0xffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff =
  // 0x1_00000000_ffffffff_fffffffe
  expected.size = 4;
  *expected_digits = 0xfffffffe;
  *(expected_digits + 1) = 0xffffffff;
  *(expected_digits + 2) = 0x00000000;
  *(expected_digits + 3) = 0x1;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // 0xffffffff_ffffffff_ffffffff - 0xffffffff_ffffffff =
  // 0xffffffff_00000000_00000000
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *(a_digits + 2) = 0xffffffff;
  expected.size = 3;
  *expected_digits = 0x0;
  *(expected_digits + 1) = 0x0;
  *(expected_digits + 2) = 0xffffffff;
  test_subtraction(a, b, expected);

  // 0x5234ad_94724362 * 0x3abf = 0x12_dd44a123_a4847a1e
  *a_digits = 0x94724362;
  *(a_digits + 1) = 0x5234ad;
  *b_digits = 0x3abf;
  *expected_digits = 0xa4847a1e;
  *(expected_digits + 1) = 0xdd44a123;
  *(expected_digits + 2) = 0x12;
  a.size = 2;
  b.size = 1;
  expected.size = 3;
  test_multiplication(a, b, expected);

  // 0x5234ad_94724362 + 0x3abf = 0x5234ad_94727e21
  expected.size = 3;
  *expected_digits = 0x94727e21;
  *(expected_digits + 1) = 0x5234ad;
  *(expected_digits + 2) = 0x0;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  a.digits = a_digits;


  // 0x5234ad_94724362 - 0x3abf = 0x5234ad_947208a3
  *a_digits = 0x94724362;
  *(a_digits + 1) = 0x5234ad;
  expected.size = 2;
  *expected_digits = 0x947208a3;
  *(expected_digits + 1) = 0x5234ad;
  test_subtraction(a, b, expected);

  // 0xadf_ebcfefef_beaaa420 * 0xadcbef_afafef69 =
  // 0x7_61fc1a06_bad8d17d_fc98d30e_95173120
  *a_digits = 0xbeaaa420;
  *(a_digits + 1) = 0xebcfefef;
  *(a_digits + 2) = 0xadf;
  *b_digits = 0xafafef69;
  *(b_digits + 1) = 0xadcbef;
  *expected_digits = 0x95173120;
  *(expected_digits + 1) = 0xfc98d30e;
  *(expected_digits + 2) = 0xbad8d17d;
  *(expected_digits + 3) = 0x61fc1a06;
  *(expected_digits + 4) = 0x7;
  a.size = 3;
  b.size = 2;
  expected.size = 5;
  test_multiplication(a, b, expected);

  // 0xadf_ebcfefef_beaaa420 + 0xadcbef_afafef69 = 0xadf_ec7dbbdf_6e5a9389
  expected.size = 4;
  *expected_digits = 0x6e5a9389;
  *(expected_digits + 1) = 0xec7dbbdf;
  *(expected_digits + 2) = 0xadf;
  *(expected_digits + 3) = 0x0;
  test_addition(a, b, expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  a.digits = a_digits;


  // 0xadf_ebcfefef_beaaa420 - 0xadcbef_afafef69 = 0xadf_eb222400_0efab4b7
  *a_digits = 0xbeaaa420;
  *(a_digits + 1) = 0xebcfefef;
  *(a_digits + 2) = 0xadf;
  expected.size = 3;
  *expected_digits = 0x0efab4b7;
  *(expected_digits + 1) = 0xeb222400;
  *(expected_digits + 2) = 0xadf;
  test_subtraction(a, b, expected);

  // 0xadf_ebcfefef_beaaa420 - 0xffffffff_ffffffff = 0xade_ebcfefef_beaaa421
  *a_digits = 0xbeaaa420;
  *(a_digits + 1) = 0xebcfefef;
  *(a_digits + 2) = 0xadf;
  expected.size = 3;
  *expected_digits = 0xbeaaa421;
  *(expected_digits + 1) = 0xebcfefef;
  *(expected_digits + 2) = 0xade;
  b.size = 2;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  test_subtraction(a, b, expected);

  // 0xffffffff_ffffffff_ffffffff * 0xffffffff_ffffffff =
  // 0xffffffff_fffffffe_ffffffff_00000000_00000001
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *(a_digits + 2) = 0xffffffff;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *expected_digits = 0x00000001;
  *(expected_digits + 1) = 0x00000000;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0xfffffffe;
  *(expected_digits + 4) = 0xffffffff;
  a.size = 3;
  b.size = 2;
  expected.size = 5;
  test_multiplication(b, a, expected);

  // test multiple overflow (subtraction)
  // 0xffffffff_00000000_00000000 - 0x1 =
  // 0xfffffffe_ffffffff_ffffffff
  *a_digits = 0x0;
  *(a_digits + 1) = 0x0;
  *(a_digits + 2) = 0xffffffff;
  *b_digits = 0x1;
  *expected_digits = 0xffffffff;
  *(expected_digits + 1) = 0xffffffff;
  *(expected_digits + 2) = 0xfffffffe;
  a.size = 3;
  b.size = 1;
  expected.size = 3;
  test_subtraction(a, b, expected);

  // test one-time overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_fffffffe_ffffffff
  *a_digits = 0x1;
  *(a_digits + 1) = 0x1;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *(b_digits + 2) = 0xffffffff;
  *(b_digits + 3) = 0xffffffff;
  *expected_digits = 0xffffffff;
  *(expected_digits + 1) = 0xfffffffe;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0xffffffff;
  *(expected_digits + 4) = 0x0;
  *(expected_digits + 5) = 0x1;
  a.size = 2;
  b.size = 4;
  expected.size = 6;
  test_multiplication(b, a, expected);

  // test multiple overflow (multiplication)
  // 0x00000001_00000001 * 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000000_ffffffff_ffffffff_ffffffff_fffffffe_ffffffff
  *a_digits = 0x1;
  *(a_digits + 1) = 0x1;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *(b_digits + 2) = 0xffffffff;
  *(b_digits + 3) = 0xffffffff;
  *(b_digits + 4) = 0xffffffff;
  *expected_digits = 0xffffffff;
  *(expected_digits + 1) = 0xfffffffe;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0xffffffff;
  *(expected_digits + 4) = 0xffffffff;
  *(expected_digits + 5) = 0x0;
  *(expected_digits + 6) = 0x1;
  a.size = 2;
  b.size = 5;
  expected.size = 7;
  test_multiplication(a, b, expected);

  // 0x00000001_00000001_00000001 *
  // 0xffffffff_ffffffff_ffffffff_ffffffff_ffffffff_ffffffff =
  // 1_00000001_00000000_ffffffff_ffffffff_ffffffff_fffffffe_fffffffe_ffffffff
  *a_digits = 0x1;
  *(a_digits + 1) = 0x1;
  *(a_digits + 2) = 0x1;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *(b_digits + 2) = 0xffffffff;
  *(b_digits + 3) = 0xffffffff;
  *(b_digits + 4) = 0xffffffff;
  *(b_digits + 5) = 0xffffffff;
  *expected_digits = 0xffffffff;
  *(expected_digits + 1) = 0xfffffffe;
  *(expected_digits + 2) = 0xfffffffe;
  *(expected_digits + 3) = 0xffffffff;
  *(expected_digits + 4) = 0xffffffff;
  *(expected_digits + 5) = 0xffffffff;
  *(expected_digits + 6) = 0x0;
  *(expected_digits + 7) = 0x1;
  *(expected_digits + 8) = 0x1;
  a.size = 3;
  b.size = 6;
  expected.size = 9;
  test_multiplication(a, b, expected);

  // multiple overflow (addition)
  // 0xffffffff_ffffffff_ffffffff_ffffffff + 0xffffffff_ffffffff_ffffffff_ffffffff = 0x1_ffffffff_ffffffff_ffffffff_fffffffe
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *(a_digits + 2) = 0xffffffff;
  *(a_digits + 3) = 0xffffffff;
  *b_digits = 0xffffffff;
  *(b_digits + 1) = 0xffffffff;
  *(b_digits + 2) = 0xffffffff;
  *(b_digits + 3) = 0xffffffff;
  *expected_digits = 0xfffffffe;
  *(expected_digits + 1) = 0xffffffff;
  *(expected_digits + 2) = 0xffffffff;
  *(expected_digits + 3) = 0xffffffff;
  *(expected_digits + 4) = 0x1;
  a.size = 4;
  b.size = 4;
  expected.size = 5;
  test_addition(a,b,expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;


  // addition size overflow
  // 0xffffffff_ffffffff + 0x1 = 0x1_00000000_00000000
  *a_digits = 0xffffffff;
  *(a_digits + 1) = 0xffffffff;
  *b_digits = 0x1;
  *expected_digits = 0x0;
  *(expected_digits + 1) = 0x0;
  *(expected_digits + 2) = 0x1;
  a.size = 2;
  b.size = 1;
  expected.size = 3;
  test_addition(a,b,expected);
  //a_digits = (uint32_t *)malloc(16 * sizeof(*a_digits));
  //a.digits = a_digits;

  // 2 / 5 = 0.4 with 10 fraction 
  a.size = 1;
  b.size = 1;
  *a_digits = 2;
  *b_digits = 5;
  expected.size = 1;
  *expected_digits = 0x199;
  test_division(a, b, 10, expected);

  // print overall result
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);
  free(a_digits);
  free(b_digits);
  free(expected_digits);

  return 0;
}
