# Unix Makefile

CC=gcc
OBJECTS=gltest.o
LIBS=-lX11 -lm -lGL -lGLU -lglut -lIL -lILU -lILUT -lXext -lXi -lXmu

all: DevIL_testGL

DevIL_testGL: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testGL

gltest.o: gltest.c gltest.h
	$(CC) -I$(HOME)/usr/DevIL/include -Wall -o gltest.o -c gltest.c

clean:
	rm $(OBJECTS) DevIL_testGL
