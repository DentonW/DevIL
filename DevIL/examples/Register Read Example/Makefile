# Unix Makefile for Simple DevIL application

CC=gcc
OBJECTS=readtest.o
LIBS=-lIL -lILU

all: DevIL_testread

DevIL_testread: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testread

readtest.o: readtest.c
	$(CC) -Wall -o readtest.o -c readtest.c

clean:
	rm -f $(OBJECTS) DevIL_testread

