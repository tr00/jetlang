#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uchar;

typedef struct NODE
{
    uintptr_t vec[256];
} node_t;

struct
{
    size_t len;
    node_t root;
} symbol_table;

uintptr_t getsym(const uchar *str)
{
    node_t *node = &symbol_table.root;

FIND:

    for (; *str; ++str)
    {
        uchar cc = *str;

        if (!node->vec[cc])
            goto PUSH;

        node = (node_t *)node->vec[cc];
    }

    goto DONE;

PUSH:

    for (; *str; ++str)
    {
        uchar cc = *str;

        node->vec[cc] = (node_t *)calloc(1, sizeof(node_t));

        node = (node_t *)node->vec[cc];
    }

    node->vec[0] = ++symbol_table.len;

DONE:

    return node->vec[0];
}

uintptr_t gensym()
{
    return ++symbol_table.len;
}

int main()
{
    printf("test: %lu\n", get("test"));
    printf("test2: %lu\n", get("test2"));
    printf("test: %lu\n", get("test"));

    return 0;
}