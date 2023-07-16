#include "matrix_mul/matrix_mul.h"


/**
 * @brief Fast exponentiation of a matrix
 *
 * @param a Matrix to exponentiate
 * @param n Exponent
 * @param multiply Function to multiply two bignums
 * @param addition Function to add two bignums
 * @return struct matrix2x2
 */
struct matrix2x2 powMatrix2x2(struct matrix2x2 a, uint32_t n, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));

/**
 * @brief Fast exponentiation of a compact matrix
 *
 * @param a Matrix to exponentiate
 * @param n Exponent
 * @param multiply Function to multiply two bignums
 * @param addition Function to add two bignums
 * @return struct cmp_matrix2x2
 */
struct cmp_matrix2x2 powCmpMatrix2x2(struct cmp_matrix2x2 a, uint32_t n, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));

struct cmp_matrix2x2 sequence(size_t n);