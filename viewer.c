#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/parser.h"
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

        //TODO print lines
        wprintw(content, "content");

        // make content visible
        wrefresh(content);

        // fade in
        if(fade)
            fade_in(content, trans, colors);

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
                // do not fade out on exit
                fade = 0;
                slide = (void*)0;
                break;
        }

        // fade out
        if(fade)
            fade_out(content, trans, colors);
    }

    endwin();

    return(0);
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

