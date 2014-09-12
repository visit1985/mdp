#if !defined( CSTACK_H )
#define CSTACK_H

/*
 * A implementation of a char stack in heap memory.
 *
 * struct: cstack_t which defines char stack type in heap memory
 *
 * function: cstack_init to intialize struct of type cstack_t
 * function: cstack_t->push to add one char on top if the stack
 * function: cstack_t->pop to remove the top char from the stack
 * function: cstack_t->top to test if the top char is a given char
 * function: cstack_t->empty to test if the stack is empty
 * function: cstack_t->delete to free the allocated memory
 *
 * Example:
 *      cstack_t *p = cstack_init();
 *      (p->push)(p, 'X');
 *      printf("%c\n", (p->pop)(p));
 *      (p->delete)(p);
 *
 */

typedef struct _cstack_t {
    char *content;
    size_t alloc;
    size_t size;
    int head;
    void (*push)(struct _cstack_t *self, char c);
    char (*pop)(struct _cstack_t *self);
    int (*top)(struct _cstack_t *self, char c);
    int (*empty)(struct _cstack_t *self);
    void (*delete)(struct _cstack_t *self);
} cstack_t;

cstack_t *cstack_init();
void cstack_push(cstack_t *self, char c);
char cstack_pop(cstack_t *self);
int cstack_top(cstack_t *self, char c);
int cstack_empty(cstack_t *self);
void cstack_delete(cstack_t *self);

#endif // !defined( CSTACK_H )