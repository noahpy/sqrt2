#include <stdio.h>
#include <stdlib.h>
#include "mat_fast_exp.h"

// Computes 2x2 matrix to the power of n
struct matrix2x2 powMatrix2x2(struct matrix2x2 a, uint32_t n, struct bignum multiply(struct bignum, struct bignum)) {
    /*
     * Initialising a unit matrix or (a)^0 that will be multiplied with other powers of a
     *
     * result = | 1 0 |
     *          | 0 1 |
     * */
    struct matrix2x2 result = {
            bignumOfInt(1),
            bignumOfInt(0),
            bignumOfInt(0),
            bignumOfInt(1)
    };

    // Pointer to store and free old matrices
    struct matrix2x2 old;

    // Squaring the matrix and multiply result by it if necessary
    for (; n > 0; n >>= 1) {
        // If the least significant bit is set to 1, multiply result by a
        if (n & 1) {
            old = result;
            result = mulMatrix2x2(result, a, multiply);
            free2x2(old);
        }

        // Square a
        old = a;
        a = mulMatrix2x2(a, a, multiply);
        free2x2(old);
    }
    free2x2(a);

    return result;
}

/* Computes the power of a 2x2 matrix with following structure: | x-1  x  |
 *                                                              |  x  x+1 |
 * */
struct cmp_matrix2x2 powCmpMatrix2x2(struct cmp_matrix2x2 a, uint32_t n, struct bignum multiply(struct bignum, struct bignum)) {
    // The unit matrix is defined as x-1= 1, x = 0 and x+1 = 1
    struct cmp_matrix2x2 result = {
            bignumOfInt(1),
            bignumOfInt(0),
            bignumOfInt(1)
    };

    // Pointer to store and free old matrices
    struct cmp_matrix2x2 old;

    // First check manual to save one matrix multiplication at the end of the for loop
    if (n & 1) {
        old = result;
        result = mulCmpMatrix2x2(result, a, multiply);
        freeCmp2x2(old);
    }
    n >>= 1;
    // Squaring the matrix and multiply result by it if necessary
    for (; n > 0; n >>= 1) {
        // Square a
        old = a;
        a = mulCmpMatrix2x2(a, a, multiply);
        freeCmp2x2(old);

        // If the least significant bit is set to 1, multiply result by a
        if (n & 1) {
            old = result;
            result = mulCmpMatrix2x2(result, a, multiply);
            freeCmp2x2(old);
        }
    }
    freeCmp2x2(a);

    return result;
}
