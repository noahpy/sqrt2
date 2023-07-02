CFLAGS=-O0 -std=c17 -Wall -Wextra -Wpedantic -g

all: test
test: test_decimal_place_converter.c ../decimal_place_converter.c
		$(CC) $(CFLAGS) -o $@ $^ -lm
clean:
		rm -f test
