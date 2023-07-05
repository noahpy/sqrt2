
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
  pipe2(pipefd, 0); // O_NONBLOCK);

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

  // 83989_89e89a89_401bbc90_90fed902
  digits = malloc(4 * sizeof(uint32_t));
  digits[0] = 0x90fed902;
  digits[1] = 0x401bbc90;
  digits[2] = 0x89e89a89;
  digits[3] = 0x83989;
  struct bignum number3 = {digits, 4, 0};
  test_print_bignum_dec(number3, "42705369154598132393320858769086722");
  free(digits);

  // 1.6A0
  digits = malloc(1 * sizeof(uint32_t));
  digits[0] = 0x16a0;
  struct bignum number4 = {digits, 3, 12};
  test_print_bignum_dec(number4, "1.4140625");

  // print overall resulte
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);

  return 0;
}
