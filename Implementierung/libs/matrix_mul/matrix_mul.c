#include "matrix_mul.h"


// Calculates the product of two 2x2 matrix, multiply is a function to multiply two bignums
struct matrix2x2
mulMatrix2x2(struct matrix2x2 a, struct matrix2x2 b, struct bignum multiply(struct bignum, struct bignum)) {
    // c11 = a11 * b11 + a12 * b21
    struct bignum c11 = additionBignum(multiply(a.a11, b.a11), multiply(a.a12, b.a21));
    // c12 = a11 * b12 + a12 * b22
    struct bignum c12 = additionBignum(multiply(a.a11, b.a12), multiply(a.a12, b.a22));
    // c21 = a21 * b11 + a22 * b21
    struct bignum c21 = additionBignum(multiply(a.a21, b.a11), multiply(a.a22, b.a21));
    // c22 = a21 * b12 + a22 * b22
    struct bignum c22 = additionBignum(multiply(a.a21, b.a12), multiply(a.a22, b.a22));
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
 * */
struct cmp_matrix2x2
mulCmpMatrix2x2(struct cmp_matrix2x2 a, struct cmp_matrix2x2 b, struct bignum multiply(struct bignum, struct bignum)) {
    // a.x * b.x is used for x-1 and x+1
    struct bignum square_of_xs = multiply(a.x, b.x);

    // x-1 = a.x-1 * b.x-1 + a.x * b.x
    struct bignum xm1 = additionBignum(multiply(a.xm1, b.xm1), square_of_xs);
    //  x  = a.x-1 * b.x + a.x * b.x+1 or a,x * bx-1 + a.x+1 * b.x
    struct bignum x = additionBignum(multiply(a.xm1, b.x), multiply(a.x, b.xp1));
    // x+1 = a.x * b.x + a.x+1 * b.x+1
    /* TODO: x+1 can also be computed by 2 * x + x-1 if | 0 1 | is the base matrix (one bitshift and one addition)
     *                                                  | 1 2 |
     * */
    struct bignum xp1 = additionBignum(square_of_xs, multiply(a.xp1, b.xp1));
    return (struct cmp_matrix2x2) {xm1, x, xp1};
}

