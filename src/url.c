/*
 * Functions necessary to handle pandoc URLs.
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

#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include <stdio.h>

#include "url.h"

static void url_del_elem(url_t *elem);
static void url_print(url_t *u);

static url_t *list;
static int index_max;
static int init_ok;

void url_init(void) {
    list = NULL;
    index_max = 0;
    init_ok = 1;
}

int url_add(const wchar_t *link_name, int link_name_length, const wchar_t *target, int target_length, int x, int y) {
    if (!init_ok) return -1;

    url_t *tmp = NULL;
    int i = 0;

    if (list) {
        tmp = list;
        while (tmp->next) {
            tmp = tmp->next;
            i++;
        }
        tmp->next = malloc(sizeof(url_t));
        assert(tmp->next);
        tmp = tmp->next;
    } else {
        list = malloc(sizeof(url_t));
        tmp = list;
        assert(tmp);
    }

    tmp -> link_name = calloc(link_name_length+1, sizeof(wchar_t));
    assert(tmp->link_name);
    wcsncpy(tmp->link_name, link_name, link_name_length);
    tmp->link_name[link_name_length] = '\0';

    tmp->target = calloc(target_length+1, sizeof(wchar_t));
    assert(tmp->target);
    wcsncpy(tmp->target, target, target_length);
    tmp->target[target_length] = '\0';

    tmp->x = x;
    tmp->y = y;
    tmp->next = NULL;

    index_max++;

    return index_max-1;
}

wchar_t * url_get_target(int index) {
    if (!init_ok) return NULL;

    url_t *tmp = list;

    if (!tmp) return NULL;

    while (index > 0 && tmp && tmp->next) {
        tmp = tmp->next;
        index--;
    }

    if (!index) {
        return tmp->target;
    } else return NULL;
}

wchar_t * url_get_name(int index) {
    url_t *tmp = list;

    while (index > 0 && tmp && tmp->next) {
        tmp = tmp->next;
        index --;
    }

    if (!index) {
        return tmp->link_name;
    } else return NULL;
}

void url_purge() {
    url_del_elem(list);
    list = NULL;
    index_max = 0;
    init_ok = 0;
}

static void url_del_elem(url_t *elem) {
    if (!elem) return;

    if (elem->next) {
        url_del_elem(elem->next);
        elem->next = NULL;
    }

    if (elem->target) {
        free(elem->target);
        elem->target = NULL;
    }

    if (elem->link_name) {
        free(elem->link_name);
        elem->link_name = NULL;
    }

    free(elem);
}

void url_dump(void) {
    if (!list) return;

    url_t *tmp = list;

    while (tmp) {
        url_print(tmp);
        if (tmp->next)
            tmp = tmp->next;
        else break;
    }
}

static void url_print(url_t *u) {
    printf("url_t @ %p\n", u);
}

int url_get_amount(void) {
    return index_max;
}

int url_count_inline(const wchar_t *line) {
    int count = 0;
    const wchar_t *i = line;

    for (; *i; i++) {
        if (*i == '\\') {
            i++;
        } else if ( *i == '[' && *(i+1) && *(i+1) != ']') {
            while (*i && *i != ']') i++;
            i++;
            if (*i == '(' && wcschr(i, ')')) {
                count ++;
                i = wcschr(i, ')');
            }
        }
    }

    return count;
}

int url_len_inline(const wchar_t *value) {
    int count = 0;
    const wchar_t *i = value;

    for (; *i; i++) {
        if (*i == '\\') {
            i++;
        } else if ( *i == '[' && *(i+1) && *(i+1) != ']') {
            while (*i && *i != ']') i++;
            i++;
            if (*i == '(' && wcschr(i, ')')) {
                while (*i && *i != ')') {
                    count++;
                    i++;
                }
            }
        }
    }

    return count;
}
