// needed by tmp.c to process markdown

#include <stdio.h>

#include "include/markdown.h"

document_t * markdown_load(FILE *input) {

    int c, i, bits;

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
            if(CHECK_BIT(bits, IS_HR)) {

                // clear text
                (text->reset)(text);
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

                // calc offset
                line->offset = next_nonblank(text, 0);
            }

        } else if('\t') {

            // expand tab to spaces
            for (i = 0;  i <= 4;  i++)
                (text->expand)(text, ' ');

        } else if(isprint(c) || isspace(c) || is_utf8(c)) {

            // add char to line
            (text->expand)(text, c);
        }
    }

    //TODO detect header

    return doc;
}

int markdown_analyse(cstring_t *text) {
    int c, i, bits,
        offset, eol,
        equals, hashes, stars, minus, plus,
        spaces, other;

    // count leading spaces
    offset = next_nonblank(text, 0);

    // IS_CODE
    if(offset >= 4) {
        SET_BIT(bits, IS_CODE);
        return bits;
    }

    // strip trailing spaces
    for(eol = text->size; eol > offset && isspace(text->text[eol - 1]); eol--);

    for(i = offset; i < eol; i++) {

        switch(text->text[i]) {
            case '=': equals++; break;
            case '#': hashes++; break;
            case '*': stars++; break;
            case '-': minus++; break;
            case '+': plus++; break;
            case ' ': spaces++; break;
            default: other++; break;
        }
    }

    // IS_HR
    if((minus >= 3 && equals + hashes + stars + plus == 0) ||
       (stars >= 3 && equals + hashes + minus + plus == 0)) {

        SET_BIT(bits, IS_HR);
        return bits;
    }

    //TODO all the other markdown tags

    return bits;
}

int is_utf8(char ch) {
    return (ch & 0x80);
}

int next_nonblank(cstring_t *text, int i) {
    while ((i < text->size) && isspace((text->text)[i]))
        ++i;
    return i;
};

