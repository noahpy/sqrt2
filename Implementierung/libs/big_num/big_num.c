
#include <stdint.h>
#include <stdlib.h>

struct bignum {
    size_t size;
    uint64_t *digits;
};
