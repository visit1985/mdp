#include <stdio.h>

#include "../include/cstack.h"
#include "../cstack.c"

int main(int argc, char *argv[]) {

    cstack_t *s = cstack_init();
    printf("INIT: head: %d, size: %zu, alloc: %zu\n", s->head, s->size, s->alloc);

    (s->push)(s, 'a');
    printf("PUSH: char: %c, head: %d, size: %zu, alloc: %zu\n", (s->top)(s), s->head, s->size, s->alloc);

    (s->push)(s, 'b');
    printf("PUSH: char: %c, head: %d, size: %zu, alloc: %zu\n", (s->top)(s), s->head, s->size, s->alloc);

    (s->push)(s, 'c');
    printf("PUSH: char: %c, head: %d, size: %zu, alloc: %zu\n", (s->top)(s), s->head, s->size, s->alloc);

    char ch = (s->pop)(s);
    printf("POP: char: %c, head: %d, size: %zu, alloc: %zu\n", ch, s->head, s->size, s->alloc);

    ch = (s->pop)(s);
    printf("POP: char: %c, head: %d, size: %zu, alloc: %zu\n", ch, s->head, s->size, s->alloc);

    (s->push)(s, 'd');
    printf("PUSH: char: %c, head: %d, size: %zu, alloc: %zu\n", (s->top)(s), s->head, s->size, s->alloc);

    (s->push)(s, 'e');
    printf("PUSH: char: %c, head: %d, size: %zu, alloc: %zu\n", (s->top)(s), s->head, s->size, s->alloc);

    while(s->size > 0) {
        ch = (s->pop)(s);
        printf("POP: char: %c, head: %d, size: %zu, alloc: %zu\n", ch, s->head, s->size, s->alloc);
    }

    (s->delete)(s);

    return(0);
}

