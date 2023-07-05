
#define _GNU_SOURCE

#include "../utils.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int test_cases = 0;
int test_passed = 0;

void test_print_bignum_dec(struct bignum input, char *expected) {
  test_cases++;
  // read ouput on stdout
  int stdout_bk; // is fd for stdout backup

  stdout_bk = dup(fileno(stdout));

  int pipefd[2];
  pipe2(pipefd, 0); 

  // What used to be stdout will now go to the pipe.
  dup2(pipefd[1], fileno(stdout));

  print_bignum_dec(&input, multiplicationBignum);

  fflush(stdout); // flushall();
  close(pipefd[1]);

  dup2(stdout_bk, fileno(stdout)); // restore

  char buf[strlen(expected) + 1];
  read(pipefd[0], buf, strlen(expected));
  buf[strlen(expected)] = 0;

  // compare output with expected
  if (strncmp(buf, expected, strlen(expected))) {
    fprintf(stderr, "Test number %d failed: Expected\n\t%s\nbut got:\n\t%s\n", test_cases, expected, buf);
    return;
  }
  fprintf(stderr, "Test passed: got %s\n", buf);
  test_passed++;
}

void test_print_bignum_hex(struct bignum input, char *expected) {
  test_cases++;
  // read ouput on stdout
  int stdout_bk; // is fd for stdout backup

  stdout_bk = dup(fileno(stdout));

  int pipefd[2];
  pipe2(pipefd, 0); 

  // What used to be stdout will now go to the pipe.
  dup2(pipefd[1], fileno(stdout));

  print_bignum_hex(&input);

  fflush(stdout); // flushall();
  close(pipefd[1]);

  dup2(stdout_bk, fileno(stdout)); // restore

  char buf[strlen(expected) + 1];
  read(pipefd[0], buf, strlen(expected));
  buf[strlen(expected)] = 0;

  // compare output with expected
  if (strncmp(buf, expected, strlen(expected))) {
    fprintf(stderr, "Test number %d failed: Expected\n\t%s\nbut got:\n\t%s\n", test_cases, expected, buf);
    return;
  }
  fprintf(stderr, "Test passed: got %s\n", buf);
  test_passed++;
}



int main() {
  uint32_t *digits = malloc(4 * sizeof(uint32_t));
  // 0x1_22333444_45555.544_44333221
  digits[0] = 0x44333221;
  digits[1] = 0x45555544;
  digits[2] = 0x22333444;
  digits[3] = 0x1;
  struct bignum number = {digits, 4, 40};
  test_print_bignum_dec(
      number, "81684042822800725.2666656491646790527738630771636962890625");
  free(digits);

  // 0x1.7b72
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0x17b72;
  struct bignum number2 = {digits, 1, 16};
  test_print_bignum_dec(number2, "1.482208251953125");
  free(digits);

  // 0x0.4ccd
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0x4ccd;
  struct bignum number10 = {digits, 1, 16};
  test_print_bignum_dec(number10, "0.3000030517578125");
  free(digits);


  // 0x5
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 5;
  struct bignum number9 = {digits, 1, 0};
  test_print_bignum_dec(number9, "5");
  free(digits);

  // 0x83989_89e89a89_401bbc90_90fed902
  digits = malloc(4 * sizeof(uint32_t));
  digits[0] = 0x90fed902;
  digits[1] = 0x401bbc90;
  digits[2] = 0x89e89a89;
  digits[3] = 0x83989;
  struct bignum number3 = {digits, 4, 0};
  test_print_bignum_dec(number3, "42705369154598132393320858769086722");
  free(digits);

  // 0x1.6A0
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0x16a0;
  struct bignum number4 = {digits, 3, 12};
  test_print_bignum_dec(number4, "1.4140625");
  free(digits);

  // 0x83989_89e89a89_401bbc90_90fed902
  digits = malloc(4 * sizeof(uint32_t));
  digits[0] = 0x90fed902;
  digits[1] = 0x401bbc90;
  digits[2] = 0x89e89a89;
  digits[3] = 0x83989;
  struct bignum number5 = {digits, 4, 0};
  test_print_bignum_hex(number5, "0x8398989e89a89401bbc9090fed902");
  free(digits);

  // 0xF
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0xf;
  struct bignum number6 = {digits, 1, 2};
  test_print_bignum_hex(number6, "0x3.c");
  free(digits);

  // 0b1111_0011_1100_00.00_1101
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0xf3c0d;
  struct bignum number7 = {digits, 1, 6};
  test_print_bignum_hex(number7, "0x3cf0.34");
  free(digits);

  // 0b1111_0011_1100_00.00_1101
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0xf3c0d;
  struct bignum number8 = {digits, 1, 8};
  test_print_bignum_hex(number8, "0xf3c.0d");
  free(digits);

  // 0x0.4ccd
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0x4ccd;
  struct bignum number11 = {digits, 1, 16};
  test_print_bignum_hex(number11, "0x0.4ccd");
  free(digits);



  // print overall resulte
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);

  return 0;
}
