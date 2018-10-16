#if !defined( CSTRING_H )
#define CSTRING_H

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
 *
 * struct: cstring_t which defines a expandable c string type in heap memory
 *
 * function: cstring_init to initialize struct of type cstring_t
 * function: cstring_t->expand to add one character to the struct
 * function: cstring_t->expand_arr to add a string to the struct
 * function: cstring_t->strip to remove a substring
 * function: cstring_t->reset to clear and reuse the struct
 * function: cstring_t->delete to free the allocated memory
 *
 * Example:
 *      cstring_t *p = cstring_init();
 *      (p->expand)(p, 'X');
 *      (p->delete)(p);
 *
 */

// The amount of memory allocated from heap when string expansion hits the
// allocated memory limit
#define REALLOC_ADD 10

typedef struct _cstring_t {
    wchar_t *value;
    size_t size;
    size_t alloc;
    void (*expand)(struct _cstring_t *self, wchar_t x);
    void (*expand_arr)(struct _cstring_t *self, wchar_t *x);
    void (*strip)(struct _cstring_t *self, int pos, int len);
    void (*reset)(struct _cstring_t *self);
    void (*delete)(struct _cstring_t *self);
} cstring_t;

cstring_t *cstring_init();
void cstring_expand(cstring_t *self, wchar_t x);
void cstring_expand_arr(cstring_t *self, wchar_t *x);
void cstring_strip(cstring_t *self, int pos, int len);
void cstring_reset(cstring_t *self);
void cstring_delete(cstring_t *self);

#endif // !defined( CSTRING_H )
