#if !defined( MARKDOWN_H )
#define MARKDOWN_H

/*
 * A implementation of markdown objects.
 *
 */

#include "cstring.h"

#define SET_BIT(var, pos)    ((var) |= (1<<(pos)))
#define CLEAR_BIT(var, pos)  ((var) &= (~(1<<(pos))))
#define TOGGLE_BIT(var, pos) ((var) ^= (1<<(pos)))
#define CHECK_BIT(var, pos)  ((var) &  (1<<(pos)))

enum line_bitmask {
    IS_HEADER,
    IS_HEADER2,
    IS_QUOTE,
    IS_CODE,
    IS_LIST,
    IS_NUMLIST,
    IS_HR
};

typedef struct _line_t {
    cstring_t *text;
    struct _line_t *prev;
    struct _line_t *next;
    int bits;
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
int is_utf8(char ch);
int next_nonblank(cstring_t *text, int i);
document_t *markdown_load(FILE *input);

#endif // !defined( MARKDOWN_H )
