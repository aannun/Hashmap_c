CC=clang
CFLAGS=-Wall
BINARY=dictionary
BINARY_TESTS=dictionary_tests

ifeq ($(OS),Windows_NT)
	BINARY:=$(BINARY).exe
	BINARY_TESTS:=$(BINARY_TESTS).exe
endif

dictionary.o: dictionary.c dictionary.h
	$(CC) -c -o $@ $(CFLAGS) $<

tests.o: tests.c
	$(CC) -c -o $@ $(CFLAGS) $^

test: tests.o dictionary.o
	$(CC) -o $(BINARY_TESTS) $^
	$(BINARY_TESTS)