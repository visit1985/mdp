#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/mdp.h"

void usage() {
    fprintf(stderr, "%s", "Usage: mdp [OPTION]... [FILE]\n");
    fprintf(stderr, "%s", "A command-line based markdown presentation tool.\n\n");
    fprintf(stderr, "%s", "  -d, --debug   enable debug messages on STDERR\n");
    fprintf(stderr, "%s", "                add it multiple times to increases debug level\n\n");
    fprintf(stderr, "%s", "  -h, --help    display this help and exit\n");
    fprintf(stderr, "%s", "\nWith no FILE, or when FILE is -, read standard input.\n\n");
    exit(EXIT_FAILURE);
}

void version() {
    printf("mdp %d.%d\n", MAJOR_VERSION, MINOR_VERSION);
    //TODO add copyright, license, written by
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    // define command-line options
    struct option longopts[] = {
        { "debug",   no_argument, 0, 'd' },
        { "help",    no_argument, 0, 'h' },
        { "version", no_argument, 0, 'v' },
        { 0, 0, 0, 0 }
    };

    // parse command-line options
    int opt, debug = 0;
    while ((opt = getopt_long(argc, argv, ":dhv", longopts, NULL)) != -1) {
        switch(opt) {
            case 'd': debug += 1; break;
            case 'h': usage(); break;
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

    //TODO close file

    if(debug > 0) {
        markdown_debug(deck, debug);
    }

    ncurses_display(deck, 0, 0);

    return(EXIT_SUCCESS);
}

