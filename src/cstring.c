/*
 * An implementation of expandable c strings in heap memory.
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

#include <wchar.h> // wcslen, wcscat, wmemmove
#include <stdio.h> // fprintf
#include <stdlib.h> // malloc, realloc

#include "cstring.h"

cstring_t *cstring_init() {
    cstring_t *x = NULL;
    if((x = malloc(sizeof(cstring_t))) != NULL) {
        x->value = NULL;
        x->size = x->alloc = 0;
        x->expand = cstring_expand;
        x->expand_arr = cstring_expand_arr;
        x->strip = cstring_strip;
        x->reset = cstring_reset;
        x->delete = cstring_delete;
    } else {
        fprintf(stderr, "%s\n", "cstring_init() failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    return x;
}

void cstring_expand(cstring_t *self, wchar_t x) {
    if((self->size + 2) * sizeof(wchar_t) > self->alloc) {
        self->alloc += (REALLOC_ADD * sizeof(wchar_t));
        if((self->value = realloc(self->value, self->alloc)) == NULL) {
            fprintf(stderr, "%s\n", "cstring_expand() failed to reallocate memory.");
            exit(EXIT_FAILURE);
        }
    }
    self->value[self->size] = x;
    self->value[self->size+1] = L'\0';
    self->size = wcslen(self->value);
}

void cstring_expand_arr(cstring_t *self, wchar_t *x) {
    if((self->size + wcslen(x) + 1) * sizeof(wchar_t) > self->alloc) {
        self->alloc = ((self->size + wcslen(x) + 1) * sizeof(wchar_t));
        if((self->value = realloc(self->value, self->alloc)) == NULL) {
            fprintf(stderr, "%s\n", "cstring_expand_arr() failed to reallocate memory.");
            exit(EXIT_FAILURE);
        }
    }
    self->value = wcscat(self->value, x);
    self->size = wcslen(self->value);
    self->value[self->size+1] = L'\0';
}

void cstring_strip(cstring_t *self, int pos, int len) {
    if(pos + len >= self->size) {
        if(pos <= self->size) {
            self->value[pos] = L'\0';
            self->size = pos;
        }
        return;
    }
    wmemmove(&self->value[pos], &self->value[pos+len], self->size - pos - len+1);
    self->size -= len;
}

void cstring_reset(cstring_t *self) {
    free(self->value);
    self->value = NULL;
    self->size = self->alloc = 0;
}

void cstring_delete(cstring_t *self) {
    free(self->value);
    free(self);
}
