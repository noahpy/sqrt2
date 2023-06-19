#include "../matrix_mul.h"
#include <stdio.h>

// Convert to bignum
struct bignum ctb(uint32_t i) {
    uint32_t *digits = NULL;
    if (!(digits = malloc(sizeof(uint32_t)))) {
        fprintf(stderr, "Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }
    *digits = i;
    return (struct bignum) {1, digits};
}


// Compares a 4x4 matrix to a uint32_t array
int compareTo(struct matrix4x4 a, uint32_t arr[]) {
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

// Compares a compact 4x4 matrix to a uint32_t array
int compareCmpTo(struct cmp_matrix4x4 a, uint32_t arr[]) {
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
 * Run test with an array of 12 uint32
 * [0...3] -> First 4x4 matrix
 * [4...7] -> Second 4x4 matrix
 * [8..11] -> Expected 4x4 result matrix
 *
 * */
int test(uint32_t arr[]) {
    printf("[%d", arr[0]);
    for (int i = 1; i < 12; i++) {
        printf(", %d", arr[i]);
    }

    printf("%s", "]: ");

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
    first = mulMatrix4x4(first, second);

    // Check for correct result
    int result = compareTo(first, (arr + 8));
    free4x4(first);
    free4x4(second);
    if (result) {
        printf("%s\n", "Successful");
    }

    return result;
}

/*
 *
 * Run test with an array of 12 uint32
 * [0...3] -> First 4x4 matrix
 * [4...7] -> Second 4x4 matrix
 * [8..11] -> Expected 4x4 result matrix
 *
 * */
int testCmp(uint32_t arr[]) {
    printf("[%d", arr[0]);
    for (int i = 1; i < 9; i++) {
        printf(", %d", arr[i]);
    }

    printf("%s", "]: ");

    struct cmp_matrix4x4 first = {
            .xm1 = ctb(arr[0]),
            .x = ctb(arr[1]),
            .xp1 = ctb(arr[2]),
    };
    struct cmp_matrix4x4 second = {
            .xm1 = ctb(arr[3]),
            .x = ctb(arr[4]),
            .xp1 = ctb(arr[5]),
    };

    // Multiply first in place
    first = mulCmpMatrix4x4(first, second);

    // Check for correct result
    int result = compareCmpTo(first, (arr + 6));
    freeCmp4x4(first);
    freeCmp4x4(second);
    if (result) {
        printf("%s\n", "Successful");
    }

    return result;
}

int main(void) {
    uint32_t cases[][12] = {
            {0,      1,      1,      2,      0,      1,      1,      2,      1,           2,           2,          5},
            {1,      2,      3,      4,      5,      6,      7,      8,      19,          22,          43,         50},
            {28363,  15351,  -29458, -27900, -23477, 4115,   -26042, 17066,  -1065648893, 378693911,   1418157266, -597361070},
            {-11230, 2439,   29702,  -2829,  5452,   -1090,  11646,  6739,   -32821366,   28677121,    128988770,  -51439811},
            {-25152, -29088, 27720,  25339,  -5788,  -702,   32247,  3297,   -792420960,  -78246432,   656663373,  64083243},
            {16979,  29955,  -25407, -25747, -19354, 7395,   29320,  32623,  549669034,   1102781670,  -263174962, -1027829146},
            {-31745, -25390, 24431,  1849,   -4469,  20956,  17641,  31311,  -306036585,  -1460234510, -76563930,  569870075},
            {30339,  -7850,  5236,   16014,  17020,  -8518,  -7111,  -10696, 572191130,   -174464002,  -24758834,  -215885992},
            {-25357, -29349, -7726,  14089,  30577,  25938,  13918,  -1700,  -1183820371, -607816566,  -40147200,  -224348288},
            {11989,  -6665,  16527,  4082,   -25808, -29137, -27073, 10953,  -128970567,  -422325238,  -537040802, -436837053},
            {32330,  2834,   -268,   18604,  26930,  2465,   -7826,  -14723, 848468016,   37968468,    -152812144, -274567312},
            {-13220, -32054, -25630, -20638, -21006, -4409,  3857,   -4956,  154067042,   217146604,   458783014,  215284598},
            {-22096, 13871,  15772,  -10713, 8627,   26960,  -23030, -13962, -510071322,  -789375062,  382785434,  574788026},
            {-156,   -9662,  32286,  -21353, -9358,  -17298, 29526,  -30152, -283820364,  294027112,   -932601066, 85352428},
            {15627,  24897,  -14931, 20191,  -18292, 14003,  13013,  -23223, 38135577,    -359358150,  535863335,  -677974386},
            {4468,   1723,   4863,   -28132, 25067,  32387,  -18264, 26211,  80530484,    189866669,   635703669,  -579869871},
            {-15594, 13502,  -14631, 15620,  10568,  18759,  12998,  811,    10701604,    -281577724,  48408352,   -261795109},
            {-10454, 6798,   -2948,  24520,  -11283, 18297,  31084,  -16773, 329261514,   -305299692,  795441964,  -465213516},
            {13918,  32396,  -20197, 13645,  -30426, -9424,  29233,  425,    523563200,   -117394932,  1013398207, 196135653},
            {-7656,  -15350, 24837,  370,    -22563, -30049, -25203, -14280, 559608378,   449253144,   -569722341, -751610613}
    };

    uint32_t cmpCases[][9] = {
            /* a = | 0 1 |
             *     | 1 2 |
             * */
            // a^1 * a^1
            {0, 1, 2, 0, 1, 2, 1, 2, 5},
            // a^4 * a^5
            {5, 12, 29, 12, 29, 70, 408, 985, 2378},
            // a^6 * a^12
            {29, 70, 169, 5741, 13860, 33461, 1136689, 2744210, 6625109},
            // a^2 * a^18
            {1, 2, 5, 1136689, 2744210, 6625109, 6625109, 15994428, 38613965},
            // a^8 * a^4
            {169, 408, 985, 5, 12, 29, 5741, 13860, 33461},
    };

    uint32_t normal = (sizeof(cases) / (sizeof(uint32_t) * 12));
    uint32_t passed = 0;
    uint32_t all = normal;

    printf("%s\n", "Testing mulMatrix4x4(struct matrix4x4, struct matrix4x4):");
    for (uint32_t i = 0; i < normal; i++) {
        passed += test(cases[i]);
    }
    printf("\n");

    uint32_t compact = (sizeof(cmpCases) / (sizeof(uint32_t) * 9));
    all += compact;

    printf("%s\n", "Testing mulCmpMatrix4x4(struct cmp_matrix4x4, struct cmp_matrix4x4):");
    for (uint32_t i = 0; i < compact; i++) {
        passed += testCmp(cmpCases[i]);
    }
    printf("\n");

    printf("Passed %d out of %d tests: %d%%\n", passed, all, (int) (((double) passed / all) * 100));
    return 0;
}
