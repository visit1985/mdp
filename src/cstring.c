/*
 * An implementation of expandable c strings in heap memory.
 * Copyright (C) 2014 Michael Goehler
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

#include <string.h> // strlen
#include <stdlib.h> // malloc, realloc

#include "cstring.h"

cstring_t *cstring_init() {
    cstring_t *x = malloc(sizeof(cstring_t));
    x->text = NULL;
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
        self->alloc = ((strlen(x) + self->size + 1) * sizeof(char));
        self->text = realloc(self->text, self->alloc);
    }
    self->text = strcat(self->text, x);
    self->size = strlen(self->text);
    self->text[self->size+1] = '\0';
}

void cstring_reset(cstring_t *self) {
    free(self->text);
    self->text = NULL;
    self->size = self->alloc = 0;
}

void cstring_delete(cstring_t *self) {
    free(self->text);
    free(self);
}
