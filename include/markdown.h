#if !defined( MARKDOWN_H )
#define MARKDOWN_H

/*
 * A implementation of markdown objects.
 *
 */

#include "cstring.h"
#include "bitops.h"

enum line_bitmask {
    IS_H1,
    IS_H2,
    IS_QUOTE,
    IS_CODE,
    IS_HR,
    IS_EMPTY
};

typedef struct _line_t {
    cstring_t *text;
    struct _line_t *prev;
    struct _line_t *next;
    int bits;
    int length;
    int offset;
} line_t;

typedef struct _slide_t {
    line_t *line;
    struct _slide_t *prev;
    struct _slide_t *next;
} slide_t;

typedef struct _deck_t {
    line_t *header;
    slide_t *slide;
} deck_t;

line_t *new_line();
line_t *next_line(line_t *prev);
slide_t *new_slide();
slide_t *next_slide(slide_t *prev);
deck_t *new_deck();

#endif // !defined( MARKDOWN_H )
