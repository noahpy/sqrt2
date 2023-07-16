#include "../big_num/big_num.h"

struct matrix2x2 {
    struct bignum a11;
    struct bignum a12;
    struct bignum a21;
    struct bignum a22;
};

struct cmp_matrix2x2 {
    struct bignum xm1;
    struct bignum x;
    struct bignum xp1;
};

/**
 * @brief Multiplies two 2x2 matrices
 *
 * @param a First matrix
 * @param b Second matrix
 * @param multiply Function to multiply two bignums
 * @param addition Function to add two bignums
 * @return struct matrix2x2 Result of multiplication
 */
struct matrix2x2 mulMatrix2x2(struct matrix2x2 a, struct matrix2x2 b, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));

/**
 * @brief frees memory allocated for matrix
 *
 * @param a Matrix to free
 */
void free2x2(struct matrix2x2);


/**
 * @brief Multiplies two symmetric (compact) 2x2 matrices
 *
 * @param a First matrix
 * @param b Second matrix
 * @param multiply Function to multiply two bignums
 * @param addition Function to add two bignums
 * @return struct cmp_matrix2x2 Result of multiplication
 */
struct cmp_matrix2x2 mulCmpMatrix2x2(struct cmp_matrix2x2 a, struct cmp_matrix2x2 b, struct bignum multiply(struct bignum, struct bignum), void addition(struct bignum*, struct bignum));


void freeCmp2x2(struct cmp_matrix2x2);
