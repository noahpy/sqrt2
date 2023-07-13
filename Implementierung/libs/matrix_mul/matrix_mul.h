#include "../big_num/big_num.h"

struct matrix2x2 {
    struct bignum a11;
    struct bignum a12;
    struct bignum a21;
    struct bignum a22;
};

struct cmp_matrix2x2 {
    struct bignum xm1;
    struct bignum x;
    struct bignum xp1;
};

struct matrix2x2 mulMatrix2x2(struct matrix2x2 a, struct matrix2x2 b, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));

void free2x2(struct matrix2x2);

void freeCmp2x2(struct cmp_matrix2x2);

struct cmp_matrix2x2 mulCmpMatrix2x2(struct cmp_matrix2x2 a, struct cmp_matrix2x2 b, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));
