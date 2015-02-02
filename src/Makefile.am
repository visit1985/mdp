AM_CFLAGS = @CFLAGS@ -3 -Wall
AM_LDFLAGS = -lncurses
# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
#
bin_PROGRAMS = mdp
mdp_SOURCES = cstack.c  cstring.c  main.c  markdown.c  parser.c  url.c  viewer.c
mdp_CFLAGS = -I../include -I/usr/include/ncursesw
mdp_CPPFLAGS = -I../include 
mdp_CPPFLAGS += -I/usr/include/ncursesw
mdp_LDFLAGS = -lncurses
