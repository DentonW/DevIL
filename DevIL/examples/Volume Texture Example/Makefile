# Unix Makefile for Simple DevIL application

CC=gcc
OBJECTS=3dtest.o
LIBS=-lIL -lILU -lILUT -lGL -lGLU -lglut -lXext -lXi -lXmu

all: DevIL_test3d

DevIL_test3d: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_test3d

3dtest.o: 3dtest.c
	$(CC) -Wall -o 3dtest.o -c 3dtest.c

clean:
	rm -f $(OBJECTS) DevIL_test3d

