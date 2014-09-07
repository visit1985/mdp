#if !defined( CSTACK_H )
#define CSTACK_H

typedef struct _cstack_t {
    char *content;
    size_t alloc;
    size_t size;
    int head;
    void (*push)(struct _cstack_t *self, char c);
    char (*pop)(struct _cstack_t *self);
    char (*top)(struct _cstack_t *self);
    int (*empty)(struct _cstack_t *self);
    void (*delete)(struct _cstack_t *self);
} cstack_t;

void cstack_push(cstack_t *self, char c);
char cstack_pop(cstack_t *self);
char cstack_top(cstack_t *self);
int cstack_empty(cstack_t *self);
void cstack_delete(cstack_t *self);

#endif // !defined( CSTACK_H )