
#include "libs/mat_fast_exp.h"

/*Calculates aquare root of 2
 * Params:
 *  - size_t s: required binary precision
 * Returns:
 *  - struct bignum: calculated approximation of sqrt 2*/
struct bignum sqrt2(size_t s);


/*Calculates aquare root of 2
 *Takes use of compact matrix representation
 * Params:
 *  - size_t s: required binary precision
 * Returns:
 *  - struct bignum: calculated approximation of sqrt 2*/
struct bignum sqrt2_V1(size_t s);


/*Calculates aquare root of 2
 *Takes use of compact matrix representation, Karazuba multiplication
 *and SIMD Arithmetics.
 * Params:
 *  - size_t s: required binary precision
 * Returns:
 *  - struct bignum: calculated approximation of sqrt 2*/
struct bignum sqrt2_V2(size_t s);


/*Calculates aquare root of 2
 *Takes use of compact matrix representation, SIMD multiplication
 *and other SIMD Arithmetics.
 * Params:
 *  - size_t s: required binary precision
 * Returns:
 *  - struct bignum: calculated approximation of sqrt 2*/
struct bignum sqrt2_V3(size_t s);
