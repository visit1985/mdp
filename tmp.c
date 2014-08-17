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

    // test line/page load
    int offset;
    line_t *header;
    if(doc->header) {
        header = doc->header;
        while(header &&
              header->text->size > 0 &&
              header->text->text[0] == '%') {

            offset = next_blank(header->text, 0) + 1;
            printf("header: %s\n", &header->text->text[offset]);
            header = header->next;
        }
    }
    int cp = 0, cl = 0;
    page_t *page = doc->page;
    line_t *line;
    while(page) {
        cp++;
        line = page->line;
        cl = 0;
        while(line) {
            cl++;
            line = line->next;
        }
        printf("page %i: %i lines\n", cp, cl);
        page = page->next;
    }
}

