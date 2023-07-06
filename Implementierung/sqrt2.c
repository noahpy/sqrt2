
#include <stdio.h>
#include "libs/big_num/big_num.h"
#include "libs/mat_fast_exp.h"
#include "libs/utils/utils.h"

struct bignum sqrt2(size_t s){
    struct cmp_matrix2x2 m = {
        bignumOfInt(0),
        bignumOfInt(1),
        bignumOfInt(2)
    };

    struct cmp_matrix2x2 r = powCmpMatrix2x2(m, 100, multiplicationBignum);

    /* print_bignum_dec(&r.x, multiplicationBignum); */
    /* print_bignum_dec(&r.xm1, multiplicationBignum); */
    /* print_bignum_dec(&r.xp1, multiplicationBignum); */
    /*  */
    divisionBignum(&r.x, &r.xp1, 1000);
    print_bignum_dec(&r.x, multiplicationBignum);
}
