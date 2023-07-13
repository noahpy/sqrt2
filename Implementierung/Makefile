programs := main.c rahmenprogramm.c sqrt2.c libs/big_num/big_num.c libs/decimal_place_converter/decimal_place_converter.c libs/matrix_mul/matrix_mul.c libs/mat_fast_exp.c libs/utils/utils.c -g -lm -msse4.1

all:
	gcc ${programs} -O3 -o main -Wall -Wextra -Wpedantic -std=c17
clear:
	rm -f main
