
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
void goldschmidt(struct bignum *a, struct bignum b, size_t fracSize);
void additionBignum(struct bignum *a, struct bignum b);
void subtractionBignum(struct bignum *a, struct bignum b);
