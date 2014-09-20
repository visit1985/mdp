#                                                                                                                           
#                                                                                                                             1 # Makefile
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

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

CFLAGS   = -O3 -Wall -I../include
ifeq ($(DEBUG),1)
CFLAGS  := -O0 -Wall -g -I../include
endif

OSTYPE  := $(shell uname -o)
ifeq (Windows_NT, $(OS))
ifneq (Cygwin, $(OSTYPE))
CFLAGS  += -DWIN32=1
endif
endif

all: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

