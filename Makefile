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

CURSES   = ncursesw
ifeq (Windows_NT, $(OS))
OSTYPE  := $(shell uname -o)
ifneq (Cygwin, $(OSTYPE))
CURSES  := pdcurses
endif
endif

LDFLAGS  = -s
LDLIBS   = -l$(CURSES)
SOURCES  = $(wildcard src/*.c)
OBJECTS  = $(SOURCES:.c=.o)
TARGET   = mdp
DESTDIR  = /usr/bin

ifeq ($(DEBUG),1)
LDFLAGS :=
endif

all: $(TARGET)

$(TARGET): src
	$(CC) $(OBJECTS) $(LDLIBS) $(LDFLAGS) -o $(TARGET)

src:
	$(MAKE) $(MFLAGS) -C src

clean:
	$(MAKE) -C src clean
	$(RM) $(TARGET)

install:
	install -d $(PREFIX)$(DESTDIR)
	install -m 755 mdp $(PREFIX)$(DESTDIR)/$(TARGET)

uninstall:
	$(RM) $(PREFIX)$(DESTDIR)/$(TARGET)

.PHONY: all clean install src uninstall
