#
# Makefile
# Copyright (C) 2014 Michael Goehler
#
# This file is part of mdp.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#


CC = gcc
CFLAGS   = -O3 -Wall
LDFLAGS  = -s
LDLIBS   = -lncurses
DESTDIR ?= /usr/bin

TARGET = mdp
SRCFOLDER = src
INCFOLDER = include
OBJFOLDER = obj
BINFOLDER = bin

SOURCES = $(wildcard $(SRCFOLDER)/*.c)
OBJECTS = $(patsubst %.c, $(OBJFOLDER)/%.o, $(notdir $(wildcard $(SRCFOLDER)/*.c)))

ifeq ($(DEBUG),1)
CFLAGS  := -Wall -g -O0
LDFLAGS :=
endif

all: mdp

mdp: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(BINFOLDER)/$(TARGET)

$(OBJFOLDER)/%.o: $(SRCFOLDER)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCFOLDER)

dirs:
	@mkdir -p bin obj
clean:
	$(RM) $(OBJECTS) mdp

install: mdp
	install -Dm755 mdp $(PREFIX)$(DESTDIR)/mdp

uninstall:
	rm -f $(PREFIX)$(DESTDIR)/mdp

.PHONY: all clean install uninstall

