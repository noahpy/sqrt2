
#include "../decimal_place_converter.h"
#include <stddef.h>
#include <stdio.h>

int test_cases = 0;
int test_passed = 0;

void test_decimal_conv(size_t decimal, size_t expected) {
  test_cases++;
  size_t result = decimal_to_binary_places(decimal);
  if (result >= expected) {
    test_passed++;
  } else {
    printf("Test failed: decimal_place_converter(%zu) == %zu, expected %zu\n",
           decimal, result, expected);
  }
}

void test_hex_conv(size_t hex, size_t expected) {
  test_cases++;
  size_t result = hex_to_binary_places(hex);
  if (result >= expected) {
    test_passed++;
  } else {
    printf("Test failed: hex_place_converter(%zu) == %zu, expected %zu\n", hex,
           result, expected);
  }
}

int main(void) {
  test_decimal_conv(0, 0);
  test_decimal_conv(1, 4);
  test_decimal_conv(7, 24);
  test_decimal_conv(11, 37);
  test_decimal_conv(99, 329);
  test_decimal_conv(200, 665);
  test_decimal_conv(293, 974);
  test_decimal_conv(299, 994);
  test_decimal_conv(539, 1791);
  test_decimal_conv(777, 2582);
  test_decimal_conv(987, 3279);
  test_decimal_conv(1021, 3392);
  test_decimal_conv(2855, 9485);
  test_decimal_conv(3366, 11182);
  test_decimal_conv(5782, 19208);
  test_decimal_conv(13456, 44700);
  test_decimal_conv(294859322, 979501466);

  test_hex_conv(0, 0);
  test_hex_conv(1, 4);
  test_hex_conv(7, 28);
  test_hex_conv(11, 44);
  test_hex_conv(99, 396);
  test_hex_conv(200, 800);
  test_hex_conv(293, 1172);
  test_hex_conv(299, 1196);
  test_hex_conv(539, 2156);
  test_hex_conv(777, 3108);
  test_hex_conv(987, 3948);
  test_hex_conv(1021, 4084);
  test_hex_conv(2855, 11420);
  test_hex_conv(3366, 13464);
  test_hex_conv(5782, 23128);
  test_hex_conv(13456, 53824);
  test_hex_conv(294859322, 1179437288);

  // print overall result
  float success_rate = ((float)test_passed) / ((float)test_cases) * 100;
  printf("PASSED: %d, FAILED: %d, SUCCESS RATE: %.1f%%\n", test_passed,
         (test_cases - test_passed), success_rate);
}
