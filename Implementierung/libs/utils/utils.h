
#include "../big_num/big_num.h"
#include <stddef.h>

/**
 * @brief Converts a hex string to a decimal string
 *
 * @param hex The hex string
 * @param len The length of the hex string
 * @param fraction_len The length of the fraction part of the resulting decimal
 * string, which is equal to the number of bits after the decimal point
 * @return char* The decimal string
 */
char* hex_to_dec(char* hex, size_t hex_size);

/**
 * @brief Print a bignum in decimal format
 * 
 * @param num The bignum to print
 * @param multiply The function to multiply the bignum by
 * @param isSqrt2 Greater than zero if num is sqrt(2), then it also represents the 
 * number of decimal digits after the decimal point to print
 */
void print_bignum_dec(struct bignum* num, struct bignum multiply(struct bignum, struct bignum), long isSqrt2);

/**
 * @brief Print a bignum in hexadecimal format
 *
 * @param num The bignum to print
 * @param isSqrt2 Greater than zero if num is sqrt(2), then it also represents the
 * number of decimal digits after the decimal point to print
 */
void print_bignum_hex(struct bignum *num, long isSqrt2);

/**
 * @brief Produces a hexademical string from a bignum
 *
 * @param num The bignum to convert
 * @return char* The hexademical string
 */
char* bin_to_hex(struct bignum num);
