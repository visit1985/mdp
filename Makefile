CFLAGS=-g -Wall

mdp: mdp.c cstring.o cstack.o markdown.o parser.o viewer.o
	cc $(CFLAGS) -o mdp mdp.c cstring.o cstack.o markdown.o parser.o viewer.o -lncurses

viewer.o: viewer.c
	cc $(CFLAGS) -c viewer.c

parser.o: parser.c
	cc $(CFLAGS) -c parser.c

markdown.o: markdown.c
	cc $(CFLAGS) -c markdown.c

cstack.o: cstack.c
	cc $(CFLAGS) -c cstack.c

cstring.o: cstring.c
	cc $(CFLAGS) -c cstring.c

all: mdp

clean:
	rm -f mdp *.o

.PHONY: test

test:
	$(MAKE) -C test

