TARGET = mqtt2mendeleev
CC = gcc
CFLAGS = -Wall -pedantic -std=gnu11 -O2 `pkg-config --cflags libmendeleev`
LIBS = -lmosquitto `pkg-config --libs libmendeleev`
VERSION := $(shell git describe --tags --always)
INSTOPTS= -s
.PHONY: default all clean

# avoid overruling the VERSION
CFLAGS += -DVERSION=\"$(VERSION)\"

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c,%.o,$(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

install: $(TARGET)
	install -vpD -m 0777 $(INSTOPTS) $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
