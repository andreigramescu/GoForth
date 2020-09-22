CC     = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g -I./../../test/
LDLIBS = -L./../../test/ -ltest

.PHONY: all clean test test-full test_utils

all: parser.o 

clean:
	/bin/rm -rf *.o parser_tests
	cd ./../../test; /bin/rm *.o *.a

test: test_utils parser_tests
	./parser_tests

test-full: test_utils parser_tests
	valgrind ./parser_tests

test_utils:
	cd ./../../test; make

parser.o: parser.h

parser_tests: parser_tests.o parser.o 

parser_tests.o: parser_tests.c
