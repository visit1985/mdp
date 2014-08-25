#include <stdio.h>
#include <stdlib.h>

#include "include/cstring.h"
#include "include/markdown.h"

line_t *new_line() {
    line_t *x = malloc(sizeof(line_t));
    x->text = (void*)0;
    x->prev = x->next = (void*)0;
    x->bits = x->length = x->offset = 0;
    return x;
}

line_t *next_line(line_t *prev) {
    line_t *x = new_line();
    x->prev = prev;
    prev->next = x;
    return x;
}

slide_t *new_slide() {
    slide_t *x = malloc(sizeof(slide_t));
    x->line = (void*)0;
    x->prev = x->next = (void*)0;
    x->lines = 0;
    return x;
}

slide_t *next_slide(slide_t *prev) {
    slide_t *x = new_slide();
    x->prev = prev;
    prev->next = x;
    return x;
}

deck_t *new_deck() {
    deck_t *x = malloc(sizeof(deck_t));
    x->header = (void*)0;
    x->slide = (void*)0;
    x->slides = x->headers = 0;
    return x;
}

