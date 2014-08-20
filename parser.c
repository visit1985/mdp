#include <stdio.h>
#include <stdlib.h>

#include "include/parser.h"

document_t *markdown_load(FILE *input) {

    int c = 0, i = 0, l = 0, bits = 0;

    document_t *doc;
    page_t *page;
    line_t *line;
    cstring_t *text;

    doc = new_document();
    page = new_page();
    doc->page = page;
    text = cstring_init();

    while ((c = fgetc(input)) != EOF) {
        if(c == '\n') {

            // markdown analyse
            bits = markdown_analyse(text);

            // if text is markdown hr
            if(CHECK_BIT(bits, IS_HR) &&
               CHECK_BIT(line->bits, IS_EMPTY)) {

                // clear text
                (text->reset)(text);

                // reset line length
                l = 0;

                // create next page
                page = next_page(page);

            } else {

                // if page ! has line
                if(!page->line) {

                    // create new line
                    line = new_line();
                    page->line = line;

                } else {

                    // create next line
                    line = next_line(line);

                }

                // add text to line
                line->text = text;

                // add bits to line
                line->bits = bits;

                // add length to line
                line->length = l;

                // calc offset
                line->offset = next_nonblank(text, 0);

                // new text
                text = cstring_init();

                // reset line length
                l = 0;
            }

        } else if(c == '\t') {

            // expand tab to spaces
            for (i = 0;  i <= 4;  i++) {
                (text->expand)(text, ' ');
                l++;
            }

        } else if(isprint(c) || isspace(c)) {

            // add char to line
            (text->expand)(text, c);

            // increase line lenght
            l++;

        } else if(is_utf8(c)) {

            // add char to line
            (text->expand)(text, c);

            // if utf-8 char > 1 byte add remaing to line
            for(i = 0; i < length_utf8(c) - 1; i++) {
                c = fgetc(input);
                (text->expand)(text, c);
            }

            // increase line length
            l++;
        }
    }

    // detect header
    line = doc->page->line;
    if(line && line->text->size > 0 && line->text->text[0] == '%') {

        // assign header to document
        doc->header = line;

        // find first non-header line
        while(line->text->size > 0 && line->text->text[0] == '%') {
            line = line->next;
        }

        // split linked list
        line->prev->next = (void*)0;
        line->prev = (void*)0;

        // remove header lines from page
        doc->page->line = line;
    }

    // combine underlined H1/H2 in single line
    page = doc->page;
    while(page) {
        line = page->line;
        while(line) {
            if((CHECK_BIT(line->bits, IS_H1) ||
                CHECK_BIT(line->bits, IS_H2)) &&
               CHECK_BIT(line->bits, IS_EMPTY) &&
               line->prev &&
               !CHECK_BIT(line->prev->bits, IS_EMPTY)) {

                // remove line from linked list
                line->prev->next = line->next;
                line->next->prev = line->prev;

                // set bits on revious line
                if(CHECK_BIT(line->bits, IS_H1)) {
                    SET_BIT(line->prev->bits, IS_H1);
                } else {
                    SET_BIT(line->prev->bits, IS_H2);
                }

                // delete line
                (line->text->delete)(line->text);
                free(line);
            }
            line = line->next;
        }
        page = page->next;
    }

    return doc;
}

int markdown_analyse(cstring_t *text) {
    int i = 0, bits = 0,
        offset = 0, eol    = 0,
        equals = 0, hashes = 0,
        stars  = 0, minus  = 0,
        spaces = 0, other  = 0;

    // count leading spaces
    offset = next_nonblank(text, 0);

    // strip trailing spaces
    for(eol = text->size; eol > offset && isspace(text->text[eol - 1]); eol--);

    for(i = offset; i < eol; i++) {

        switch(text->text[i]) {
            case '=': equals++;  break;
            case '#': hashes++;  break;
            case '*': stars++;   break;
            case '-': minus++;   break;
            case ' ': spaces++;  break;
            default:  other++;   break;
        }
    }

    // IS_H1
    if((equals > 0 &&
        hashes + stars + minus + spaces + other == 0) ||
       (text &&
        text->text &&
        text->text[offset] == '#' &&
        text->text[offset+1] != '#')) {

        SET_BIT(bits, IS_H1);
    }

    // IS_H2
    if((minus > 0 &&
        equals + hashes + stars + spaces + other == 0) ||
       (text &&
        text->text &&
        text->text[offset] == '#' &&
        text->text[offset+1] == '#')) {

        SET_BIT(bits, IS_H2);
    }

    // IS_QUOTE
    if(text &&
       text->text &&
       text->text[offset] == '>') {

        SET_BIT(bits, IS_QUOTE);
    }

    // IS_CODE
    if(offset >= 4) {
        SET_BIT(bits, IS_CODE);
    }

    // IS_HR
    if((minus >= 3 && equals + hashes + stars + other == 0) ||
       (stars >= 3 && equals + hashes + minus + other == 0)) {

        SET_BIT(bits, IS_HR);
    }

    // IS_EMPTY
    if(other == 0) {
        SET_BIT(bits, IS_EMPTY);
    }

    return bits;
}

void markdown_debug(document_t *doc, int debug) {

    // print header to STDERR
    int offset;
    line_t *header;
    if(doc->header) {
        header = doc->header;
        while(header &&
            header->length > 0 &&
            header->text->text[0] == '%') {

            offset = next_blank(header->text, 0) + 1;
            fprintf(stderr, "header: %s\n", &header->text->text[offset]);
            header = header->next;
        }
    }

    // print page/line count to STDERR
    int cp = 0, cl = 0;
    page_t *page = doc->page;
    line_t *line;
    while(page) {
        cp++;
        if(debug > 1) {
            fprintf(stderr, "page %i:\n", cp);
        }
        line = page->line;
        cl = 0;
        while(line) {
            cl++;
            if(debug > 1) {
                fprintf(stderr, "  line %i: bits = %i, length = %i\n", cl, line->bits, line->length);
            }
            line = line->next;
        }
        if(debug == 1) {
            fprintf(stderr, "page %i: %i lines\n", cp, cl);
        }
        page = page->next;
    }
}


int is_utf8(char ch) {
    return (ch & 0x80);
}

int length_utf8(char ch) {
    int i = 0;
    while(ch & 0x80) {
        i++;
        ch <<= 1;
    }
    return i;
}

int next_nonblank(cstring_t *text, int i) {
    while ((i < text->size) && isspace((text->text)[i]))
        ++i;
    return i;
};

int next_blank(cstring_t *text, int i) {
    while ((i < text->size) && !isspace((text->text)[i]))
        ++i;
    return i;
};

