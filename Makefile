#
# Makefile
# Copyright (C) 2015 Michael Goehler
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

UNAME_S := $(shell uname -s 2>/dev/null || echo not)

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET  = mdp
DESTDIR =
PREFIX  = /usr/local

CURSES  = ncursesw
LDFLAGS ?= -s

ifeq (Windows_NT,$(OS))
	ifeq (,$(findstring CYGWIN,$(UNAME_S)))
		CURSES := pdcurses
	endif
endif

ifeq ($(UNAME_S),Darwin)
	CURSES := ncurses
	LDFLAGS :=
endif

ifeq ($(DEBUG),1)
	LDFLAGS :=
endif

LDLIBS   = -l$(CURSES)

all: $(TARGET)

$(TARGET): src
	$(CC) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o $(TARGET)

src:
	$(MAKE) $(MFLAGS) -C src

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET)

install:
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 mdp $(DESTDIR)$(PREFIX)/bin/$(TARGET)

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/$(TARGET)

.PHONY: all clean install src uninstall
