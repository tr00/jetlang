#ifndef __SYM_H_
#define __SYM_H_

typedef struct
{
    size_t len;
    const char *str;
} sym_string_t; // 16B

sym_string_t *getsym(const char *str, size_t len);
sym_string_t *gensym();

#endif
