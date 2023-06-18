#include "../big_num/big_num.h"

struct matrix4x4 {
    struct bignum a11;
    struct bignum a12;
    struct bignum a21;
    struct bignum a22;
};

struct cmp_matrix4x4 {
    struct bignum xm1;
    struct bignum x;
    struct bignum xp1;
};

struct matrix4x4 mulMatrix4x4 (struct matrix4x4, struct matrix4x4);

struct cmp_matrix4x4 mulCmpMatrix4x4 (struct cmp_matrix4x4, struct cmp_matrix4x4);

struct cmp_matrix4x4 sqCmpMatrix4x4 (struct cmp_matrix4x4);