
#include "libs/big_num/big_num.h"
#include "libs/mat_fast_exp.h"
#include "libs/utils/utils.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

double gradient = 0.336;

struct bignum sqrt2(size_t s) {
  struct cmp_matrix2x2 m = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};

  size_t n = (size_t)floor(((double)s) * gradient) + 3;
  struct cmp_matrix2x2 r = powCmpMatrix2x2(m, n, multiplicationBignum);
  // 2.5 * n ist ein Approximatives Minimum für die Anzahl der Stellen, um alle 
  // konvergenten Nachkommastellen zu berechnen
  divisionBignum2(&r.x, &r.xp1, 2.5 * n);
  print_bignum_dec(&r.x, multiplicationBignum, true);
  freeCmp2x2(r);
}
