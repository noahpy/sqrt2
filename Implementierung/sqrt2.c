
#include <stdbool.h>
#include <stdio.h>
#include "libs/big_num/big_num.h"
#include "libs/mat_fast_exp.h"
#include "libs/utils/utils.h"

struct bignum sqrt2(size_t s){
    for (int i = 1; i < 300; i++) {
        struct cmp_matrix2x2 m = {
            bignumOfInt(0),
            bignumOfInt(1),
            bignumOfInt(2)
        };

        struct cmp_matrix2x2 r = powCmpMatrix2x2(m, i, multiplicationBignum);
        divisionBignum(&r.x, &r.xp1, 200);
        print_bignum_dec(&r.x, multiplicationBignum, true);
        freeCmp2x2(r);
    }
}
