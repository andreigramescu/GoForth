CC     = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g -I./../testing/
LDLIBS = -L./../testing/ -ltestlib
BUILD  = trie.o

.PHONY: all clean test test-full test_utils

all: $(BUILD)

clean:
	/bin/rm -rf *.o trie_tests
	cd ./../testing; /bin/rm *.o *.a

test: trie_tests test_utils
	./trie_tests

test-full: trie_tests test_utils
	valgrind ./trie_tests

test_utils:
	cd ./../testing; make

trie.o: trie.h

trie_tests: trie_tests.o trie.o 

trie_tests.o: trie_tests.c
