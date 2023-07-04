
#include <stdint.h>
#include <stdio.h>
#include "../big_num/big_num.h"

void print_bignum_dec(struct bignum *num){
    uint64_t a = 4294967296 * (uint64_t)num->digits[1] + num->digits[0];
    printf("%d\n", num->digits[0]);
}

void print_bignum_hex(struct bignum *num){
    (void) num;
}
