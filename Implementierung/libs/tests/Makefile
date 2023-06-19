CFLAGS=-O0 -std=c17 -Wall -Wextra -Wpedantic

all: test
test: test_mat_fast_exp.c ../mat_fast_exp.c ../matrix_mul/matrix_mul.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f test