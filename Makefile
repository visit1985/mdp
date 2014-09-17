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

CFLAGS   = -O3
LDFLAGS  = -s
LDLIBS   = -lncurses
OBJECTS  = cstring.o cstack.o markdown.o parser.o viewer.o mdp.o
DESTDIR ?= /usr/bin

ifeq ($(DEBUG),1)
CFLAGS  := -Wall -g -O0
LDFLAGS :=
endif

all: mdp

mdp: $(OBJECTS)

clean:
	$(RM) $(OBJECTS) mdp

install: mdp
	install -d $(PREFIX)$(DESTDIR)
	install -m 755 mdp $(PREFIX)$(DESTDIR)/mdp

.PHONY: all clean install

