CFLAGS=-O0 -std=c17 -Wall -Wextra -Wpedantic -g -lm -msse4.1

all: test
test: test_mat_fast_exp.c ../mat_fast_exp.c ../matrix_mul/matrix_mul.c ../big_num/big_num.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f test
