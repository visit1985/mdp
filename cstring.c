#include <string.h> // strlen
#include <stdlib.h> // malloc, realloc

#include "include/cstring.h"

cstring_t *cstring_init() {
    cstring_t *x = malloc(sizeof(cstring_t));
    x->text = (void*)0;
    x->size = x->alloc = 0;
    x->expand = cstring_expand;
    x->expand_arr = cstring_expand_arr;
    x->reset = cstring_reset;
    x->delete = cstring_delete;
    return x;
}

void cstring_expand(cstring_t *self, char x) {
    if(self->size + sizeof(x) + sizeof(char) > self->alloc) {
        self->alloc += (REALLOC_ADD * sizeof(char));
        self->text = realloc(self->text, self->alloc);
    }
    self->text[self->size] = x;
    self->text[self->size+1] = '\0';
    self->size = strlen(self->text);
}

void cstring_expand_arr(cstring_t *self, char *x) {
    if(self->size + strlen(x) + sizeof(char) > self->alloc) {
        self->alloc += (REALLOC_ADD * sizeof(char));
        self->text = realloc(self->text, self->alloc);
    }
    self->text = strcat(self->text, x);
    self->size = strlen(self->text);
}

void cstring_reset(cstring_t *self) {
    free(self->text);
    self->text = (void*)0;
    self->size = self->alloc = 0;
}

void cstring_delete(cstring_t *self) {
    free(self->text);
    free(self);
}

