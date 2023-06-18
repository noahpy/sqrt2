#include "../matrix_mul.h"
#include <stdio.h>

// Convert to bignum
struct bignum ctb(uint32_t i) {
    struct bignum *b = malloc(sizeof (struct bignum));
    uint32_t *digits = malloc(sizeof (uint32_t));
    *digits = i;
    b->size = 1;
    b->digits = digits;
    return *b;
}

/*
 *
 * Run test with an array of 12 uint32
 * [0...3] -> First 4x4 matrix
 * [4...7] -> Second 4x4 matrix
 * [8..11] -> Expected 4x4 result matrix
 *
 * */
void runTest(int arr[]) {
    struct matrix4x4 first = {
            .a11 = ctb(arr[0]),
            .a12 = ctb(arr[1]),
            .a21 = ctb(arr[2]),
            .a22 = ctb(arr[3]),
    };
    struct matrix4x4 second = {
            .a11 = ctb(arr[4]),
            .a12 = ctb(arr[5]),
            .a21 = ctb(arr[6]),
            .a22 = ctb(arr[7]),
    };

    // Multiply first in place
    struct matrix4x4 r = mulMatrix4x4(first, second);

    // Check for correct result
    if (r.a11.digits[0] != arr[8]) {
        fprintf(stderr, "Error on a11, expected %d but got %d\n", arr[8], r.a11.digits[0]);
        return;
    }
    if (r.a12.digits[0] != arr[9]) {
        fprintf(stderr, "Error on a12, expected %d but got %d\n", arr[9], r.a12.digits[0]);
        return;
    }
    if (r.a21.digits[0] != arr[10]) {
        fprintf(stderr, "Error on a21, expected %d but got %d\n", arr[10], r.a21.digits[0]);
        return;
    }
    if (r.a22.digits[0] != arr[11]) {
        fprintf(stderr, "Error on a22, expected %d but got %d\n", arr[11], r.a22.digits[0]);
        return;
    }
    printf("%s", "case correct\n");
}

int main () {
    int cases[][12] = {{0, 1, 1, 2, 0, 1, 1, 2, 1, 2, 2, 5},
                       {1, 2, 3, 4, 5, 6, 7, 8, 19, 22, 43, 50}};

    for (int i = 0; i < (sizeof cases)/(12*4); i++) {
        runTest(cases[i]);
    }
    return 0;
}