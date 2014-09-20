CC = gcc
CFLAGS = -O3 -Wall
LDLIBS   = -lncurses
TARGET = mdp

SRCFOLDER = src
INCFOLDER = ../include

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(notdir $(wildcard *.c)))

ifeq ($(DEBUG),1)
CFLAGS  := -Wall -g -O0
LDFLAGS :=
endif

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o ../$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCFOLDER)

clean:
	@rm -rf *.o ../$(TARGET)
