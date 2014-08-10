# set some compiler flags
CFLAGS=-lncurses

# find *.c in the current directory
SOURCES=$(wildcard *.c)
TESTS=$(wildcard test/*.c)

# define the output objects by replacing .c with .o
TARGETS=$(SOURCES:.c=)
TEST_TARGETS=$(TESTS:.c=)

# this will make all objects
all: $(TEST_TARGETS) $(TARGETS) 
test: $(TEST_TARGETS)

# each objects will be build by a *.c file
#%.o: %.c
%: %.c
	cc $(CFLAGS) -o $@ $^

# this will delete all objects
# if not all objects are there, they will be compiled first
clean: $(TARGETS) $(TEST_TARGETS)
	rm -f $^

