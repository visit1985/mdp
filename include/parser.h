#if !defined( PARSER_H )
#define PARSER_H

#include "markdown.h"

document_t *markdown_load(FILE *input);
int markdown_analyse(cstring_t *text);
int is_utf8(char ch);
int next_nonblank(cstring_t *text, int i);
int next_blank(cstring_t *text, int i);

#endif // !defined( PARSER_H )
