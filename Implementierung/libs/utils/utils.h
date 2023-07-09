
#include "../big_num/big_num.h"
#include <stddef.h>
#include <stdbool.h>

char* hex_to_dec(char* hex, size_t hex_size);

void print_bignum_dec(struct bignum* num, struct bignum multiply(struct bignum, struct bignum), bool isSqrt2);

void print_bignum_hex(struct bignum *num);

char* bin_to_hex(struct bignum num);
