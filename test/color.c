#include <ncurses.h>

WINDOW *w;

void set_colors(void)
{
    int i;
    initscr();
    start_color();
    use_default_colors();

    for(i=0; i < COLORS; i++)
    {
        init_pair(i, i, -1);
        attron(COLOR_PAIR(i));
        printw(" %3.hd", i);
        if((i + 1) % 36 == 0) printw("\n");
        refresh();
    }
    standend();
    getch();
    endwin();
}

int main(void)
{
    set_colors();
    return 0;
}
