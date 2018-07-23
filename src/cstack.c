/*
 * An implementation of a char stack in heap memory.
 * Copyright (C) 2018 Michael Goehler
 *
 * This file is part of mdp.
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

#include <wchar.h>
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc, realloc

#include "cstack.h"

cstack_t *cstack_init() {
    cstack_t *stack = NULL;
    if((stack = malloc(sizeof(cstack_t))) != NULL) {
        stack->content = NULL;
        stack->alloc = stack->size = 0;
        stack->head = -1;
        stack->push = cstack_push;
        stack->pop = cstack_pop;
        stack->top = cstack_top;
        stack->empty = cstack_empty;
        stack->delete = cstack_delete;
    } else {
        fprintf(stderr, "%s\n", "cstack_init() failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    return stack;
}

void cstack_push(cstack_t *self, wchar_t c) {
    if(self->size + sizeof(c) > self->alloc) {
        self->alloc += (sizeof(wchar_t));
        if((self->content = realloc(self->content, self->alloc)) == NULL) {
            fprintf(stderr, "%s\n", "cstack_push() failed to reallocate memory.");
            exit(EXIT_FAILURE);
        }
    }
    self->content[++self->head] = c;
    self->size += (sizeof(wchar_t));
}

wchar_t cstack_pop(cstack_t *self) {
    self->size -= (sizeof(wchar_t));
    return self->content[self->head--];
}

bool cstack_top(cstack_t *self, wchar_t c) {
    return self->head >= 0 && self->content[self->head] == c;
}

bool cstack_empty(cstack_t *self) {
    return self->head == -1;
}

void cstack_delete(cstack_t *self) {
    free(self->content);
    free(self);
}
