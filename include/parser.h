#if !defined( PARSER_H )
#define PARSER_H

/*
 * Functions necessary to parse a file and transform its content into
 * a deck of slides containing lines. All based on markdown formating
 * rules.
 *
 * function: markdown_load is the main function which reads a file handle,
 *           and initializes deck, slides and lines
 * function: markdown_analyse which is used to identify line wide formating
 *           rules in given line
 * function: markdown_debug to print a report of the generated data structure
 * function: is_utf8 detects multi-byte char
 * function: length_utf8 calculates the amount of bytes used for a multi-byte
 *           char
 * function: next_nonblank, next_blank, next_word to calculate string offset's
 *
 */

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
