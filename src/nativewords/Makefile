CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g -I../../test/ -I../forthmachine -I../forthstack -I../parser -I../trie
LDLIBS = -L../../test/  -ltest

.PHONY: all clean test test-full test_utils

all: nativewords.o

clean:
	/bin/rm -rf *.o nativewords_tests
	cd ../../test; /bin/rm *.o *.a

test: test_utils nativewords_tests 
	./nativewords_tests

test-full: test_utils nativewords_tests
	valgrind ./nativewords_tests

test_utils: 
	cd ../../test; make

nativewords.o: nativewords.h

forthmachine.o:
	cd ../forthmachine; make

forthstack.o:
	cd ../forthstack; make

parser.o:
	cd ../parser; make

trie.o:
	cd ../trie; make

nativewords_tests: nativewords_tests.o nativewords.o ../forthmachine/forthmachine.o ../forthstack/forthstack.o ../parser/parser.o ../trie/trie.o

