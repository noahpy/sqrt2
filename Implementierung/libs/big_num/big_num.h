
// #include <cstdint>
#include <stdint.h>
#include <stdlib.h>

struct bignum {
    size_t size;
    uint32_t *digits;
};

struct bignum multiplicationBignum(struct bignum a, struct bignum b);
