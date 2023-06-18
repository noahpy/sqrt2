#include "stdio.h"
#include "../mat_fast_exp.h"

// Creates a new 4x4 zero matrix
struct matrix4x4 createZeroMatrix() {
    struct matrix4x4 zero = {
            bignumOfInt(0),
            bignumOfInt(0),
            bignumOfInt(0),
            bignumOfInt(0),
    };
    return zero;
}

// Stores values in an existing matrix
void storeIn(struct matrix4x4 a, uint32_t arr[]) {
    *(a.a11.digits) = arr[0];
    *(a.a12.digits) = arr[1];
    *(a.a21.digits) = arr[2];
    *(a.a22.digits) = arr[3];
}

// Compares a 4x4 matrix to a uint32_t array
int compareTo(struct matrix4x4 a, uint32_t arr[]) {
    if (*(a.a11.digits) != arr[0]) {
        printf("Wrong value for a11: expected '%d' but got '%d\n", arr[0], *(a.a11.digits));
        return 0;
    }
    if (*(a.a12.digits) != arr[1]) {
        printf("Wrong value for a12: expected '%d' but got '%d\n", arr[1], *(a.a12.digits));
        return 0;
    }
    if (*(a.a21.digits) != arr[2]) {
        printf("Wrong value for a21: expected '%d' but got '%d\n", arr[2], *(a.a21.digits));
        return 0;
    }
    if (*(a.a22.digits) != arr[3]) {
        printf("Wrong value for a22: expected '%d' but got '%d\n", arr[3], *(a.a22.digits));
        return 0;
    }

    return 1;
}

/*
 *
 * Run test with an array of 9 uint32
 * [0...3] -> Base 4x4 matrix
 * [4]     -> Exponent
 * [5...8] -> Expected 4x4 matrix
 *
 * */
int test(uint32_t arr[]) {
    printf("[%d", arr[0]);
    for (int i = 1; i < 9; i++) {
        printf(", %d", arr[i]);
    }

    printf("%s", "]: ");

    struct matrix4x4 matrix = createZeroMatrix();
    storeIn(matrix, arr);

    matrix = powMatrix4x4(matrix, arr[4]);

    int result = compareTo(matrix, (arr + 5));
    free4x4(matrix);
    if (result) {
        printf("%s\n", "Successful");
    }

    return result;
}

int main() {
    uint32_t cases[][9] = {
            {1,   2,  3,  4,  11, 25699957, 37455814, 56183721, 81883678},
            {420, 69, 98, 24, 3,  79930368, 13373442, 18994164, 3178440}
    };
    uint32_t all = (sizeof(cases) / (sizeof(uint32_t) * 9));
    uint32_t passed = 0;

    for (uint32_t i = 0; i < all; i++) {
        passed += test(cases[i]);
    }

    printf("Passed %d out of %d tests: %d%%\n", passed, all, (int) (((double) passed / all) * 100));
    return 0;
}