CFLAGS=-O3 -std=c17 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L

all: performance
performance: performance_matrix_exp.c ../mat_fast_exp.c ../matrix_mul/matrix_mul.c ../big_num/big_num.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f performance
