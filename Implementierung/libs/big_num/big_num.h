
#pragma once

#include <stdint.h>
#include <stdlib.h>

struct bignum {
    uint32_t *digits;
    size_t size;
    size_t fracSize;
};

// Helper functions
struct bignum bignumOfInt(uint32_t n); 
uint32_t *allocateDigits(size_t number);
int compareBigNum(struct bignum a, struct bignum b);

// Multiplication functions
struct bignum multiplicationBignum(struct bignum a, struct bignum b);
struct bignum karazubaMultiplication(struct bignum, struct bignum);
struct bignum multiplicationBignumSIMD(struct bignum a, struct bignum b);

// division functions
void divisionBignum(struct bignum *a, struct bignum *b, size_t fracSize);
void divisionBignum2(struct bignum *a, struct bignum *b, size_t fracSize, void subtract(struct bignum*, struct bignum));

void additionBignum(struct bignum *a, struct bignum b);
void additionBignumSIMD(struct bignum *a, struct bignum b);

void subtractionBignum(struct bignum *a, struct bignum b);
void subtractionBignumSIMD(struct bignum *a, struct bignum b);

struct bignum shiftLeftConstant(struct bignum a, size_t n);
struct bignum shiftLeft(struct bignum a, size_t n);
void shiftLeftInplace(struct bignum* a, size_t n);
void shiftRight(struct bignum *a, size_t n);

