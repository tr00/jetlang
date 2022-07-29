#ifndef __SYM_H_
#define __SYM_H_

typedef struct
{
    int len;
    char data[];
} symbol_t;

symbol_t *getsym(const char *str, int len);
symbol_t *gensym();

void judy_init();
void judy_exit();

#endif
