#include <stdio.h>
#include "mat_fast_exp.h"

// Creates a bignum with value n on the heap TODO: Move to bignum
struct bignum bignumOfInt(uint32_t n) {
    uint32_t *digit = NULL;
    if (!(digit = malloc(sizeof(uint32_t)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Set the value to n
    *digit = n;

    return (struct bignum) {1, digit};
}

// Computes 4x4 matrix to the power of n
struct matrix4x4 powMatrix4x4(struct matrix4x4 a, uint32_t n) {
    /*
     * Initialising a unit matrix or (a)^0 that will be multiplied with other powers of a
     *
     * result = | 1 0 |
     *          | 0 1 |
     * */
    struct matrix4x4 result = {
            bignumOfInt(1),
            bignumOfInt(0),
            bignumOfInt(0),
            bignumOfInt(1)
    };

    // Squaring the matrix and multiply result by it if necessary
    for (; n > 0; n >>= 1) {
        // If the least significant bit is set to 1, multiply result by a
        if (n & 1) {
            result = mulMatrix4x4(result, a);
        }

        // Square a
        a = mulMatrix4x4(a, a);
    }

    return result;
}

/* Computes the power of a 4x4 matrix with following structure: | x-1  x  |
 *                                                              |  x  x+1 |
 * */
struct cmp_matrix4x4 powCmpMatrix4x4(struct cmp_matrix4x4 a, uint32_t n) {
    // The unit matrix is defined as x-1= 1, x = 0 and x+1 = 1
    struct cmp_matrix4x4 result = {
            bignumOfInt(1),
            bignumOfInt(0),
            bignumOfInt(1)
    };

    // First check manual to save one matrix multiplication at the end of the for loop
    if (n & 1) {
        result = mulCmpMatrix4x4(result, a);
    }
    n >>= 1;
    // Squaring the matrix and multiply result by it if necessary
    for (; n > 0; n >>= 1) {
        // Square a
        a = mulCmpMatrix4x4(a, a);

        // If the least significant bit is set to 1, multiply result by a
        if (n & 1) {
            result = mulCmpMatrix4x4(result, a);
        }
    }

    return result;
}
