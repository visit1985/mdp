#if !defined( MARKDOWN_H )
#define MARKDOWN_H

/*
 * An implementation of markdown objects.
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
 * enum: line_bitmask which enumerates markdown formatting bits
 *
 * struct: deck_t the root object representing a deck of slides
 * struct: slide_t a linked list element of type slide contained in a deck
 * struct: line_t a linked list element of type line contained in a slide
 *
 * function: new_deck to initialize a new deck
 * function: new_slide to initialize a new linked list of type slide
 * function: next_slide to extend a linked list of type slide by one element
 * function: new_line to initialize a new linked list of type line
 * function: next_line to extend a linked list of type line by one element
 * function: free_line to free a line elements memory
 * function: free_deck to free a deck's memory
 *
 */

#include "cstring.h"
#include "bitops.h"

enum line_bitmask {
    IS_H1,
    IS_H1_ATX,
    IS_H2,
    IS_H2_ATX,
    IS_QUOTE,
    IS_CODE,
    IS_TILDE_CODE,
    IS_GFM_CODE,
    IS_HR,
    IS_UNORDERED_LIST_1,
    IS_UNORDERED_LIST_2,
    IS_UNORDERED_LIST_3,
    IS_UNORDERED_LIST_EXT,
    IS_CENTER,
    IS_STOP,
    IS_EMPTY
};

typedef struct _line_t {
    cstring_t *text;
    struct _line_t *prev;
    struct _line_t *next;
    int bits;
    int length;
    int offset;
} line_t;

typedef struct _slide_t {
    line_t *line;
    struct _slide_t *prev;
    struct _slide_t *next;
    int lines;
    int stop;
    int lines_consumed;
} slide_t;

typedef struct _deck_t {
    line_t *header;
    slide_t *slide;
    int slides;
    int headers;
} deck_t;

line_t *new_line();
line_t *next_line(line_t *prev);
slide_t *new_slide();
slide_t *next_slide(slide_t *prev);
deck_t *new_deck();
void free_line(line_t *l);
void free_deck(deck_t *);

#endif // !defined( MARKDOWN_H )
