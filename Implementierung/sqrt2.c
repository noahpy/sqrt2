
#include "libs/big_num/big_num.h"
#include "libs/mat_fast_exp.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Dieser Gradient wurde durch automatisierte Experimente ermittelt
// Er ist so gewählt, so dass man weniger als 1% der benötigten konvergenten
// binären Nachkommastellen berechnet.
double gradient = 0.336;

struct bignum sqrt2(size_t s) {
  struct matrix2x2 m = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(1),
                        bignumOfInt(2)};

  size_t n = (size_t)floor(((double)s) * gradient) + 3;
  struct matrix2x2 r =
      powMatrix2x2(m, n, multiplicationBignum, additionBignum);
  // 2.5 * n ist ein Approximatives Minimum für die Anzahl der Stellen, um alle
  // konvergenten Nachkommastellen zu berechnen, die die Exponentation hergibt
  divisionBignum(&r.a12, &r.a22, 2.5 * n, subtractionBignum);
  free(r.a11.digits);
  free(r.a21.digits);
  free(r.a22.digits);
  return r.a12;
}


struct bignum sqrt2_V1(size_t s) {
  struct cmp_matrix2x2 m = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};

  size_t n = (size_t)floor(((double)s) * gradient) + 3;
  struct cmp_matrix2x2 r =
      powCmpMatrix2x2(m, n, multiplicationBignum, additionBignum);
  divisionBignum(&r.x, &r.xp1, 2.5 * n, subtractionBignum);
  free(r.xp1.digits);
  free(r.xm1.digits);
  return r.x;
}

struct bignum sqrt2_V2(size_t s) {
  struct cmp_matrix2x2 m = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};

  size_t n = (size_t)floor(((double)s) * gradient) + 3;
  struct cmp_matrix2x2 r =
      powCmpMatrix2x2(m, n, karazubaMultiplication, additionBignumSIMD);
  divisionBignum(&r.x, &r.xp1, 2.5 * n, subtractionBignumSIMD);
  free(r.xp1.digits);
  free(r.xm1.digits);
  return r.x;
}

struct bignum sqrt2_V3(size_t s) {
  struct cmp_matrix2x2 m = {bignumOfInt(0), bignumOfInt(1), bignumOfInt(2)};

  size_t n = (size_t)floor(((double)s) * gradient) + 3;
  struct cmp_matrix2x2 r =
      powCmpMatrix2x2(m, n, multiplicationBignumSIMD, additionBignumSIMD);
  divisionBignum(&r.x, &r.xp1, 2.5 * n, subtractionBignumSIMD);
  free(r.xp1.digits);
  free(r.xm1.digits);
  return r.x;
}


