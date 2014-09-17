/*
 * mdp -- A command-line based markdown presentation tool.
 * Copyright (C) 2014 Michael Goehler
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/mdp.h"

void usage() {
    fprintf(stderr, "%s", "Usage: mdp [OPTION]... [FILE]\n");
    fprintf(stderr, "%s", "A command-line based markdown presentation tool.\n\n");
    fprintf(stderr, "%s", "  -d, --debug     enable debug messages on STDERR\n");
    fprintf(stderr, "%s", "                  add it multiple times to increases debug level\n");
    fprintf(stderr, "%s", "  -f, --nofade    disable color fading in 256 color mode\n");
    fprintf(stderr, "%s", "  -h, --help      display this help and exit\n");
    fprintf(stderr, "%s", "  -t, --notrans   disable transparency in transparent terminal\n");
    fprintf(stderr, "%s", "\nWith no FILE, or when FILE is -, read standard input.\n\n");
    exit(EXIT_FAILURE);
}

void version() {
    printf("mdp %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
    printf("Copyright (C) 2014 Michael Goehler\n");
    printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
    printf("This is free software: you are free to change and redistribute it.\n");
    printf("There is NO WARRANTY, to the extent permitted by law.\n");
    printf("\nWritten by Michael Goehler and others, see <https://github.com/visit1985/mdp/blob/master/AUTHORS>.\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int notrans = 0;
    int nofade = 0;

    // define command-line options
    struct option longopts[] = {
        { "debug",   no_argument, 0, 'd' },
        { "nofade",  no_argument, 0, 'f' },
        { "help",    no_argument, 0, 'h' },
        { "notrans", no_argument, 0, 't' },
        { "version", no_argument, 0, 'v' },
        { 0, 0, 0, 0 }
    };

    // parse command-line options
    int opt, debug = 0;
    while ((opt = getopt_long(argc, argv, ":dfhtv", longopts, NULL)) != -1) {
        switch(opt) {
            case 'd': debug += 1; break;
            case 'f': nofade = 1; break;
            case 'h': usage(); break;
            case 't': notrans = 1; break;
            case 'v': version(); break;
            case ':': fprintf(stderr, "%s: '%c' requires an argument\n", argv[0], optopt); usage(); break;
            case '?':
            default : fprintf(stderr, "%s: option '%c' is invalid\n", argv[0], optopt); usage(); break;
        }
    }

    // open file or set input to STDIN
    char *file;
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
        }
    } else {
        input = stdin;
    }

    // load deck object from input
    deck_t *deck;
    deck = markdown_load(input);

    // close file
    fclose(input);

    // replace stdin with current tty if input was a pipe
    if(input == stdin)
        freopen("/dev/tty", "rw", stdin);

    if(debug > 0) {
        markdown_debug(deck, debug);
    }

    ncurses_display(deck, notrans, nofade);

    return(EXIT_SUCCESS);
}

