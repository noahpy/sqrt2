
#pragma once

#include <stdint.h>
#include <stdlib.h>

struct bignum {
    uint32_t *digits;
    size_t size;
    size_t fracSize;
};

// Helper functions

/**
 * @brief Converts an int to a bignum
 * 
 * @param uint32_t n: The int to convert
 * @return struct bignum The bignum representation of the int 
 */
struct bignum bignumOfInt(uint32_t n); 

/**
 * @brief Allocated an array of uint_32t of size number
 *
 * @param size_t number: The size of the array to allocate
 * @return uint32_t* The allocated array
 */
uint32_t *allocateDigits(size_t number);

/**
 * @brief Compares two bignums
 * 
 * @param struct bignum a: The first bignum to compare
 * @param struct bignum b: The second bignum to compare
 * @return int: 1 if a > b, 0 if a == b, -1 if a < b
 */
int compareBigNum(struct bignum a, struct bignum b);

// Multiplication functions

/**
 * @brief Multiplies two bignums
 *
 * @param struct bignum a: The first bignum to multiply
 * @param struct bignum b: The second bignum to multiply
 * @return struct bignum: The product of the two bignums
 */
struct bignum multiplicationBignum(struct bignum a, struct bignum b);

/**
 * @brief Multiplies two bignums using Karazuba's algorithm
 *
 * @param struct bignum a: The first bignum to multiply
 * @param struct bignum b: The second bignum to multiply
 * @return struct bignum: The product of the two bignums
 */
struct bignum karazubaMultiplication(struct bignum, struct bignum);

/**
 * @brief Multiplies two bignums using SIMD
 *
 * @param struct bignum a: The first bignum to multiply
 * @param struct bignum b: The second bignum to multiply
 * @return struct bignum: The product of the two bignums
 */
struct bignum multiplicationBignumSIMD(struct bignum a, struct bignum b);

// division functions

/**
 * @brief Divides two bignums and writes the result to the first parameter
 * using the newton-raphson method
 *
 * @param struct bignum a: The first bignum to divide
 * @param struct bignum b: The second bignum to divide
 * @param size_t fracSize: The number of bits after the decimal point to be computed
 */
void divisionBignum(struct bignum *a, struct bignum *b, size_t fracSize, void subtract(struct bignum *, struct bignum));

/**
 * @brief Divides two bignums and writes the result to the first parameter
 *
 * @param struct bignum a: The first bignum to divide
 * @param struct bignum b: The second bignum to divide
 * @param size_t fracSize: The number of bits after the decimal point to be computed
 */
void divisionBignum2(struct bignum *a, struct bignum *b, size_t fracSize, void subtract(struct bignum*, struct bignum));

/**
 * @brief Adds two bignums and writes the result to the first parameter
 *
 * @param struct bignum a: The first bignum to add
 * @param struct bignum b: The second bignum to add
 */
void additionBignum(struct bignum *a, struct bignum b);

/**
 * @brief Adds two bignums using SIMD and writes the result to the first parameter
 *
 * @param struct bignum a: The first bignum to add
 * @param struct bignum b: The second bignum to add
 */
void additionBignumSIMD(struct bignum *a, struct bignum b);

/**
 * @brief Subtracts two bignums and writes the result to the first parameter
 *
 * @param struct bignum a: The first bignum to subtract
 * @param struct bignum b: The second bignum to subtract
 */
void subtractionBignum(struct bignum *a, struct bignum b);

/**
 * @brief Subtracts two bignums using SIMD and writes the result to the first parameter
 *
 * @param struct bignum a: The first bignum to subtract
 * @param struct bignum b: The second bignum to subtract
 */
void subtractionBignumSIMD(struct bignum *a, struct bignum b);


/**
 * @brief Shifts a bignum, which is 2. to the left by n bits
 *
 * @param struct bignum a: The bignum to shift
 * @param size_t n: The number of bits to shift by
 * @return struct bignum: The shifted bignum
 */
struct bignum shiftLeftConstant(struct bignum a, size_t number);

/**
 * @brief Shifts a bignum to the left by n bits
 *
 * @param struct bignum a: The bignum to shift
 * @param size_t n: The number of bits to shift by
 * @return struct bignum: The shifted bignum
 */
struct bignum shiftLeft(struct bignum a, size_t n);

/**
 * @brief Shifts a bignum to the right by n bits, in place
 *
 * @param struct bignum a: The bignum to shift
 * @param size_t n: The number of bits to shift by
 */
void shiftLeftInplace(struct bignum* a, size_t n);

/**
 * @brief Shifts a bignum to the right by n bits
 *
 * @param struct bignum a: The bignum to shift
 * @param size_t n: The number of bits to shift by
 * @return struct bignum: The shifted bignum
 */
void shiftRight(struct bignum *a, size_t n);

