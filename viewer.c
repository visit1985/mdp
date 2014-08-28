#include <ncurses.h>
#include <stdlib.h>

#include "include/parser.h"
#include "include/viewer.h"

int ncurses_display(deck_t *deck, int notrans, int nofade) {

    int c = 0;          // char
    int colors = 0;     // amount of colors supported
    int fade = 0;       // disable color fading by default
    int trans = -1;     // enable transparency if term supports it
    int max_lines = 0;  // max lines per slide
    int max_cols = 0;   // max columns per line

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

    if((max_cols > COLS) ||
       (max_lines + bar_top + bar_bottom + 2 > LINES)) {

        fprintf(stderr, "Error: Terminal size %ix%i to small. Need at least %ix%i.\n",
            COLS, LINES, max_cols, max_lines + bar_top + bar_bottom + 2);
        return(1);
    }

    // replace stdin with current tty if markdown input was piped
    freopen("/dev/tty", "rw", stdin);

    // init ncurses
    initscr();

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

        if(!notrans) trans = 0; // 0 is black

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
        //TODO move cursor to calculated indentation
        wmove(stdscr, 0, 1);
        //TODO add text to header
        wprintw(stdscr, "header");
    }

    // setup footer
    if(bar_bottom) {
        //TODO move cursor to calculated indentation
        wmove(stdscr, LINES - 1, 1);
        //TODO add text to footer
        wprintw(stdscr, "footer");
    }

    // setup main window
    WINDOW *content = newwin(LINES - bar_top - bar_bottom, COLS, 0, 0 + bar_top);
    if(colors)
        wbkgd(content, COLOR_PAIR(CP_WHITE));

    slide = deck->slide;
    while(slide) {
        // fade out
        // print header / footer
        // print lines
        // fade in

        // wait for user input
        c = getch();

        // evaluate user input
        switch(c) {

            // show previous slide
            case KEY_UP:
            case KEY_LEFT:
            case KEY_BACKSPACE:
            case 'h':
            case 'k':
                if(slide->prev)
                    slide = slide->prev;
                break;

            // show next slide
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_ENTER:
            case 'j':
            case 'l':
                if(slide->next)
                    slide = slide->next;
                break;

            // quit
            case 'q':
                slide = (void*)0;
                break;
        }
    }

    endwin();

    return(0);
}

