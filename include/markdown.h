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
    IS_NUMLIST
};

typedef struct _line_t {
    cstring_t *text;
    struct _line_t *next;
    int bits;
    int left_white_space;
} line_t;

typedef struct _page_t {
    line_t *lines;
    struct _page_t *next;
} page_t;

typedef struct _document_t {
    line_t *title;
    line_t *author;
    line_t *date;
    page_t *pages;
} document_t;

#endif // !defined( MARKDOWN_H )
