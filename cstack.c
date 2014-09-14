/*
 * An implementation of a char stack in heap memory.
 * Copyright (C) 2014 Michael Goehler
 *
 * This file is part of mpd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

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

int cstack_top(cstack_t *self, char c) {
    if(self->head >= 0 && self->content[self->head] == c)
        return 1;
    return 0;
}

int cstack_empty(cstack_t *self)  {
   return self->head == -1;
}

void cstack_delete(cstack_t *self) {
    free(self->content);
    free(self);
}

