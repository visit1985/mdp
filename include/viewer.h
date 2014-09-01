#if !defined( VIEWER_H )
#define VIEWER_H

#include <ncurses.h>

#define CP_WHITE  1 // 255
#define CP_BLUE   2 // 123
#define CP_RED    3 // 213
#define CP_YELLOW 4 // 208

#define FADE_DELAY 15000 // micro seconds

int ncurses_display(deck_t *deck, int notrans, int nofade);
void add_line(WINDOW *window, int y, int x, line_t *line);
void fade_out(WINDOW *window, int trans, int colors);
void fade_in(WINDOW *window, int trans, int colors);

#endif // !defined( VIEWER_H )
