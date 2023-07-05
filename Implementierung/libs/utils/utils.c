
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../big_num/big_num.h"

void reverse_string(char* string, size_t len){
    for(size_t i = 0; i < len / 2; i++){
        char temp = string[i];
        string[i] = string[len - i - 1];
        string[len - i - 1] = temp;
    }
}

char get_hex_value(char hex){
    if(hex >= '0' && hex <= '9'){
        return hex - '0';
    }
    if(hex >= 'A' && hex <= 'F'){
        return hex - 'A' + 10;
    }
    if(hex >= 'a' && hex <= 'f'){
        return hex - 'a' + 10;
    }
    fprintf(stderr, "Error: Invalid hex character\n");
    exit(EXIT_FAILURE);
}

/**
 * @brief Converts a hex string to a decimal string
 * 
 * @param hex The hex string
 * @param len The length of the hex string
 * @param fraction_len The length of the fraction part of the resulting decimal string,
 *        which is equal to the number of bits after the decimal point
 * @return char* The decimal string
 */
char* hex_to_dec(char* hex, size_t len, size_t fraction_len){
    // determine the length of the decimal string with magic number 1.204119983 (ln(16) / ln(10))
    size_t string_len;
    size_t fraction = (size_t) ceil(((double) len) * 0.204119983);
    if(__builtin_add_overflow(len, fraction, &string_len)){
        fprintf(stderr, "Error: Could not respresent the length of the decimal string\n");
        exit(EXIT_FAILURE);
    }
    // allocate memory for the decimal string (with extra space for dot, zero and terminating null byte)
    char* result = malloc(string_len + 3);
    if(result == NULL){
        fprintf(stderr, "Error: Could not allocate memory for the decimal string\n");
        exit(EXIT_FAILURE);
    }
    // convert the hex string to decimal
    size_t result_len = 0;
    for(size_t i = 0; i < len; i++){
        int carry = get_hex_value(hex[i]);
        for(size_t j = 0; j < result_len; j++){
            int digit = result[j] * 16 + carry;
            result[j] = (char) (digit % 10);
            carry = digit / 10;
        }
        while(carry){
            result[result_len++] = (char) (carry % 10);
            carry /= 10;
        }
    }
    // format the result string
    reverse_string(result, result_len);
    for(size_t i = 0; i < result_len; i++){
        result[i] += '0';
    }

    if(result_len < fraction_len){
        fprintf(stderr, "Error: The fraction length is greater than the decimal length\n");
        exit(EXIT_FAILURE);
    }
    size_t offset = (result_len - fraction_len) ? 0 : 1;

    // add dot, zero and remove trailing zeros
    if(fraction_len){
        bool trailing = true;
        size_t index = result_len - 1;
        while(index >= result_len - fraction_len && index < result_len){
            if(result[index] != '0' || !trailing){
                if(trailing) result[index + 2 + offset] = '\0';
                result[index + 1 + offset] = result[index];
                trailing = false;
            }
            index--;
        }
        result[index + 1 + offset] = '.';
        if(offset){
            result[0] = '0';
        }
    }

    // add terminating null byte 
    if(!fraction_len) result[result_len] = '\0';
    return result;
}


char* bin_to_hex(struct bignum num){
    size_t string_len;
    if(__builtin_umull_overflow(num.size, 8, &string_len)){
        fprintf(stderr, "Error: Could not represent the length of the hex string\n");
        exit(EXIT_FAILURE);
    }
    char* result = malloc(string_len + 1);
    if(result == NULL){
        fprintf(stderr, "Error: Could not allocate memory for the hex string\n");
        exit(EXIT_FAILURE);
    }
    char* current = result;
    for (int i = num.size - 1; i > -1; i--){
        sprintf(current, "%08x", num.digits[i]);
        current += 8;
    }
    // add terminating null byte
    result[string_len] = '\0';
    return result;
}


void print_bignum_dec(struct bignum *num, struct bignum multiply(struct bignum, struct bignum)){
    struct bignum result = *num;
    if(num->fracSize){
        size_t frac_hex = num->fracSize / 4;
        struct bignum multiplier = bignumOfInt(10000);
        for(size_t i = 0; i < frac_hex; i++){
            struct bignum new_result = multiply(result, multiplier);
            if(i > 0) free(result.digits);
            result = new_result;
        }
        free(multiplier.digits);
        // remove fraction zeros
        shiftRight(&result, result.fracSize);  
    }
    char* hex_string = bin_to_hex(result);
    char* dec_string = hex_to_dec(hex_string, strlen(hex_string), num->fracSize);
    
    printf("%s\n", dec_string);
    free(hex_string);
    free(dec_string);
    if(num->fracSize) free(result.digits);
}

void print_bignum_hex(struct bignum *num){
    char* hex_string;
    // shift to left to fit in to hex digits
    size_t shift = 4 - (num->fracSize % 4);
    if(shift < 4){
        // TODO: replace with real left shift
        int lt[] = {2, 4, 8};
        struct bignum multiplier = bignumOfInt(lt[shift-1]);
        struct bignum result = multiplicationBignum(*num, multiplier);
        hex_string = bin_to_hex(result);
        free(multiplier.digits);
        free(result.digits);
    }
    else{
        shift = 0;
        hex_string = bin_to_hex(*num);
    }
    // remove leading zeros, add dot if needed
    size_t len = strlen(hex_string);
    size_t lead_offset = 0;
    while(hex_string[lead_offset] == '0'){
        lead_offset++;
    }
    if(lead_offset + (num->fracSize + shift)/4 == len){
        lead_offset--;
        hex_string[0] = '0';
    }
    size_t i = lead_offset;
    for (; i < len; i++){
        if(i == len - (num->fracSize + shift) / 4){
            hex_string[i - lead_offset] = '.';
            lead_offset--;
        }
        hex_string[i - lead_offset] = hex_string[i];
    }
    hex_string[i - lead_offset] = '\0';
    printf("0x%s\n", hex_string);
    free(hex_string);
}
