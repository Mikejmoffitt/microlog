CC := clang
CFLAGS := -Iinc -Wall -O2
LDFLAGS := ``
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLE := microlog

.PHONY: all clean

all: $(EXECUTABLE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@  $(LDFLAGS) 

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

