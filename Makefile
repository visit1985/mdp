tmp: tmp.c cstring.o markdown.o markdown_io.o
	cc -g -o tmp tmp.c cstring.o markdown.o markdown_io.o

markdown_io.o: markdown_io.c cstring.o markdown.o
	cc -g -c markdown_io.c -o markdown_io.o -lmarkdown

markdown.o: markdown.c cstring.o
	cc -g -c markdown.c -o markdown.o -lcstring

cstring.o: cstring.c
	cc -g -c cstring.c -o cstring.o

all: tmp

.PHONY: test

test:
	$(MAKE) -C test

clean:
	rm -f tmp *.o
	$(MAKE) -C test clean

