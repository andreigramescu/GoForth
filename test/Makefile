CC		 = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -g
BUILD  = libtest.a

.PHONY: all clean

all: libtest.a
	ar rcs libtest.a test.o

clean:
	/bin/rm -rf *.o *.a

libtest.a: test.o
test.o: test.h