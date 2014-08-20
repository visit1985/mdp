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

typedef struct _page_t {
    line_t *line;
    struct _page_t *prev;
    struct _page_t *next;
} page_t;

typedef struct _document_t {
    line_t *header;
    page_t *page;
} document_t;

line_t *new_line();
line_t *next_line(line_t *prev);
page_t *new_page();
page_t *next_page(page_t *prev);
document_t *new_document();

#endif // !defined( MARKDOWN_H )
