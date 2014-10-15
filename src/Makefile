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

UNAME_S := $(shell uname -s 2>/dev/null || echo not)

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
CFLAGS  = -O3 -Wall -I../include $(shell pkg-config --cflags ncursesw)

ifeq ($(DEBUG),1)
	CFLAGS := -O0 -Wall -g -I../include
endif

ifeq ($(OS),Windows_NT)
	ifeq (,$(findstring CYGWIN,$(UNAME_S)))
		CFLAGS += -DWIN32
	endif
endif

ifeq ($(UNAME_S),Linux)
	LSB_RELEASE := $(shell lsb_release -si 2>/dev/null || echo not)
	ifneq ($(filter $(LSB_RELEASE),Debian Ubuntu LinuxMint),)
		CFLAGS += -I/usr/include/ncursesw
	endif
endif

all: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

