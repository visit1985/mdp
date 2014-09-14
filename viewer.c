/*
 * Functions necessary to display a deck of slides in different color modes
 * using ncurses. Only white, red, and blue are supported, as they can be
 * faded in 256 color mode.
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

#include <locale.h> // setlocale
#include <ncurses.h>
#include <stdlib.h>
#include <string.h> // strchr
#include <unistd.h>

#include "include/viewer.h"

static short white_ramp[24] = { 16, 232, 233, 234, 235, 236,
                               237, 238, 239, 240, 241, 242,
                               244, 245, 246, 247, 248, 249,
                               250, 251, 252, 253, 254, 255 };

static short blue_ramp[24]  = { 16,  17,  17,  18,  18,  19,
                                19,  20,  20,  21,  27,  32,
                                33,  38,  39,  44,  45,  45,
                                81,  81,  51,  51, 123, 123 };

static short red_ramp[24]   = { 16,  52,  52,  53,  53,  89,
                                89,  90,  90, 126, 127, 127,
                               163, 163, 164, 164, 200, 200,
                               201, 201, 207, 207, 213, 213 };

int ncurses_display(deck_t *deck, int notrans, int nofade) {

    int c = 0;          // char
    int l = 0;          // line number
    int colors = 0;     // amount of colors supported
    int fade = 0;       // disable color fading by default
    int trans = -1;     // enable transparency if term supports it
    int max_lines = 0;  // max lines per slide
    int max_cols = 0;   // max columns per line
    int offset;         // text offset

    // header line 1 is displayed at the top
    int bar_top = (deck->headers > 0) ? 1 : 0;
    // header line 2 and 3 are displayed at the bottom
    int bar_bottom = (deck->headers > 1) ? 1 : 0;

    slide_t *slide = deck->slide;
    line_t *line;

    while(slide) {
        // set max_lines if line count exceeded
        max_lines = (slide->lines > max_lines) ? slide->lines : max_lines;
        line = slide->line;
        while(line) {
            // set max_cols if length exceeded
            max_cols = (line->length > max_cols) ? line->length : max_cols;
            line = line->next;
        }
        slide = slide->next;
    }

    // set locale to display UTF-8 correctly in ncurses
    setlocale(LC_CTYPE, "");

    // replace stdin with current tty if markdown input was piped
    freopen("/dev/tty", "rw", stdin);

    // init ncurses
    initscr();

    if((max_cols > COLS) ||
       (max_lines + bar_top + bar_bottom + 2 > LINES)) {

        fprintf(stderr, "Error: Terminal size %ix%i to small. Need at least %ix%i.\n",
            COLS, LINES, max_cols, max_lines + bar_top + bar_bottom + 2);
        endwin();
        return(1);
    }

    // replace stdin with current tty if markdown input was piped
    freopen("/dev/tty", "rw", stdin);

    // disable cursor
    curs_set(0);

    // disable output of keyboard typing
    noecho();

    // make getch() process one char at a time
    cbreak();

    // enable arrow keys
    keypad(stdscr,TRUE);

    // set colors
    if(has_colors() == TRUE) {
        start_color();
        use_default_colors();

        if(notrans) trans = 0; // 0 is black

        if(COLORS == 256) {
            // 256 color mode
            init_pair(CP_WHITE, 255, trans);
            init_pair(CP_BLUE, 123, trans);
            init_pair(CP_RED, 213, trans);
            init_pair(CP_YELLOW, 208, trans);

            // enable color fading
            if(!nofade) fade = 1;
        } else {

            // 8 color mode
            init_pair(CP_WHITE, 7, trans);
            init_pair(CP_BLUE, 4, trans);
            init_pair(CP_RED, 1, trans);
            init_pair(CP_YELLOW, 3, trans);
        }

        colors = 1;
    }

    // set background color of main window
    if(colors)
        wbkgd(stdscr, COLOR_PAIR(CP_YELLOW));

    // setup header
    if(bar_top) {
        line = deck->header;
        offset = next_blank(line->text, 0) + 1;
        // add text to header
        mvwprintw(stdscr,
                  0, (COLS - line->length + offset) / 2,
                  "%s", &line->text->text[offset]);
    }

    // setup footer
    //TODO display slide number in footer
    if(bar_bottom) {
        line = deck->header->next;
        offset = next_blank(line->text, 0) + 1;
        // add text to left footer
        mvwprintw(stdscr,
                  LINES - 1, 3,
                  "%s", &line->text->text[offset]);

        if(deck->headers > 2) {
            line = deck->header->next->next;
            offset = next_blank(line->text, 0) + 1;
            // add text to right footer
            mvwprintw(stdscr,
                      LINES - 1, COLS - line->length + offset - 3,
                      "%s", &line->text->text[offset]);
        }
    }

    // make header + fooder visible
    wrefresh(stdscr);

    // setup main window
    WINDOW *content = newwin(LINES - bar_top - bar_bottom, COLS, 0 + bar_top, 0);
    if(colors)
        wbkgd(content, COLOR_PAIR(CP_WHITE));

    slide = deck->slide;
    while(slide) {
        // clear main window
        werase(content);

        line = slide->line;
        l = 0;

        // print lines
        while(line) {
            add_line(content, l, (COLS - max_cols) / 2, line, max_cols);
            line = line->next;
            l++;
        }

        // make content visible
        wrefresh(content);

        // fade in
        if(fade)
            fade_in(content, trans, colors);

        // re-enable fading after any undefined key press
        if(COLORS == 256 && !nofade) fade = 1;

        // wait for user input
        c = getch();

        // evaluate user input
        switch(c) {

            // show previous slide
            case KEY_UP:
            case KEY_LEFT:
            case 8:   // BACKSPACE (ascii)
            case 127: // BACKSPACE (xterm)
            case 263: // BACKSPACE (getty)
            case 'h':
            case 'k':
                if(slide->prev)
                    slide = slide->prev;
                break;

            // show next slide
            case KEY_DOWN:
            case KEY_RIGHT:
            case '\n': // ENTER
            case ' ':  // SPACE
            case 'j':
            case 'l':
                if(slide->next)
                    slide = slide->next;
                break;

            // quit
            case 'q':
                // do not fade out on exit
                fade = 0;
                slide = (void*)0;
                break;

            default:
                // disable fading on undefined key press
                fade = 0;
                break;
        }

        // fade out
        if(fade)
            fade_out(content, trans, colors);
    }

    endwin();

    return(0);
}

void add_line(WINDOW *window, int y, int x, line_t *line, int max_cols) {
    int i = 0; // increment
    char *c; // char pointer for iteration
    char *special = "\\*_"; // list of interpreted chars
    cstack_t *stack = cstack_init();

    if(line->text->text) {
        int offset = 0; // text offset
        offset = next_nonblank(line->text, 0);

        // IS_CODE
        if(CHECK_BIT(line->bits, IS_CODE)) {

            // set static offset for code
            offset = CODE_INDENT;

            // reverse color for code blocks
            wattron(window, A_REVERSE);

            // print whole lines
            mvwprintw(window,
                      y, x,
                      "%s", &line->text->text[offset]);

        } else {

            // IS_H1 || IS_H2
            if(CHECK_BIT(line->bits, IS_H1) || CHECK_BIT(line->bits, IS_H2)) {

                // set headline color
                wattron(window, COLOR_PAIR(CP_BLUE));

                // enable underline for H1
                if(CHECK_BIT(line->bits, IS_H1))
                    wattron(window, A_UNDERLINE);

                // skip hashes
                while(line->text->text[offset] == '#')
                    offset = next_word(line->text, offset);

                // print whole lines
                mvwprintw(window,
                      y, x,
                      "%s", &line->text->text[offset]);

                wattroff(window, A_UNDERLINE);

            } else {
                // move the cursor in position
                wmove(window, y, x);

                // IS_QUOTE
                if(CHECK_BIT(line->bits, IS_QUOTE)) {
                    // print a reverse color block
                    wattron(window, A_REVERSE);
                    wprintw(window, "%s", " ");
                    wattroff(window, A_REVERSE);
                    wprintw(window, "%s", " ");
                    offset += 2;
                }

                // for each char in line
                c = &line->text->text[offset];
                while(*c) {

                    // if char is in special char list
                    if(strchr(special, *c)) {

                        // closing special char (or second backslash)
                        if((stack->top)(stack, *c)) {

                            switch(*c) {
                                // print escaped backslash
                                case '\\':
                                    wprintw(window, "%c", *c);
                                    break;
                                // disable highlight
                                case '*':
                                    wattron(window, COLOR_PAIR(CP_WHITE));
                                    break;
                                // disable underline
                                case '_':
                                    wattroff(window, A_UNDERLINE);
                                    break;
                            }

                            // remove top special char from stack
                            (stack->pop)(stack);

                        // treat special as regular char
                        } else if((stack->top)(stack, '\\')) {
                            wprintw(window, "%c", *c);

                            // remove backslash from stack
                            (stack->pop)(stack);

                        // opening special char
                        } else {
                            switch(*c) {
                                // enable highlight
                                case '*':
                                    wattron(window, COLOR_PAIR(CP_RED));
                                    break;
                                // enable underline
                                case '_':
                                    wattron(window, A_UNDERLINE);
                                    break;
                                // do nothing for backslashes
                            }

                            // push special char to stack
                            (stack->push)(stack, *c);
                        }

                    } else {
                        // print regular char
                        wprintw(window, "%c", *c);
                    }

                    c++;
                }

                // pop stack until empty to prevent formated trailing spaces
                while(!(stack->empty)(stack)) {
                    switch((stack->pop)(stack)) {
                        case '\\':
                            wprintw(window, "%c", '\\');
                            break;
                        // disable highlight
                        case '*':
                            wattron(window, COLOR_PAIR(CP_WHITE));
                            break;
                        // disable underline
                        case '_':
                            wattroff(window, A_UNDERLINE);
                            break;
                    }
                }
            }
        }

        // fill rest off line with spaces
        for(i = getcurx(window) - x; i < max_cols; i++)
            wprintw(window, "%s", " ");

        // reset to default color
        wattron(window, COLOR_PAIR(CP_WHITE));
        wattroff(window, A_UNDERLINE);
        wattroff(window, A_REVERSE);
    }

    (stack->delete)(stack);
}

void fade_out(WINDOW *window, int trans, int colors) {
    int i; // increment
    if(colors) {
        for(i = 22; i >= 0; i--) {
            // darken color pairs
            init_pair(CP_WHITE, white_ramp[i], trans);
            init_pair(CP_BLUE, blue_ramp[i], trans);
            init_pair(CP_RED, red_ramp[i], trans);
            // refresh window with new color
            wrefresh(window);
            // delay for our eyes to recognize the change
            usleep(FADE_DELAY);
        }
    }
}

void fade_in(WINDOW *window, int trans, int colors) {
    int i; // increment
    if(colors) {
        for(i = 0; i <= 23; i++) {
            // lighten color pairs
            init_pair(CP_WHITE, white_ramp[i], trans);
            init_pair(CP_BLUE, blue_ramp[i], trans);
            init_pair(CP_RED, red_ramp[i], trans);
            // refresh window with new color
            wrefresh(window);
            // delay for our eyes to recognize the change
            usleep(FADE_DELAY);
        }
    }
}

