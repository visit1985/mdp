#if !defined( CSTACK_H )
#define CSTACK_H

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
 *
 * struct: cstack_t which defines char stack type in heap memory
 *
 * function: cstack_init to initialize struct of type cstack_t
 * function: cstack_t->push to add one char on top of the stack
 * function: cstack_t->pop to remove the top char from the stack
 * function: cstack_t->top to test if the top char is a given char
 * function: cstack_t->empty to test if the stack is empty
 * function: cstack_t->delete to free the allocated memory
 *
 * Example:
 *      cstack_t *p = cstack_init();
 *      (p->push)(p, 'X');
 *      printf("%c\n", (p->pop)(p));
 *      (p->delete)(p);
 *
 */

#include "common.h"

typedef struct _cstack_t {
    wchar_t *content;
    size_t alloc;
    size_t size;
    int head;
    void (*push)(struct _cstack_t *self, wchar_t c);
    wchar_t (*pop)(struct _cstack_t *self);
    bool (*top)(struct _cstack_t *self, wchar_t c);
    bool (*empty)(struct _cstack_t *self);
    void (*delete)(struct _cstack_t *self);
} cstack_t;

cstack_t *cstack_init();
void cstack_push(cstack_t *self, wchar_t c);
wchar_t cstack_pop(cstack_t *self);
bool cstack_top(cstack_t *self, wchar_t c);
bool cstack_empty(cstack_t *self);
void cstack_delete(cstack_t *self);

#endif // !defined( CSTACK_H )
