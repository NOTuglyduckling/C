CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -Werror

all: ex1 ex2 ex3

ex1: EX1.c
	$(CC) $(CFLAGS) $< -o $@

ex2: EX2.c
	$(CC) $(CFLAGS) $< -o $@

ex3: EX3.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ex1 ex2 ex3
