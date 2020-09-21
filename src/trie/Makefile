CC     = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g -I./../../test/
LDLIBS = -L./../../test/ -ltest
BUILD  = trie.o

.PHONY: all clean test test-full test_utils

all: $(BUILD)

clean:
	/bin/rm -rf *.o trie_tests
	cd ./../../test; /bin/rm *.o *.a

test: test_utils trie_tests
	./trie_tests

test-full: trie_tests test_utils
	valgrind ./trie_tests

test_utils:
	cd ./../../test; make

trie.o: trie.h

trie_tests: trie_tests.o trie.o 

trie_tests.o: trie_tests.c
