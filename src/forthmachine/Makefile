CC     = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g -I./../../test -I./../trie/ -I./../forthstack/
BUILD  = forthmachine.o

.PHONY: all clean

all: $(BUILD)

clean:
	/bin/rm -f forthmachine.o

forthmachine.o: forthmachine.h
