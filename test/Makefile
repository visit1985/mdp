# set some compiler flags
CFLAGS=

# find *.c in the current directory
SOURCES=$(wildcard *.c)

# define the output objects by replacing .c with .o
TARGETS=$(SOURCES:.c=)

# this will make all objects
all: $(TARGETS) 

# each objects will be build by a *.c file
#%.o: %.c
%: %.c
	cc $(CFLAGS) -o $@ $^

# this will delete all objects
# if not all objects are there, they will be compiled first
clean: $(TARGETS)
	rm -f $^

