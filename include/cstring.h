#if !defined( CSTRING_H )
#define CSTRING_H

/*
 * A implementation of expandable c strings in heap memory.
 *
 * struct: cstring_t which defines a expandable c string type in heap memory
 *
 * function: cstring_init to intialize struct of type cstring_t
 * function: cstring_t->expand to add one character to the string
 * function: cstring_t->delete to free the allocated memory
 *
 * Example:
 *      cstring_t *p = cstring_init();
 *      (p->expand)(p, 'X');
 *      (p->delete)(p);
 *
 */

#include <string.h> // strlen
#include <stdlib.h> // malloc, realloc

// The amount of memory allocated from heap when string expansion hits the
// allocated memory limit
#define REALLOC_ADD 10

typedef struct _cstring_t {
    char *text;
    size_t size;
    size_t alloc;
    void (*expand)(struct _cstring_t *self, char x);
    void (*expand_arr)(struct _cstring_t *self, char *x);
    void (*delete)(struct _cstring_t *self);
} cstring_t;

void cstring_expand(cstring_t *self, char x);
void cstring_expand_arr(cstring_t *self, char *x);
void cstring_delete(cstring_t *self);

cstring_t *cstring_init() {
    cstring_t *x = malloc(sizeof(cstring_t));
    x->text = (void*)0;
    x->size = x->alloc = 0;
    x->expand = cstring_expand;
    x->expand_arr = cstring_expand_arr;
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


void cstring_delete(cstring_t *self) {
    free(self->text);
    free(self);
}

#endif // !defined( CSTRING_H )
