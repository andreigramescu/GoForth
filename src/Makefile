CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g\
		 -I./forthstack\
		 -I./parser\
		 -I./trie\
		 -I./ReturnStack\
		 -I./forthmachine
	
BUILD = goforth 

.PHONY: all clean

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o
	cd forthstack; make clean
	cd parser; make clean
	cd trie; make clean
	cd ReturnStack; make clean
	cd forthmachine; make clean

goforth: main
	mv main goforth

main: main.o forthstack/forthstack.o parser/parser.o trie/trie.o ReturnStack/ReturnStack.o forthmachine/forthmachine.o forthmachine/nativewords.o

main.o: main.c

forthmachine/forthmachine.o:
	cd forthmachine; make

parser/parser.o:
	cd parser; make

trie/trie.o:
	cd trie; make

forthstack/forthstack.o:
	cd forthstack; make

forthstack/nativewords.o:
	cd forthmachine; make

ReturnStack/ReturnStack.o:
	cd ReturnStack; make


