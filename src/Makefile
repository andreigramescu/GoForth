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

main.o: forthmachine/forthmachine.h main.c

forthstack/forthstack.o: forthstack/forthstack.h forthstack/forthstack.c

parser/parser.o: parser/parser.h parser/parser.c

trie/trie.o: trie/trie.h trie/trie.c

ReturnStack/ReturnStack.o: ReturnStack/ReturnStack.h ReturnStack/ReturnStack.c

forthmachine/forthmachine.o: forthmachine/forthmachine.h forthmachine/forthmachine.c

forthmachine/nativewords.o: forthmachine/nativewords.h forthmachine/nativewords.c
