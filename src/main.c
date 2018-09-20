/*
 * mdp -- A command-line based markdown presentation tool.
 * Copyright (C) 2018 Michael Goehler
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

#include <errno.h>
#include <getopt.h>
#include <locale.h> // setlocale
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

void usage() {
    fprintf(stderr, "%s", "Usage: mdp [OPTION]... [FILE]\n");
    fprintf(stderr, "%s", "A command-line based markdown presentation tool.\n\n");
    fprintf(stderr, "%s", "  -d, --debug       enable debug messages on STDERR\n");
    fprintf(stderr, "%s", "                    add it multiple times to increases debug level\n");
    fprintf(stderr, "%s", "  -e, --expand      enable character entity expansion\n");
    fprintf(stderr, "%s", "  -f, --nofade      disable color fading in 256 color mode\n");
    fprintf(stderr, "%s", "  -h, --help        display this help and exit\n");
    fprintf(stderr, "%s", "  -i, --invert      swap black and white color\n");
    fprintf(stderr, "%s", "  -t, --notrans     disable transparency in transparent terminal\n");
    fprintf(stderr, "%s", "  -s, --noslidenum  do not show slide number at the bottom\n");
    fprintf(stderr, "%s", "  -v, --version     display the version number and license\n");
    fprintf(stderr, "%s", "  -x, --noslidemax  show slide number, but not total number of slides\n");
    fprintf(stderr, "%s", "  -c, --nocodebg    don't change the background color of code blocks\n");
    fprintf(stderr, "%s", "\nWith no FILE, or when FILE is -, read standard input.\n\n");
    exit(EXIT_FAILURE);
}

void version() {
    printf("mdp %d.%d.%d\n", MDP_VER_MAJOR, MDP_VER_MINOR, MDP_VER_REVISION);
    printf("Copyright (C) 2018 Michael Goehler\n");
    printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
    printf("\nWritten by Michael Goehler and others, see <https://github.com/visit1985/mdp/blob/master/AUTHORS>.\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int notrans = 0;   // disable transparency
    int nofade = 0;    // disable fading
    int invert = 0;    // invert color (black on white)
    int noexpand = 1;  // disable character entity expansion
    int reload = 0;    // reload page N (0 means no reload)
    int noreload = 1;  // reload disabled until we know input is a file
    int slidenum = 2;  // 0:don't show; 1:show #; 2:show #/#
    int nocodebg = 0;  // 0:show code bg as inverted; 1: don't invert code bg

    // define command-line options
    struct option longopts[] = {
        { "debug",      no_argument, 0, 'd' },
        { "expand",     no_argument, 0, 'e' },
        { "nofade",     no_argument, 0, 'f' },
        { "help",       no_argument, 0, 'h' },
        { "invert",     no_argument, 0, 'i' },
        { "notrans",    no_argument, 0, 't' },
        { "version",    no_argument, 0, 'v' },
        { "noslidenum", no_argument, 0, 's' },
        { "noslidemax", no_argument, 0, 'x' },
        { "nocodebg",   no_argument, 0, 'c' },
        { 0, 0, 0, 0 }
    };

    // parse command-line options
    int opt, debug = 0;
    while ((opt = getopt_long(argc, argv, ":defhitvsxc", longopts, NULL)) != -1) {
        switch(opt) {
            case 'd': debug += 1;   break;
            case 'e': noexpand = 0; break;
            case 'f': nofade = 1;   break;
            case 'h': usage();      break;
            case 'i': invert = 1;   break;
            case 't': notrans = 1;  break;
            case 'v': version();    break;
            case 's': slidenum = 0; break;
            case 'x': slidenum = 1; break;
            case 'c': nocodebg = 1; break;
            case ':': fprintf(stderr, "%s: '%c' requires an argument\n", argv[0], optopt); usage(); break;
            case '?':
            default : fprintf(stderr, "%s: option '%c' is invalid\n", argv[0], optopt); usage(); break;
        }
    }

    // set locale to that of the environment, so that ncurses properly renders
    // UTF-8 characters if the system supports it
    setlocale(LC_CTYPE, "");

    // setup list string
    setup_list_strings();

    // open file or set input to STDIN
    char *file = NULL;
    FILE *input;
    if (optind < argc) {
        do {
            file = argv[optind];
        } while(++optind < argc);

        if(!strcmp(file, "-")) {
            input = stdin;
        } else {
            input = fopen(file,"r");
            if(!input) {
                fprintf(stderr, "%s: %s: %s\n", argv[0], file, strerror(errno));
                exit(EXIT_FAILURE);
            }
            // enable reload because input is a file
            noreload = 0;
        }
    } else {
        input = stdin;
    }

    // reload loop
    do {

        // reopen input file on reload
        if(noreload == 0 && reload > 0) {
            if(file) {
                input = fopen(file,"r");
                if(!input) {
                    fprintf(stderr, "%s: %s: %s\n", argv[0], file, strerror(errno));
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "%s: %s\n", argv[0], "no input file");
                exit(EXIT_FAILURE);
            }
        }

        // load deck object from input
        deck_t *deck;
        deck = markdown_load(input, noexpand);

        // close file
        fclose(input);

        // replace stdin with current tty if input was a pipe
        // if input was a pipe reload is disabled, so we simply check that
        if(noreload == 1) {
            input = freopen("/dev/tty", "rw", stdin);
            if(!input) {
                fprintf(stderr, "%s: %s: %s\n", argv[0], "/dev/tty", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        if(debug > 0) {
            markdown_debug(deck, debug);
        }

        reload = ncurses_display(deck, notrans, nofade, invert, reload, noreload, slidenum, nocodebg);

        free_deck(deck);

    // reload if supported and requested
    } while(noreload == 0 && reload > 0);

    return EXIT_SUCCESS;
}
