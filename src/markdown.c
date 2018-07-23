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
 */

#include <stdio.h>
#include <stdlib.h>

#include "markdown.h"

line_t *new_line() {
    line_t *x = malloc(sizeof(line_t));
    x->text = NULL;
    x->prev = x->next = NULL;
    x->bits = x->length = x->offset = 0;
    return x;
}

line_t *next_line(line_t *prev) {
    line_t *x = new_line();
    x->prev = prev;
    prev->next = x;
    return x;
}

slide_t *new_slide() {
    slide_t *x = malloc(sizeof(slide_t));
    x->line = NULL;
    x->prev = x->next = NULL;
    x->lines = x->stop = 0;
    return x;
}

slide_t *next_slide(slide_t *prev) {
    slide_t *x = new_slide();
    x->prev = prev;
    prev->next = x;
    return x;
}

deck_t *new_deck() {
    deck_t *x = malloc(sizeof(deck_t));
    x->header = NULL;
    x->slide = new_slide();
    x->slides = x->headers = 0;
    return x;
}

void free_line(line_t *line) {
    line_t *next;
    while (line) {
        next = line->next;
        if(line->text)
            (line->text->delete)(line->text);
        free(line);
        line = next;
    }
}

void free_deck(deck_t *deck) {
    slide_t *slide, *next;
    if (deck == NULL)
        return;
    slide = deck->slide;
    while (slide) {
        free_line(slide->line);
        next = slide->next;
        free(slide);
        slide = next;
    }
    free_line(deck->header);
    free(deck);
}
