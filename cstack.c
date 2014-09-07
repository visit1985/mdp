#include <stdlib.h> // malloc, realloc

#include "include/cstack.h"

cstack_t *cstack_init() {
    cstack_t *stack = malloc(sizeof(cstack_t));
    stack->content = (void*)0;
    stack->alloc = stack->size = 0;
    stack->head = -1;
    stack->push = cstack_push;
    stack->pop = cstack_pop;
    stack->top = cstack_top;
    stack->empty = cstack_empty;
    stack->delete = cstack_delete;
    return stack;
}

void cstack_push(cstack_t *self, char c) {
    if(self->size + sizeof(c) > self->alloc) {
        self->alloc += (sizeof(char));
        self->content = realloc(self->content, self->alloc);
    }
    self->content[++self->head] = c;
    self->size += (sizeof(char));
}

char cstack_pop(cstack_t *self) {
    self->size -= (sizeof(char));
    return self->content[self->head--];
}

char cstack_top(cstack_t *self) {
    return self->content[self->head];
}

int cstack_empty(cstack_t *self)  {
   return self->head == -1;
}

void cstack_delete(cstack_t *self) {
    free(self->content);
    free(self);
}

