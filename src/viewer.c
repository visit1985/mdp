/*
 * Functions necessary to display a deck of slides in different color modes
 * using ncurses. Only white, red, and blue are supported, as they can be
 * faded in 256 color mode.
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

#include <ctype.h>  // isalnum
#include <wchar.h>  // wcschr
#include <wctype.h> // iswalnum
#include <string.h> // strcpy
#include <unistd.h> // usleep
#include <stdlib.h> // getenv
#include "viewer.h"
#include "config.h"

int ncurses_display(deck_t *deck, int notrans, int nofade, int invert, int reload, int noreload, int slidenum, int nocodebg) {

    int c = 0;                // char
    int i = 0;                // iterate
    int l = 0;                // line number
    int lc = 0;               // line count
    int sc = 1;               // slide count
    int colors = 0;           // amount of colors supported
    int fade = 0;             // disable color fading by default
    int trans = -1;           // enable transparency if term supports it
    int max_lines = 0;        // max lines per slide
    int max_lines_slide = -1; // the slide that has the most lines
    int max_cols = 0;         // max columns per line
    int offset;               // text offset
    int stop = 0;             // passed stop bits per slide

    // header line 1 is displayed at the top
    int bar_top = (deck->headers > 0) ? 1 : 0;
    // header line 2 is displayed at the bottom
    // anyway we display the slide number at the bottom
    int bar_bottom = (slidenum || deck->headers > 1) ? 1 : 0;

    slide_t *slide = deck->slide;
    line_t *line;

    // init ncurses
    initscr();

    while(slide) {
        lc = 0;
        line = slide->line;

        while(line && line->text) {

            if (line->text->value) {
                lc += url_count_inline(line->text->value);
                line->length -= url_len_inline(line->text->value);
            }

            if(line->length > COLS) {
                i = line->length;
                offset = 0;
                while(i > COLS) {

                    i = prev_blank(line->text, offset + COLS) - offset;

                    // single word is > COLS
                    if(!i) {
                        // calculate min_width
                        i = next_blank(line->text, offset + COLS) - offset;

                        // disable ncurses
                        endwin();

                        // print error
                        fwprintf(stderr, L"Error: Terminal width (%i columns) too small. Need at least %i columns.\n", COLS, i);
                        fwprintf(stderr, L"You may need to shorten some lines by inserting line breaks.\n");

                        // no reload
                        return 0;
                    }

                    // set max_cols
                    max_cols = MAX(i, max_cols);

                    // iterate to next line
                    offset = prev_blank(line->text, offset + COLS);
                    i = line->length - offset;
                    lc++;
                }
                // set max_cols one last time
                max_cols = MAX(i, max_cols);
            } else {
                // set max_cols
                max_cols = MAX(line->length, max_cols);
            }
            lc++;
            line = line->next;
        }

        max_lines = MAX(lc, max_lines);
        if (lc == max_lines) {
            max_lines_slide = sc;
        }

        slide->lines_consumed = lc;
        slide = slide->next;
        ++sc;
    }

    // not enough lines
    if(max_lines + bar_top + bar_bottom > LINES) {

        // disable ncurses
        endwin();

        // print error
        fwprintf(stderr, L"Error: Terminal height (%i lines) too small. Need at least %i lines for slide #%i.\n", LINES, max_lines + bar_top + bar_bottom, max_lines_slide);
        fwprintf(stderr, L"You may need to add additional horizontal rules (---) to split your file in shorter slides.\n");

        // no reload
        return 0;
    }

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

        // 256 color mode
        if(COLORS == 256) {

            if(notrans) {
                if(invert) {
                    trans = 15; // white in 256 color mode
                } else {
                    trans = 16; // black in 256 color mode
                }
            }

            if(invert) {
                init_pair(CP_WHITE, 232, trans);
                init_pair(CP_BLUE, 21, trans);
                init_pair(CP_RED, 196, trans);
                init_pair(CP_BLACK, 15, 232);
            } else {
                init_pair(CP_WHITE, 255, trans);
                init_pair(CP_BLUE, 123, trans);
                init_pair(CP_RED, 213, trans);
                init_pair(CP_BLACK, 16, 255);
            }
            init_pair(CP_YELLOW, 208, trans);

            // enable color fading
            if(!nofade)
                fade = true;

        // 8 color mode
        } else {

            if(notrans) {
                if(invert) {
                    trans = FG_COLOR; // white in 8 color mode
                } else {
                    trans = BG_COLOR; // black in 8 color mode
                }
            }

            if(invert) {
                init_pair(CP_WHITE, BG_COLOR, trans);
                init_pair(CP_BLACK, FG_COLOR, BG_COLOR);
            } else {
                init_pair(CP_WHITE, FG_COLOR, trans);
                init_pair(CP_BLACK, BG_COLOR, FG_COLOR);
            }
            init_pair(CP_BLUE, HEADER_COLOR, trans);
            init_pair(CP_RED, BOLD_COLOR, trans);
            init_pair(CP_YELLOW, TITLE_COLOR, trans);
        }

        colors = 1;
    }

    // set background color for main window
    if(colors)
        wbkgd(stdscr, COLOR_PAIR(CP_WHITE));

    // setup content window
    WINDOW *content = newwin(LINES - bar_top - bar_bottom, COLS, 0 + bar_top, 0);

    // set background color of content window
    if(colors)
        wbkgd(content, COLOR_PAIR(CP_WHITE));

    slide = deck->slide;

    // find slide to reload
    sc = 1;
    while(reload > 1 && reload <= deck->slides) {
        slide = slide->next;
        sc++;
        reload--;
    }

    // reset reload indicator
    reload = 0;

    while(slide) {

        url_init();

        // clear windows
        werase(content);
        werase(stdscr);

        // always resize window in case terminal geometry has changed
        wresize(content, LINES - bar_top - bar_bottom, COLS);

        // set main window text color
        if(colors)
            wattron(stdscr, COLOR_PAIR(CP_YELLOW));

        // setup header
        if(bar_top) {
            line = deck->header;
            offset = next_blank(line->text, 0) + 1;
            // add 1st header to header
            mvwaddwstr(stdscr,
                       0, (COLS - line->length + offset) / 2,
                       &line->text->value[offset]);
        }

        // setup footer
        if(deck->headers > 1) {
            line = deck->header->next;
            offset = next_blank(line->text, 0) + 1;
            switch(slidenum) {
                case 0:
                    if (deck->headers == 2) {
                        // add 2nd header to center footer
                        mvwaddwstr(stdscr,
                                   LINES - 1, (COLS - line->length + offset) / 2,
                                   &line->text->value[offset]);
                        break;
                    }
                case 1:
                case 2: // add 2nd header to left footer
                    mvwaddwstr(stdscr,
                               LINES - 1, 3,
                               &line->text->value[offset]);
                    break;
            }
        }

        switch(slidenum) {
            case 0:
                if (deck->headers > 2) {
                    // add 3rd header to right footer
                    line = deck->header->next->next;
                    offset = next_blank(line->text, 0) + 1;
                    mvwaddwstr(stdscr,
                               LINES - 1, COLS - line->length + offset - 3,
                               &line->text->value[offset]);
                }
                break;
            case 1: // add slide number to right footer
                mvwprintw(stdscr,
                          LINES - 1, COLS - int_length(sc) - 3,
                          "%d", sc);
                break;
            case 2: // add slide number and number of slides to right footer
                mvwprintw(stdscr,
                          LINES - 1, COLS - int_length(deck->slides) - int_length(sc) - 6,
                          "%d / %d", sc, deck->slides);
                break;
        }

        // copy changed lines in main window to virtual screen
        wnoutrefresh(stdscr);

        line = slide->line;
        l = stop = 0;

        // print lines
        while(line) {
            add_line(content, l + ((LINES - slide->lines_consumed - bar_top - bar_bottom) / 2),
                     (COLS - max_cols) / 2, line, max_cols, colors, nocodebg);

            // raise stop counter if we pass a line having a stop bit
            if(CHECK_BIT(line->bits, IS_STOP))
                stop++;

            l += (line->length / COLS) + 1;
            line = line->next;

            // only stop here if we didn't stop here recently
            if(stop > slide->stop)
                break;
        }

        // print pandoc URL references
        // only if we already printed all lines of the current slide (or output is stopped)
        if(!line ||
           stop > slide->stop) {
            int i, ymax;
            getmaxyx( content, ymax, i );
            for (i = 0; i < url_get_amount(); i++) {
                mvwprintw(content, ymax - url_get_amount() - 1 + i, 3,
                          "[%d] ", i);
                waddwstr(content, url_get_target(i));
            }
        }

        // copy changed lines in content window to virtual screen
        wnoutrefresh(content);

        // compare virtual screen to physical screen and does the actual updates
        doupdate();

        // fade in
        if(fade)
            fade_in(content, trans, colors, invert);

        // re-enable fading after any undefined key press
        if(COLORS == 256 && !nofade)
            fade = true;

        // wait for user input
        c = getch();

        // evaluate user input
        i = 0;

        if (evaluate_binding(prev_slide_binding, c)) {
            // show previous slide or stop bit
            if(stop > 1 || (stop == 1 && !line)) {
                // show current slide again
                // but stop one stop bit earlier
                slide->stop--;
                fade = false;
            } else {
                if(slide->prev) {
                    // show previous slide
                    slide = slide->prev;
                    sc--;
                    //stop on first bullet point always
                    if(slide->stop > 0)
                        slide->stop = 0;
                } else {
                    // do nothing
                    fade = false;
                }
            }
        } else if (evaluate_binding(next_slide_binding, c)) {
            // show next slide or stop bit
            if(stop && line) {
                // show current slide again
                // but stop one stop bit later (or at end of slide)
                slide->stop++;
                fade = false;
            } else {
                if(slide->next) {
                    // show next slide
                    slide = slide->next;
                    sc++;
                } else {
                    // do nothing
                    fade = false;
                }
            }
        } else if (isdigit(c) && c != '0') {
            // show slide n
            i = get_slide_number(c);
            if(i > 0 && i <= deck->slides) {
                while(sc != i) {
                    // search forward
                    if(sc < i) {
                        if(slide->next) {
                            slide = slide->next;
                            sc++;
                        }
                    // search backward
                    } else {
                        if(slide->prev) {
                            slide = slide->prev;
                            sc--;
                        }
                    }
                }
            } else {
                // disable fading if slide n doesn't exist
                fade = false;
            }
        } else if (evaluate_binding(first_slide_binding, c)) {
            // show first slide
            slide = deck->slide;
            sc = 1;
        } else if (evaluate_binding(last_slide_binding, c)) {
            // show last slide
            for(i = sc; i <= deck->slides; i++) {
                if(slide->next) {
                        slide = slide->next;
                        sc++;
                }
            }
        } else if (evaluate_binding(reload_binding, c)) {
            // reload
            if(noreload == 0) {
                // reload slide N
                reload = sc;
                slide = NULL;
            } else {
                // disable fading if reload is not possible
                fade = false;
            }
        } else if (evaluate_binding(quit_binding, c)) {
            // quit
            // do not fade out on exit
            fade = false;
            // do not reload
            reload = 0;
            slide = NULL;
        } else {
            // disable fading on undefined key press
            fade = false;
        }

        // fade out
        if(fade)
            fade_out(content, trans, colors, invert);

        url_purge();
    }

    // disable ncurses
    endwin();

    // free ncurses memory
    delwin(content);
    if(reload == 0)
        delwin(stdscr);

    // return reload indicator (0 means no reload)
    return reload;
}

void setup_list_strings(void)
{
    const char *str;

    /* utf8 can require 6 bytes */
    if ((str = getenv("MDP_LIST_OPEN")) != NULL && strlen(str) <= 4*6) {
        list_open1 = list_open2 = list_open3 = str;
    } else {
        if ((str = getenv("MDP_LIST_OPEN1")) != NULL && strlen(str) <= 4*6)
            list_open1 = str;
        if ((str = getenv("MDP_LIST_OPEN2")) != NULL && strlen(str) <= 4*6)
            list_open2 = str;
        if ((str = getenv("MDP_LIST_OPEN3")) != NULL && strlen(str) <= 4*6)
            list_open3 = str;
    }
    if ((str = getenv("MDP_LIST_HEAD")) != NULL && strlen(str) <= 4*6) {
        list_head1 = list_head2 = list_head3 = str;
    } else {
        if ((str = getenv("MDP_LIST_HEAD1")) != NULL && strlen(str) <= 4*6)
            list_head1 = str;
        if ((str = getenv("MDP_LIST_HEAD2")) != NULL && strlen(str) <= 4*6)
            list_head2 = str;
        if ((str = getenv("MDP_LIST_HEAD3")) != NULL && strlen(str) <= 4*6)
            list_head3 = str;
    }
}

void add_line(WINDOW *window, int y, int x, line_t *line, int max_cols, int colors, int nocodebg) {

    int i; // increment
    int offset = 0; // text offset

    // move the cursor in position
    wmove(window, y, x);

    if(!line->text->value) {

        // fill rest off line with spaces if we are in a code block
        if(CHECK_BIT(line->bits, IS_CODE) && colors) {
            if(colors && !nocodebg)
                wattron(window, COLOR_PAIR(CP_BLACK));
            for(i = getcurx(window) - x; i < max_cols; i++)
                wprintw(window, "%s", " ");
        }

        // do nothing
        return;
    }

    // IS_UNORDERED_LIST_3
    if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_3)) {
        offset = next_nonblank(line->text, 0);
        char prompt[13 * 6];
        int pos = 0, len, cnt;
        len = sizeof(prompt) - pos;
        cnt = snprintf(&prompt[pos], len, "%s", CHECK_BIT(line->bits, IS_UNORDERED_LIST_1)? list_open1 : "    ");
        pos += (cnt > len - 1 ? len - 1 : cnt);
        len = sizeof(prompt) - pos;
        cnt = snprintf(&prompt[pos], len, "%s", CHECK_BIT(line->bits, IS_UNORDERED_LIST_2)? list_open2 : "    ");
        pos += (cnt > len - 1 ? len - 1 : cnt);
        len = sizeof(prompt) - pos;

        if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_EXT)) {
            snprintf(&prompt[pos], len, "%s", line->next && CHECK_BIT(line->next->bits, IS_UNORDERED_LIST_3)? list_open3 : "    ");
        } else {
            snprintf(&prompt[pos], len, "%s", list_head3);
            offset += 2;
        }

        wprintw(window,
                "%s", prompt);

        if(!CHECK_BIT(line->bits, IS_CODE))
            inline_display(window, &line->text->value[offset], colors, nocodebg);

    // IS_UNORDERED_LIST_2
    } else if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_2)) {
        offset = next_nonblank(line->text, 0);
        char prompt[9 * 6];
        int pos = 0, len, cnt;
        len = sizeof(prompt) - pos;
        cnt = snprintf(&prompt[pos], len, "%s", CHECK_BIT(line->bits, IS_UNORDERED_LIST_1)? list_open1 : "    ");
        pos += (cnt > len - 1 ? len - 1 : cnt);
        len = sizeof(prompt) - pos;

        if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_EXT)) {
            snprintf(&prompt[pos], len, "%s", line->next && CHECK_BIT(line->next->bits, IS_UNORDERED_LIST_2)? list_open2 : "    ");
        } else {
            snprintf(&prompt[pos], len, "%s", list_head2);
            offset += 2;
        }

        wprintw(window,
                "%s", prompt);

        if(!CHECK_BIT(line->bits, IS_CODE))
            inline_display(window, &line->text->value[offset], colors, nocodebg);

    // IS_UNORDERED_LIST_1
    } else if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_1)) {
        offset = next_nonblank(line->text, 0);
        char prompt[5 * 6];

        if(CHECK_BIT(line->bits, IS_UNORDERED_LIST_EXT)) {
            strcpy(&prompt[0], line->next && CHECK_BIT(line->next->bits, IS_UNORDERED_LIST_1)? list_open1 : "    ");
        } else {
            strcpy(&prompt[0], list_head1);
            offset += 2;
        }

        wprintw(window,
                "%s", prompt);

        if(!CHECK_BIT(line->bits, IS_CODE))
            inline_display(window, &line->text->value[offset], colors, nocodebg);
    }

    // IS_CODE
    if(CHECK_BIT(line->bits, IS_CODE)) {

        if (!CHECK_BIT(line->bits, IS_TILDE_CODE) &&
            !CHECK_BIT(line->bits, IS_GFM_CODE)) {
            // set static offset for code
            offset = CODE_INDENT;
        }

        // reverse color for code blocks
        if(colors && !nocodebg)
            wattron(window, COLOR_PAIR(CP_BLACK));

        // print whole lines
        waddwstr(window, &line->text->value[offset]);
    }

    if(!CHECK_BIT(line->bits, IS_UNORDERED_LIST_1) &&
       !CHECK_BIT(line->bits, IS_UNORDERED_LIST_2) &&
       !CHECK_BIT(line->bits, IS_UNORDERED_LIST_3) &&
       !CHECK_BIT(line->bits, IS_CODE)) {

        // IS_QUOTE
        if(CHECK_BIT(line->bits, IS_QUOTE)) {
            while(line->text->value[offset] == '>') {
                // print a reverse color block
                if(colors) {
                    wattron(window, COLOR_PAIR(CP_BLACK));
                    wprintw(window, "%s", " ");
                    wattron(window, COLOR_PAIR(CP_WHITE));
                    wprintw(window, "%s", " ");
                } else {
                    wprintw(window, "%s", ">");
                }

                // find next quote or break
                offset++;
                if(line->text->value[offset] == ' ')
                    offset = next_word(line->text, offset);
            }

            inline_display(window, &line->text->value[offset], colors, nocodebg);
        } else {

            // IS_CENTER
            if(CHECK_BIT(line->bits, IS_CENTER)) {
                if(line->length < max_cols) {
                    wmove(window, y, x + ((max_cols - line->length) / 2));
                }
            }

            // IS_H1 || IS_H2
            if(CHECK_BIT(line->bits, IS_H1) || CHECK_BIT(line->bits, IS_H2)) {

                // set headline color
                if(colors)
                    wattron(window, COLOR_PAIR(CP_BLUE));

                // enable underline for H1
                if(CHECK_BIT(line->bits, IS_H1))
                    wattron(window, A_UNDERLINE);

                // skip hashes
                while(line->text->value[offset] == '#')
                    offset = next_word(line->text, offset);

                // print whole lines
                waddwstr(window, &line->text->value[offset]);

                wattroff(window, A_UNDERLINE);

            // no line-wide markdown
            } else {

                inline_display(window, &line->text->value[offset], colors, nocodebg);
            }
        }
    }

    // fill rest off line with spaces
    // we only need this if the color is inverted (e.g. code-blocks),
    // to ensure the background fades too
    if(CHECK_BIT(line->bits, IS_CODE))
        for(i = getcurx(window) - x; i < max_cols; i++)
            wprintw(window, "%s", " ");

    // reset to default color
    if(colors)
        wattron(window, COLOR_PAIR(CP_WHITE));
    wattroff(window, A_UNDERLINE);
}

void inline_display(WINDOW *window, const wchar_t *c, const int colors, int nocodebg) {
    const static wchar_t *special = L"\\*_`!["; // list of interpreted chars
    const wchar_t *i = c; // iterator
    const wchar_t *label_start, *label_end, *url_start, *url_end;
    int label_length, url_length, url_num;
    cstack_t *stack = cstack_init();


    // for each char in line
    for(; *i; i++) {

        // if char is in special char list
        if(wcschr(special, *i)) {

            // closing special char (or second backslash)
            // only if not followed by :alnum:
            if((stack->top)(stack, *i) &&
               (!iswalnum(i[1]) || *(i + 1) == L'\0' || *i == L'\\')) {

                switch(*i) {
                    // print escaped backslash
                    case L'\\':
                        waddnwstr(window, i, 1);
                        break;
                    // disable highlight
                    case L'*':
                        if(colors)
                            wattron(window, COLOR_PAIR(CP_WHITE));
                        break;
                    // disable underline
                    case L'_':
                        wattroff(window, A_UNDERLINE);
                        break;
                    // disable inline code
                    case L'`':
                        if(colors)
                            wattron(window, COLOR_PAIR(CP_WHITE));
                        break;
                }

                // remove top special char from stack
                (stack->pop)(stack);

            // treat special as regular char
            } else if((stack->top)(stack, L'\\')) {
                waddnwstr(window, i, 1);

                // remove backslash from stack
                (stack->pop)(stack);

            // opening special char
            } else {

                // emphasis or code span can start after new-line or space only
                // and of cause after another emphasis markup
                //TODO this condition looks ugly
                if(i == c ||
                   iswspace(*(i - 1)) ||
                   ((iswspace(*(i - 1)) || *(i - 1) == L'*' || *(i - 1) == L'_') &&
                    ((i - 1) == c || iswspace(*(i - 2)))) ||
                   *i == L'\\') {

                    // url in pandoc style
                    if ((*i == L'[') || (*i == L'!' && *(i + 1) && *(i + 1) == L'[')) {
                        label_start = (*i == L'!') ? i + 2 : i + 1;
                        label_end = url_find_closing_bracket(label_start);
                        url_start = NULL;
                        url_end = NULL;

                        if (label_end && *(label_end + 1) && *(label_end + 1) == L'(' && *(label_end + 2)) {
                            url_start = label_end + 2;
                            url_end = url_find_closing_parentheses(url_start);
                        }

                        if (label_end && *(label_end + 1) == L'(' && url_end && *url_end == L')') {
                            // turn higlighting and underlining on
                            if (colors)
                                wattron(window, COLOR_PAIR(CP_BLUE));
                            wattron(window, A_UNDERLINE);

                            // print the content of the label
                            // the label is printed as is
                            i = label_start;
                            while (i < label_end) {
                                if (*i == L'\\' && *(i + 1)) {
                                    i++;
                                }
                                waddnwstr(window, i, 1);
                                i++;
                            }

                            label_length = label_end - label_start;
                            url_length = url_end - url_start;
                            url_num = url_add(label_start, label_length, url_start, url_length, 0, 0);

                            wprintw(window, " [%d]", url_num);

                            i = url_end;

                            // turn highlighting and underlining off
                            wattroff(window, A_UNDERLINE);
                            wattron(window, COLOR_PAIR(CP_WHITE));

                        } else {
                            waddnwstr(window, i, 1);
                        }

                    } else switch(*i) {
                        // enable highlight
                        case L'*':
                            if(colors)
                                wattron(window, COLOR_PAIR(CP_RED));
                            break;
                        // enable underline
                        case L'_':
                            wattron(window, A_UNDERLINE);
                            break;
                        // enable inline code
                        case L'`':
                            if(colors && !nocodebg)
                                wattron(window, COLOR_PAIR(CP_BLACK));
                            break;
                        // broken pandoc url
                        case L'!':
                        case L'[':
                            waddnwstr(window, i, 1);
                            break;
                        // do nothing for backslashes
                    }

                    // push special char to stack
                    (stack->push)(stack, *i);

                } else {
                    waddnwstr(window, i, 1);
                }
            }

        } else {
            // remove backslash from stack
            if((stack->top)(stack, L'\\'))
                (stack->pop)(stack);

            // print regular char
            waddnwstr(window, i, 1);
        }
    }

    // pop stack until empty to prevent formated trailing spaces
    while(!(stack->empty)(stack)) {
        switch((stack->pop)(stack)) {
            // disable highlight
            case L'*':
                if(colors)
                    wattron(window, COLOR_PAIR(CP_WHITE));
                break;
            // disable underline
            case L'_':
                wattroff(window, A_UNDERLINE);
                break;
            // disable inline code
            case L'`':
                if(colors)
                    wattron(window, COLOR_PAIR(CP_WHITE));
                break;
            // do nothing for backslashes
        }
    }

    (stack->delete)(stack);
}

void fade_out(WINDOW *window, int trans, int colors, int invert) {
    int i; // increment
    if(colors && COLORS == 256) {
        for(i = 22; i >= 0; i--) {

            // dim color pairs
            if(invert) {
                init_pair(CP_WHITE, white_ramp_invert[i], trans);
                init_pair(CP_BLUE, blue_ramp_invert[i], trans);
                init_pair(CP_RED, red_ramp_invert[i], trans);
                init_pair(CP_BLACK, 15, white_ramp_invert[i]);
            } else {
                init_pair(CP_WHITE, white_ramp[i], trans);
                init_pair(CP_BLUE, blue_ramp[i], trans);
                init_pair(CP_RED, red_ramp[i], trans);
                init_pair(CP_BLACK, 16, white_ramp[i]);
            }

            // refresh virtual screen with new color
            wnoutrefresh(window);

            // compare virtual screen to physical screen and does the actual updates
            doupdate();

            // delay for our eyes to recognize the change
            usleep(FADE_DELAY);
        }
    }
}

void fade_in(WINDOW *window, int trans, int colors, int invert) {
    int i; // increment
    if(colors && COLORS == 256) {
        for(i = 0; i <= 23; i++) {

            // brighten color pairs
            if(invert) {
                init_pair(CP_WHITE, white_ramp_invert[i], trans);
                init_pair(CP_BLUE, blue_ramp_invert[i], trans);
                init_pair(CP_RED, red_ramp_invert[i], trans);
                init_pair(CP_BLACK, 15, white_ramp_invert[i]);
            } else {
                init_pair(CP_WHITE, white_ramp[i], trans);
                init_pair(CP_BLUE, blue_ramp[i], trans);
                init_pair(CP_RED, red_ramp[i], trans);
                init_pair(CP_BLACK, 16, white_ramp[i]);
            }

            // refresh virtual screen with new color
            wnoutrefresh(window);

            // compare virtual screen to physical screen and does the actual updates
            doupdate();

            // delay for our eyes to recognize the change
            usleep(FADE_DELAY);
        }
    }
}

int int_length (int val) {
    int l = 1;
    while(val > 9) {
        l++;
        val /= 10;
    }
    return l;
}

int get_slide_number(char init) {
    int retval = init - '0';
    int c;
    // block for tenths of a second when using getch, ERR if no input
    halfdelay(GOTO_SLIDE_DELAY);
    while((c = getch()) != ERR) {
        if (c < '0' || c > '9') {
            retval = -1;
            break;
        }
        retval = (retval * 10) + (c - '0');
    }
    nocbreak();     // cancel half delay mode
    cbreak();       // go back to cbreak
    return retval;
}

bool evaluate_binding(const int bindings[], int c) {
    int binding;
    int ind = 0; 
    while((binding = bindings[ind]) != 0) {
        if (c == binding) return true;
        ind++;
    }
    return false;
}

