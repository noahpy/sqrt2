#include "matrix_mul.h"

// TODO: Merge with bignum arithmetic
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
    struct bignum *result = malloc(sizeof (struct bignum));
    uint32_t *digits = malloc(sizeof (uint32_t));
    digits[0] = a.digits[0] * b.digits[0];
    result->size = 1;
    result->digits = digits;
    return *result;
}
struct bignum additionBignum(struct bignum a, struct bignum b) {
    struct bignum *result = malloc(sizeof (struct bignum));
    uint32_t *digits = malloc(sizeof (uint32_t));
    digits[0] = a.digits[0] + b.digits[0];
    result->size = 1;
    result->digits = digits;
    return *result;
}

//Calculates the product of two 4x4 matrix
struct matrix4x4 mulMatrix4x4 (struct matrix4x4 a, struct matrix4x4 b) {
    // c11 = a11 * b11 + a12 * b21
    struct bignum c11 = additionBignum(multiplicationBignum(a.a11, b.a11), multiplicationBignum(a.a12, b.a21));
    // c12 = a11 * b12 + a12 * b22
    struct bignum c12 = additionBignum(multiplicationBignum(a.a11, b.a12), multiplicationBignum(a.a12, b.a22));
    // c21 = a21 * b11 + a22 * b21
    struct bignum c21 = additionBignum(multiplicationBignum(a.a21, b.a11), multiplicationBignum(a.a22, b.a21));
    // c22 = a21 * b12 + a22 * b22
    struct bignum c22 = additionBignum(multiplicationBignum(a.a21, b.a12), multiplicationBignum(a.a22, b.a22));
    struct matrix4x4 result = {c11, c12, c21, c22};
    return result;
}

/*
 * A 4x4 in form of | x-1  x  | only needs to store three values in form of a cmp_matrix4x4 (compact 4x4 matrix)
 *                  |  x  x+1 |
 * which makes multiplication a bit easier
 * */
