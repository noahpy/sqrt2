#include "stdio.h"
#include "../mat_fast_exp.h"

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

// Creates a new 2x2 matrix with the given values in array [0...3]
struct matrix2x2 createMatrix(uint32_t arr[]) {
    struct matrix2x2 m = {
            bignumOfInt(arr[0]),
            bignumOfInt(arr[1]),
            bignumOfInt(arr[2]),
            bignumOfInt(arr[3]),
    };
    return m;
}

// Creates a new compact 2x2 matrix with the given values in array [0...2]
struct cmp_matrix2x2 createCmpMatrix(uint32_t arr[]) {
    return (struct cmp_matrix2x2) {
            bignumOfInt(arr[0]),
            bignumOfInt(arr[1]),
            bignumOfInt(arr[2])
    };
}

// Compares a 2x2 matrix to a uint32_t array
int compareTo(struct matrix2x2 a, uint32_t arr[]) {
    if (*(a.a11.digits) != arr[0]) {
        printf("Wrong value for a11: expected '%d' but got '%d'\n", arr[0], *(a.a11.digits));
        return 0;
    }
    if (*(a.a12.digits) != arr[1]) {
        printf("Wrong value for a12: expected '%d' but got '%d'\n", arr[1], *(a.a12.digits));
        return 0;
    }
    if (*(a.a21.digits) != arr[2]) {
        printf("Wrong value for a21: expected '%d' but got '%d'\n", arr[2], *(a.a21.digits));
        return 0;
    }
    if (*(a.a22.digits) != arr[3]) {
        printf("Wrong value for a22: expected '%d' but got '%d'\n", arr[3], *(a.a22.digits));
        return 0;
    }

    return 1;
}

// Compares a compact 2x2 matrix to a uint32_t array
int compareCmpTo(struct cmp_matrix2x2 a, uint32_t arr[]) {
    if (*(a.xm1.digits) != arr[0]) {
        printf("Wrong value for x-1: expected '%d' but got '%d'\n", arr[0], *(a.xm1.digits));
        return 0;
    }

    if (*(a.x.digits) != arr[1]) {
        printf("Wrong value for x: expected '%d' but got '%d'\n", arr[1], *(a.x.digits));
        return 0;
    }
    if (*(a.xp1.digits) != arr[2]) {
        printf("Wrong value for x+1: expected '%d' but got '%d'\n", arr[2], *(a.xp1.digits));
        return 0;
    }

    return 1;
}

/*
 *
 * Run test with an array of 9 uint32
 * [0...3] -> Base 2x2 matrix
 * [4]     -> Exponent
 * [5...8] -> Expected 2x2 matrix
 *
 * */
int test(uint32_t arr[]) {
    printf("[%d", arr[0]);
    for (int i = 1; i < 9; i++) {
        printf(", %d", arr[i]);
    }

    printf("%s", "]: ");

    struct matrix2x2 matrix = createMatrix(arr);

    matrix = powMatrix2x2(matrix, arr[4], multiplicationBignum);

    int result = compareTo(matrix, (arr + 5));
    free2x2(matrix);
    if (result) {
        printf("%s\n", "Successful");
    }

    return result;
}

/*
 *
 * Run test with an array of 12 uint32
 * [0...2] -> First 2x2 matrix
 * [3] -> Exponent
 * [4...6] -> Expected 2x2 result matrix
 *
 * */
int testCmp(uint32_t arr[]) {
    printf("[%d", arr[0]);
    for (int i = 1; i < 7; i++) {
        printf(", %d", arr[i]);
    }

    printf("%s", "]: ");

    struct cmp_matrix2x2 matrix = createCmpMatrix(arr);

    matrix = powCmpMatrix2x2(matrix, arr[3], multiplicationBignum);

    int result = compareCmpTo(matrix, (arr + 4));
    freeCmp2x2(matrix);
    if (result) {
        printf("%s\n", "Successful");
    }

    return result;
}

int main() {
    uint32_t cases[][9] = {
            {1,      2,      3,      4,      11,      25699957,    37455814,    56183721,    81883678},
            {420,    69,     98,     24,     3,       79930368,    13373442,    18994164,    3178440},
            {1,      0,      0,      1,      1000000, 1,           0,           0,           1},
            {-28761, 21397,  -27268, -11555, 3,       -1897892709, 454276325,   -357118660,  1064016689},
            {-26287, 6501,   -18109, -24926, 10,      1496308931,  -1314309295, -464074569,  1303737936},
            {30834,  4392,   15327,  -27036, 16,      -27852800,   -656670720,  -29720576,   1143603200},
            {-15176, 13645,  31876,  2030,   9,       -1943515648, 921582080,   311527424,   620853760},
            {29665,  5355,   1117,   10678,  8,       -1273614360, -1341289887, 966602871,   2067328455},
            {19179,  -10071, 31197,  -4191,  5,       -1475788532, -1464912348, -340508428,  390883556},
            {7661,   18230,  5768,   24015,  13,      1323213949,  231505254,   159949000,   -1457911569},
            {-29101, 2566,   1289,   -2509,  3,       868164945,   1352340710,  -189368999,  203543345},
            {-24808, -13810, 17798,  -3162,  10,      -405080064,  1432681472,  1691577344,  721220608},
            {-7693,  1322,   438,    -15388, 9,       664279027,   -707165110,  2072381590,  27439668},
            {-32766, 28965,  8663,   -23390, 12,      -1233461503, -270904568,  -1025723560, 1142243329},
            {-4112,  -11249, -32318, -1565,  9,       -2033551774, -60417535,   1531962398,  -1237163969},
            {-1283,  11047,  -7519,  129,    15,      -848928384,  286602624,   1041669760,  -633840768},
            {-6850,  32571,  3441,   32540,  6,       1729761499,  609775498,   -799953122,  -1039349473},
            {6217,   11072,  -17218, -8379,  14,      1166065457,  -1069598080, -592773668,  -1712665111},
            {14702,  -29516, 28860,  18267,  11,      -750217200,  -772542772,  192527812,   -1402081021},
            {-19056, -6122,  -16050, -25418, 9,       -729172480,  -1382054912, -1188899840, 319527424},
            {-2191,  15274,  22687,  -16679, 14,      -2011861873, -467347940,  1315530218,  -203082369}
    };

    uint32_t cmpCases[][7] = {
            /* a = | 0 1 |
             *     | 1 2 |
             * */
            // a^2
            {0, 1, 2, 2,  1,       2,        5},
            // a^12
            {0, 1, 2, 12, 5741,    13860,    33461},
            // a^16
            {0, 1, 2, 16, 195025,  470832,   1136689},
            // a^5
            {0, 1, 2, 5,  12,      29,       70},
            // a^20
            {0, 1, 2, 20, 6625109, 15994428, 38613965},

    };

    uint32_t normal = (sizeof(cases) / (sizeof(uint32_t) * 9));
    uint32_t passed = 0;
    uint32_t all = normal;

    printf("%s\n", "Testing powMatrix2x2(struct matrix2x2, uint32_t):");
    for (uint32_t i = 0; i < normal; i++) {
        passed += test(cases[i]);
    }
    printf("\n");

    uint32_t compact = (sizeof(cmpCases) / (sizeof(uint32_t) * 7));
    all += compact;

    printf("%s\n", "Testing powCmpMatrix2x2(struct cmp_matrix2x2, uint32_t):");
    for (uint32_t i = 0; i < compact; i++) {
        passed += testCmp(cmpCases[i]);
    }
    printf("\n");

    printf("Passed %d out of %d tests: %d%%\n", passed, all, (int) (((double) passed / all) * 100));
    return 0;
}
