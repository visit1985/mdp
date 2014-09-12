#if !defined( CSTRING_H )
#define CSTRING_H

/*
 * A implementation of expandable c strings in heap memory.
 *
 * struct: cstring_t which defines a expandable c string type in heap memory
 *
 * function: cstring_init to intialize struct of type cstring_t
 * function: cstring_t->expand to add one character to the struct
 * function: cstring_t->expand_arr to add a string to the struct
 * function: cstring_t->reset to clear and reuse the struct
 * function: cstring_t->delete to free the allocated memory
 *
 * Example:
 *      cstring_t *p = cstring_init();
 *      (p->expand)(p, 'X');
 *      (p->delete)(p);
 *
 */

// The amount of memory allocated from heap when string expansion hits the
// allocated memory limit
#define REALLOC_ADD 10

typedef struct _cstring_t {
    char *text;
    size_t size;
    size_t alloc;
    void (*expand)(struct _cstring_t *self, char x);
    void (*expand_arr)(struct _cstring_t *self, char *x);
    void (*reset)(struct _cstring_t *self);
    void (*delete)(struct _cstring_t *self);
} cstring_t;

cstring_t *cstring_init();
void cstring_expand(cstring_t *self, char x);
void cstring_expand_arr(cstring_t *self, char *x);
void cstring_reset(cstring_t *self);
void cstring_delete(cstring_t *self);

#endif // !defined( CSTRING_H )
