
// #include <cstdint>
#include <stdint.h>
#include <stdlib.h>

struct bignum {
    uint32_t *digits;
    size_t size;
    size_t fracSize;
};

struct bignum bignumOfInt(uint32_t n); 
struct bignum multiplicationBignum(struct bignum a, struct bignum b);
struct bignum additionBignum(struct bignum a, struct bignum b);
struct bignum subtractionBignum(struct bignum a, struct bignum b);
struct bignum goldschmidt(struct bignum a, struct bignum b, size_t fracSize);
