CC     = gcc
CFLAGS = -std=c99 -pedantic -g -Werror -Wall 
BUILD  = forthstack.o

.PHONY: all clean 

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD)
	/bin/rm -f *.o

forthstack.o: forthstack.h
