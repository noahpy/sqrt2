
#include "../utils.h"
#include <stdint.h>

int main(){
    // number: 74857857827867872 hex: 109F2D3_387A0CE0
    uint32_t* digits = malloc(sizeof(uint32_t) * 2);
    digits[0] = 0x387a0ce0;
    digits[1] = 0x109F2D3;
    struct bignum num = {digits, 2, 0};
    char* buffer = malloc(sizeof(char) * 100);
    print_bignum_dec(num, buffer, 100);
    free(digits);
    free(buffer);
}
