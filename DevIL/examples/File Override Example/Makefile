# Unix Makefile for Simple DevIL application

CC=gcc
OBJECTS=iotest.o
LIBS=-lIL -lILU

all: DevIL_testio

DevIL_testio: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testio

iotest.o: iotest.c
	$(CC) -Wall -o iotest.o -c iotest.c

clean:
	rm -f $(OBJECTS) DevIL_testio

