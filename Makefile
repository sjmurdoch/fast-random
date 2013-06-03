CC=gcc
CFLAGS=-Wall

all: fast-random test/tests

fast-random: fast-random.o
	$(CC) $(CFLAGS) -o $@ $^

test/tests: fast-random-test.o test/tests.o test/tinytest.o
	$(CC) $(CFLAGS) -o $@ $^

fast-random-test.o: fast-random.c
	$(CC) $(CFLAGS) -DTEST -DDEBUG -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o test/*.o test/tests fast-random

test: test/tests
	$<

.PHONY: all clean test

