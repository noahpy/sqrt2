//
// Created by Leon Kniffki on 22.06.23.
//
#include <stdio.h>
#include <stdlib.h>
//#include "big_num.c"

// Creates a bignum with value n on the heap TODO: Move to bignum
/*struct bignum bignumOfInt(uint32_t n) {
    uint32_t *digit = NULL;
    if (!(digit = malloc(sizeof(uint32_t)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Set the value to n
    *digit = n;

    return (struct bignum) {1, digit};
}*/

void test (size_t n) {
    // n is the size of the integers in bits


    uint64_t q[100];
    size_t qptr = 2;
    q[0] = q[1] = 16;
    uint64_t r[100];
    r[0] = r[1] = 4;
    size_t rptr = 2;
    uint64_t Q = 4;
    uint64_t R = 2;
    uint64_t k = 1;

    while (q[k-1] + q[k] < n) {
        k++;
        Q += R;
        if ((R+1) * (R+1) <= Q) {
            R++;
        }
        q[k] = (1 << Q);
        r[k] = (2 << R);
    }
}

void recKarazubaMultiplication(uint64_t *digits, uint32_t *x, uint32_t *y, size_t n,
                               size_t offset) {
    if (n == 1) {
        digits[offset] = (uint64_t) x[offset] * y[offset];
    } else {
        size_t left = n / 2;

        // Compute x0 * y0
        recKarazubaMultiplication(digits, x, y, left, offset);
        recKarazubaMultiplication(digits, x, y, n - left, offset + left);
    }
}

void add_bignums(uint32_t *digits, uint32_t *x, uint32_t *y, size_t n) {
    // Take bignum a into result and zero the rest
    for (size_t i = 0; i < n; i++) {
        digits[i] = x[i];
    }

    // Add the 32bit blocks of b to the corresponding blocks of a
    for (size_t i = 0; i < n; i++) {
        uint64_t b64 = (uint64_t) y[i];

        // If there is an addition overflow, increment the third 32bit block
        if (__builtin_uaddl_overflow(b64, *(uint64_t *)(digits + i),
                                     (unsigned long *) (uint64_t *) (digits + i))) {
            __builtin_uaddl_overflow(1, *(uint64_t *)(digits + 2 + i),
                                     (unsigned long *) (uint64_t *) (digits + 2 + i));
        }
    }
}

uint32_t *karazubaMultiplication(uint32_t *x, uint32_t *y, size_t size) {
    // Don't expand small bignums < 16 bit to avoid exponential memory usage
    uint32_t *bignumDigits = NULL;
    if (!(bignumDigits = malloc((2 * size) * sizeof(*bignumDigits)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    recKarazubaMultiplication((uint64_t *) bignumDigits, x, y, size, 0);
    return bignumDigits;
}


int main() {
    //test(70000);
    uint32_t *first = malloc(4 * sizeof(uint32_t));
    uint32_t *second = malloc(4 * sizeof(uint32_t));
    first[0] = 1;
    first[1] = 2;
    first[2] = 3;
    first[3] = 4;
    second[0] = 5;
    second[1] = 6;
    second[2] = 7;
    second[3] = 8;
    karazubaMultiplication(first, second, 4);
    return 0;
}