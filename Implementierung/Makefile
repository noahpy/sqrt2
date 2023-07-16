programs := main.c rahmenprogramm.c sqrt2.c libs/big_num/big_num.c libs/decimal_place_converter/decimal_place_converter.c libs/matrix_mul/matrix_mul.c libs/mat_fast_exp.c libs/utils/utils.c 

all:
	gcc ${programs} -O3 -o main -Wall -Wextra -Wpedantic -Werror -std=c17 -g -lm -msse4.1
clear:
	rm -f main
