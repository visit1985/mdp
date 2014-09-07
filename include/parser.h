#if !defined( PARSER_H )
#define PARSER_H

#include "markdown.h"

#define EXPAND_TABS 4
#define CODE_INDENT 4

deck_t *markdown_load(FILE *input);
int markdown_analyse(cstring_t *text);
void markdown_debug(deck_t *deck, int debug);
int is_utf8(char ch);
int length_utf8(char ch);
int next_nonblank(cstring_t *text, int i);
int next_blank(cstring_t *text, int i);
int next_word(cstring_t *text, int i);

#endif // !defined( PARSER_H )
