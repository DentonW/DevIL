# Unix Makefile

CC=gcc
OBJECTS=allegtest.o
LIBS=-lallegro -lIL -lILU -lILUT

all: DevIL_testalleg

DevIL_testalleg: $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o DevIL_testalleg

allegtest.o: allegtest.c
	$(CC) -Wall -o allegtest.o -c allegtest.c

clean:
	rm $(OBJECTS) DevIL_testalleg
