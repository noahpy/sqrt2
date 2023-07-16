CFLAGS=-O3 -std=c17 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L -g

all: performance
performance: performance_big_num.c ../big_num.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f performance
