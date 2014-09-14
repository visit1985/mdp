#if !defined( PARSER_H )
#define PARSER_H

/*
 * Functions necessary to parse a file and transform its content into
 * a deck of slides containing lines. All based on markdown formating
 * rules.
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
 *
 * function: markdown_load is the main function which reads a file handle,
 *           and initializes deck, slides and lines
 * function: markdown_analyse which is used to identify line wide formating
 *           rules in given line
 * function: markdown_debug to print a report of the generated data structure
 * function: is_utf8 detects multi-byte char
 * function: length_utf8 calculates the amount of bytes used for a multi-byte
 *           char
 * function: next_nonblank, next_blank, next_word to calculate string offset's
 *
 */

#include "markdown.h"

#define EXPAND_TABS 4
#define CODE_INDENT 4

deck_t *markdown_load(FILE *input);
int markdown_analyse(cstring_t *text);
void markdown_debug(deck_t *deck, int debug);
int is_utf8(char ch);
int length_utf8(char ch);
int next_nonblank(cstring_t *text, int i);
int next_blank(cstring_t *text, int i);
int next_word(cstring_t *text, int i);

#endif // !defined( PARSER_H )
