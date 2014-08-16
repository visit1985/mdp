#include <stdio.h>
#include <stdlib.h>

#include "include/cstring.h"
#include "include/markdown.h"

line_t *new_line() {
    line_t *x = malloc(sizeof(line_t));
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
    return x;
}

