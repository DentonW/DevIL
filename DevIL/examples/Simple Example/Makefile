# Unix Makefile for Simple DevIL application

CC=gcc
OBJECTS=simple.o
LIBS=-lIL -lILU

all: DevIL_test

DevIL_test: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_test

simple.o: simple.c
	$(CC) -Wall -o simple.o -c simple.c

clean:
	rm -f $(OBJECTS) DevIL_test

