# Unix Makefile

CC       = gcc
CFLAGS   = -Wall
LIBS     = -lX11 -lm -lGL -lGLU -lSDL -lIL -lILU -lILUT -lXext -lXi -lXmu

SRC     = benchmark.c
OBJECTS = $(SRC:%.c=.objects/%.o)
DEPENDS = $(SRC:%.c=.depends/%.d)
TARGET  = benchmark

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $@ $^

.objects/%.o: %.c
	@@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.depends/%.d: %.c
	@@if [ ! -d $(@D) ]; then mkdir -p $(@D); fi
	$(CC) $(INCLUDES) -MM -MG $< -MT '.objects/$(@F:%.d=%.o)' > $@

clean:
	rm -rf $(DEPENDS) $(OBJECTS) $(TARGET)

-include $(DEPENDS)

