CFLAGS=-O3 -std=c17 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L -lm -msse4.1

all: performance
performance: performance_matrix_mul.c ../matrix_mul.c ../../big_num/big_num.c ../../mat_fast_exp.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f performance
