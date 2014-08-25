#include <ncurses.h>
#include <signal.h>
#include <stdlib.h>
#define COLOR_PAIR1 1
#define COLOR_PAIR2 2
int main()
{
    initscr();
    curs_set(0);//sets visability of cursor
    noecho();//disable auto-echoing
    cbreak();//making getch() work without a buffer
    keypad(stdscr,TRUE);//allows use of special keys, namely the arrow keys
    if(has_colors() == TRUE)
    {
        start_color();
        use_default_colors();
        init_pair(COLOR_PAIR1,123,-1);
        init_pair(COLOR_PAIR2,213,-1);
        wbkgd(stdscr,COLOR_PAIR(COLOR_PAIR1));
        wmove(stdscr,10,5);
        printw("q = quit");
        WINDOW *test=newwin(10,20,5,20);
        wbkgd(test,COLOR_PAIR(COLOR_PAIR2));
        wprintw(test,"try\nUP or DOWN");
        wrefresh(stdscr);
        wrefresh(test);

        // fading colors
        short blue_ramp[]  = { 16,  17,  17,  18,  18,  19,
                               19,  20,  20,  21,  27,  32,
                               33,  38,  39,  44,  45,  45,
                               81,  81,  51,  51, 123, 123};
        short white_ramp[] = {232, 233, 234, 235, 236, 237,
                              238, 239, 240, 241, 242, 243,
                              244, 245, 246, 247, 248, 249,
                              250, 251, 252, 253, 254, 255};
        short red_ramp[]   = { 16,  52,  52,  53,  53,  89,
                               89,  90,  90, 126, 127, 127,
                              163, 163, 164, 164, 200, 200,
                              201, 201, 207, 207, 213, 213};

        short i = 23;
        for (;;)
        {
            int c = getch();
            if(c == 'q') break;

            switch(c) {
                case KEY_UP:
                    if(i < 23) i = i + 1;
                    break;

                case KEY_DOWN:
                    if(i > 0)i = i - 1;
                    break;
            }

            init_pair(COLOR_PAIR1,blue_ramp[i],-1);
            init_pair(COLOR_PAIR2,red_ramp[i],-1);
            wrefresh(stdscr);
            wrefresh(test);
        }

        delwin(test);
    }

    endwin();
    return 0;
}
