# Unix Makefile

CC=gcc -g
OBJECTS=gltest.o
LIBS=-lm -lGL -lGLU -lglut -lIL -lILU -lILUT

all: DevIL_testGL

DevIL_testGL: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testGL

gltest.o: gltest.c gltest.h
	$(CC) -Wall -o gltest.o -c gltest.c

clean:
	rm $(OBJECTS) DevIL_testGL
