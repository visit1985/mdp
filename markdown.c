#include <stdio.h>
#include <stdlib.h>

#include "include/cstring.h"
#include "include/markdown.h"

line_t *new_line() {
    line_t *x = malloc(sizeof(line_t));
    x->text = (void*)0;
    x->prev = x->next = (void*)0;
    x->bits = x->offset = 0;
    return x;
}

line_t *next_line(line_t *prev) {
    line_t *x = new_line();
    x->prev = prev;
    prev->next = x;
    return x;
}

page_t *new_page() {
    page_t *x = malloc(sizeof(page_t));
    x->line = (void*)0;
    x->prev = x->next = (void*)0;
    return x;
}

page_t *next_page(page_t *prev) {
    page_t *x = new_page();
    x->prev = prev;
    prev->next = x;
    return x;
}

document_t *new_document() {
    document_t *x = malloc(sizeof(document_t));
    x->header = (void*)0;
    x->page = (void*)0;
    return x;
}

