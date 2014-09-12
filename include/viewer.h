#if !defined( VIEWER_H )
#define VIEWER_H

/*
 * Functions necessary to display a deck of slides in different color modes
 * using ncurses. Only white, red, and blue are supported, as they can be
 * faded in 256 color mode.
 *
 * function: ncurses_display initializes ncurses, defines colors, calculates
 *           window geometry and handles key strokes
 * function: add_line detects inline markdown formating and prints line char
 *           by char
 * function: fade_in, fade_out implementing color fading in 256 color mode
 *
 */

#include <ncurses.h>

#include "parser.h"
#include "cstack.h"

#define CP_WHITE  1 // 255
#define CP_BLUE   2 // 123
#define CP_RED    3 // 213
#define CP_YELLOW 4 // 208

#define FADE_DELAY 15000 // micro seconds

int ncurses_display(deck_t *deck, int notrans, int nofade);
void add_line(WINDOW *window, int y, int x, line_t *line, int max_cols);
void fade_out(WINDOW *window, int trans, int colors);
void fade_in(WINDOW *window, int trans, int colors);

#endif // !defined( VIEWER_H )
