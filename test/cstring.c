#include <stdio.h>

#include "../include/cstring.h"
#include "../cstring.c"

int main(int argc, char *argv[]) {

    // testing with char
    cstring_t *p = cstring_init();
    printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);

    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand)(p, 'X'); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);

    (p->reset)(p);
    printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);

    char x[2] = {'X', '\0'};

    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);
    (p->expand_arr)(p, x); printf("text: %s, size: %i, alloc: %i\n", p->text, p->size, p->alloc);

    (p->delete)(p);
}

