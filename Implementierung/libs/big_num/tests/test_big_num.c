
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../big_num.h"

int test_cases = 0;
int test_passed = 0;

uint32_t *a_digits;
uint32_t *b_digits;
uint32_t *expected_digits;

void test_multiplication(struct bignum a, struct bignum b, struct bignum expected){
    test_cases++;
    struct bignum result = multiplicationBignum(a, b);
    if (result.size == expected.size){
        for (size_t i = 0; i < result.size; i++) {
            if (result.digits[i] != expected.digits[i]) {
                printf("Test failed: multiplicationBignum(");
                for (size_t j = a.size-1; j > 0; j--){
                    printf("%d ", a.digits[j]);
                }
                printf("%d ", a.digits[0]);
                printf(", ");
                for (size_t j = b.size-1; j > 0; j--){
                    printf("%d ", b.digits[j]);
                }
                printf("%d ", b.digits[0]);
                printf(").digits[%zu] == %d, but got %d\n", i, expected.digits[i], result.digits[i]);
                free(result.digits);
                return;
            }
        }
        test_passed++;
    } else {
        printf("Test failed: multiplicationBignum(");
        for (size_t j = b.size-1; j > 0; j--){
            printf("%d ", a.digits[j]);
        }
        printf("%d ", a.digits[0]);
        printf(", ");
        for (size_t j = b.size-1; j > 0; j--){
            printf("%d ", b.digits[j]);
        }
        printf("%d ", b.digits[0]);
        printf(") - size should be %zu, but was %zu\n", expected.size, result.size);
    }
    free(result.digits);
}

int main(void){
  a_digits = (uint32_t*) malloc(16 * sizeof(*a_digits));
  b_digits = (uint32_t*) malloc(16 * sizeof(*b_digits));
  expected_digits = (uint32_t*) malloc(16 * sizeof(*expected_digits));

  if (!a_digits || !b_digits || !expected_digits) {
    printf("failed alloc a b or expected");
    return EXIT_FAILURE;
  }

  struct bignum a = { .size = 1 , .digits = a_digits };
  struct bignum b = { .size = 1 , .digits = b_digits };
  struct bignum expected = { .size = 2 , .digits = expected_digits };

  // 4 * 8 = 32
  *a_digits = 4; *b_digits = 8; *expected_digits = 32;
  test_multiplication(a, b, expected);

  // 4 * 8 = 32, but a.size is 2
  a.size = 2;
  expected.size = 3;
  test_multiplication(a, b, expected);

  // 9 * 1 = 9
  *a_digits = 9; *b_digits = 1; *expected_digits = 9;
  a.size = 1;
  expected.size = 2;
  test_multiplication(a, b, expected);

  // 1 * 9 = 9
  *a_digits = 1; *b_digits = 9; *expected_digits = 9;
  test_multiplication(a, b, expected);

  // 754 * 754 = 568516
  *a_digits = 754; *b_digits = 754; *expected_digits = 568516;
  test_multiplication(a, b, expected);

  // 4294967296 * 1 = 4294967296
  *a_digits = 0; *(a_digits+1) = 1; *b_digits = 1; *expected_digits = 0; *(expected_digits+1) = 1;
  a.size = 2;
  expected.size = 3;
  test_multiplication(a, b, expected);

  // print overall result
  float success_rate = ((float)test_passed)/((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCESS RATE: %.1f%%\n", test_passed, (test_cases-test_passed), success_rate);
  free(a_digits); free(b_digits); free(expected_digits);

  return 0;
}
