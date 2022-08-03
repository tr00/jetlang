#ifndef __CST_H_
#define __CST_H_

#include <stddef.h>


enum
{
    // atoms:
    CST_TAG_NIL,
    CST_TAG_INT,
    CST_TAG_SYM,
    CST_TAG_STR,

    // exprs:
    CST_TAG_CALL,
    CST_TAG_BIND,

    __CST_NTAGS
};

typedef struct
{
    int tag;
    int idx;
} cst_node_t;

typedef struct
{
    size_t len, cap;
    struct
    {
        int s, e;
    } *data;
} cst_atom_t;

typedef struct
{
    size_t len;
    size_t cap;
    struct
    {
        int len;
        cst_node_t *data;
    } *nodes;
} cst_expr_t;

struct CST
{
    cst_node_t root;

    cst_atom_t ints;
    cst_atom_t syms;
    cst_atom_t strs;

    struct
    {
        size_t len, cap;
        struct
        {
            int len;
            cst_node_t *data;
        } *nodes;
    } calls, binds;
};

cst_node_t cst_make_node(struct CST *cst, int tag, size_t s, size_t e);
cst_node_t cst_hint_size(struct CST *cst, cst_node_t list, size_t len);
cst_node_t cst_push_node(struct CST *cst, cst_node_t list, cst_node_t item);
cst_node_t cst_flip_node(struct CST *cst, cst_node_t list);


#endif
