# Unix Makefile for Simple DevIL application

CC=gcc
OBJECTS=sdl_test.o
LIBS=-lIL -lILU

all: DevIL_testsdl

DevIL_testsdl: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testsdl

sdl_test.o: sdl_test.c
	$(CC) -Wall -o sdl_test.o -c sdl_test.c

clean:
	rm -f $(OBJECTS) DevIL_testsdl

