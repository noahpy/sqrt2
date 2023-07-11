
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


// mutltiply with magic number ln(10) / ln(2)
size_t decimal_to_binary_places(size_t decimal_places) {
    if(__builtin_umull_overflow(decimal_places, 3, &decimal_places)) {
        fprintf(stderr, "Error: Can not represent decimal places\n");
        return 0;
    }
    size_t fraction = (size_t) ceil(((double)decimal_places) * 0.3219281);
    if(__builtin_uaddl_overflow(decimal_places, fraction, &decimal_places)) {
        fprintf(stderr, "Error: Can not represent decimal places\n");
        return 0;
    }
    return decimal_places; }

size_t hex_to_binary_places(size_t hex_places) {
  if (__builtin_umull_overflow(hex_places, 4, &hex_places)) {
    fprintf(stderr, "Error: Can not represent hexadecimal places\n");
    return 0;
  }
  return hex_places;
}
