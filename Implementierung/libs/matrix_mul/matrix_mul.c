#include "matrix_mul.h"

// TODO: Merge with bignum arithmetic
struct bignum multiplicationBignum(struct bignum a, struct bignum b) {
    struct bignum *result = malloc(sizeof(struct bignum));
    uint32_t *digits = malloc(sizeof(uint32_t));
    digits[0] = a.digits[0] * b.digits[0];
    result->size = 1;
    result->digits = digits;
    return *result;
}

struct bignum additionBignum(struct bignum a, struct bignum b) {
    struct bignum *result = malloc(sizeof(struct bignum));
    uint32_t *digits = malloc(sizeof(uint32_t));
    digits[0] = a.digits[0] + b.digits[0];
    result->size = 1;
    result->digits = digits;
    return *result;
}

// Calculates the product of two 4x4 matrix
struct matrix4x4 mulMatrix4x4(struct matrix4x4 a, struct matrix4x4 b) {
    // c11 = a11 * b11 + a12 * b21
    struct bignum c11 = additionBignum(multiplicationBignum(a.a11, b.a11), multiplicationBignum(a.a12, b.a21));
    // c12 = a11 * b12 + a12 * b22
    struct bignum c12 = additionBignum(multiplicationBignum(a.a11, b.a12), multiplicationBignum(a.a12, b.a22));
    // c21 = a21 * b11 + a22 * b21
    struct bignum c21 = additionBignum(multiplicationBignum(a.a21, b.a11), multiplicationBignum(a.a22, b.a21));
    // c22 = a21 * b12 + a22 * b22
    struct bignum c22 = additionBignum(multiplicationBignum(a.a21, b.a12), multiplicationBignum(a.a22, b.a22));
    return (struct matrix4x4) {c11, c12, c21, c22};
}

// Free all digit parts of a 4x4 matrix
void free4x4(struct matrix4x4 a) {
    // Free the digit parts
    free(a.a11.digits);
    free(a.a12.digits);
    free(a.a21.digits);
    free(a.a22.digits);
}

// Free all digit parts of a 4x4 matrix
void freeCmp4x4(struct cmp_matrix4x4 a) {
    // Free the digit parts
    free(a.xm1.digits);
    free(a.x.digits);
    free(a.xp1.digits);
}

/*
 * A 4x4 in form of | x-1  x  | only needs to store three values in form of a cmp_matrix4x4 (compact 4x4 matrix)
 *                  |  x  x+1 |
 *
 * If a matrix like this is multiplied by itself for as many times as wanted, it will stay in this form, reducing
 * the number of arithmetic operations from 12 to 8
 * */
struct cmp_matrix4x4 mulCmpMatrix4x4(struct cmp_matrix4x4 a, struct cmp_matrix4x4 b) {
    // a.x * b.x is used for x-1 and x+1
    struct bignum square_of_xs =  multiplicationBignum(a.x, b.x);

    // x-1 = a.x-1 * b.x-1 + a.x * b.x
    struct bignum xm1 = additionBignum(multiplicationBignum(a.xm1, b.xm1), square_of_xs);
    //  x  = a.x-1 * b.x + a.x * b.x+1 or a,x * bx-1 + a.x+1 * b.x
    struct bignum x = additionBignum(multiplicationBignum(a.xm1, b.x), multiplicationBignum(a.x, b.xp1));
    // x+1 = a.x * b.x + a.x+1 * b.x+1
    /* TODO: x+1 can also be computed by 2 * x + x-1 if | 0 1 | is the base matrix (one bitshift and one addition)
     *                                                  | 1 2 |
     * */
    struct bignum xp1 = additionBignum(square_of_xs, multiplicationBignum(a.xp1, b.xp1));
    return (struct cmp_matrix4x4) {xm1, x, xp1};
}

