CFLAGS=-O2 -std=c17 -Wall -Wextra -Wpedantic -D_POSIX_C_SOURCE=199309L -g -lm -msse4.1

all: performance
performance: performance_big_num.c ../big_num.c
		$(CC) $(CFLAGS) -o $@ $^
clean:
		rm -f performance
