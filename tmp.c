#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "include/markdown.h"

int main(int argc, char *argv[]) {

    FILE *input;
    document_t *doc;

    if (argc > 1) {
        if(!strcmp(argv[1], "-")) {
            input = stdin;
        } else {
            input = fopen(argv[1],"r");
            if(!input) {
                fprintf(stderr, "Unable to open '%s': %s\n",
                    argv[1], strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    } else {
        input = stdin;
    }

    doc = markdown_load(input);
}

