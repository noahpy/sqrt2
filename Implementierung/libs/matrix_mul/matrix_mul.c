#include "matrix_mul.h"
#include "stdio.h"

// Swaps two bignums
void swap(struct bignum *a, struct bignum *b) {
    struct bignum tmp = *a;
    *a = *b;
    *b = tmp;
}

// Calculates the product of two 2x2 matrix, multiply is a function to multiply two bignums
struct matrix2x2 mulMatrix2x2(struct matrix2x2 a, struct matrix2x2 b, struct bignum multiply(struct bignum, struct bignum)) {
    // c11 = a11 * b11 + a12 * b21
    struct bignum c11 = multiply(a.a11, b.a11);
    struct bignum a12_mul_b21 = multiply(a.a12, b.a21);

    // Put the larger bignum in the firs argument as required in additionBignum()
    if (c11.size < a12_mul_b21.size) swap(&c11, &a12_mul_b21);
    additionBignum(&c11, a12_mul_b21);
    free(a12_mul_b21.digits);

    // c12 = a11 * b12 + a12 * b22
    struct bignum c12 = multiply(a.a11, b.a12);
    struct bignum a12_mul_b22 = multiply(a.a12, b.a22);
    if (c12.size < a12_mul_b22.size) swap(&c12, &a12_mul_b22);
    additionBignum(&c12, a12_mul_b22);
    free(a12_mul_b22.digits);

    // c21 = a21 * b11 + a22 * b21
    struct bignum c21 = multiply(a.a21, b.a11);
    struct bignum a22_mul_b21 = multiply(a.a22, b.a21);
    if (c21.size < a22_mul_b21.size) swap(&c21, &a22_mul_b21);
    additionBignum(&c21, a22_mul_b21);
    free(a22_mul_b21.digits);

    // c22 = a21 * b12 + a22 * b22
    struct bignum c22 = multiply(a.a21, b.a12);
    struct bignum a22_mul_b22 = multiply(a.a22, b.a22);
    if (c22.size < a22_mul_b22.size) swap(&c22, &a22_mul_b22);
    additionBignum(&c22, a22_mul_b22);
    free(a22_mul_b22.digits);

    return (struct matrix2x2) {c11, c12, c21, c22};
}

// Free all digit parts of a 2x2 matrix
void free2x2(struct matrix2x2 a) {
    // Free the digit parts
    free(a.a11.digits);
    free(a.a12.digits);
    free(a.a21.digits);
    free(a.a22.digits);
}

// Free all digit parts of a 2x2 matrix
void freeCmp2x2(struct cmp_matrix2x2 a) {
    // Free the digit parts
    free(a.xm1.digits);
    free(a.x.digits);
    free(a.xp1.digits);
}

/*
 * A 2x2 in form of | x-1  x  | only needs to store three values in form of a cmp_matrix2x2 (compact 2x2 matrix)
 *                  |  x  x+1 |
 *
 * If a matrix like this is multiplied by itself for as many times as wanted, it will stay in this form, reducing
 * the number of arithmetic operations from 12 to 8
 *
 * multiply is a function for multiplying two bignums
 *
 * ONLY WORKS WITH | 0 1 | AS BASE MATRIX DUE TO OPTIMIZATION OF X+1
 *                 | 1 2 |
 * */
struct cmp_matrix2x2 mulCmpMatrix2x2(struct cmp_matrix2x2 a, struct cmp_matrix2x2 b, struct bignum multiply(struct bignum, struct bignum)) {

    // x-1 = a.x-1 * b.x-1 + a.x * b.x
    struct bignum xm1 = multiplicationBignum(a.xm1, b.xm1);
    struct bignum ax_mul_bx = multiply(a.x, b.x);
    if (xm1.size < ax_mul_bx.size) swap(&xm1, &ax_mul_bx);
    additionBignum(&xm1, ax_mul_bx);
    free(ax_mul_bx.digits);

    //  x  = a.x-1 * b.x + a.x * b.x+1 or a,x * bx-1 + a.x+1 * b.x
    struct bignum x = multiply(a.xm1, b.x);
    struct bignum ax_mul_bxp1 = multiply(a.x, b.xp1);
    if (x.size < ax_mul_bxp1.size) swap(&x, &ax_mul_bxp1);
    additionBignum(&x, ax_mul_bxp1);
    free(ax_mul_bxp1.digits);

    // x+1 = a.x * b.x + a.x+1 * b.x+1
    /* x+1 can also be computed by 2 * x + x-1 if | 0 1 | is the base matrix (one bitshift and one addition)
     *                                            | 1 2 |
     * */
    struct bignum xp1 = shiftLeft(x, 1);
    additionBignum(&xp1, xm1);

    return (struct cmp_matrix2x2) {xm1, x, xp1};
}

