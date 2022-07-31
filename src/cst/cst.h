#ifndef __CST_H_
#define __CST_H_

#include <stddef.h>
#include <stdint.h>

struct SVEC
{
    size_t len, cap;
    struct
    {
        int s, e;
    } *data;
};

struct CST
{
    cst_node_t root;

    struct SVEC ints;
    struct SVEC syms;
    struct SVEC strs;
};

enum
{
    CST_TAG_NIL,
    CST_TAG_INT,
    CST_TAG_SYM,
    CST_TAG_STR,
    CST_TAG_CALL,
};

typedef uintptr_t cst_node_t;

typedef struct
{
    int idx : 0x20;
    int tag : 0x1f;
    int ___ : 0x01;
} cst_atom_t; // 64b / 8B

typedef struct
{
    size_t len;
    cst_node_t vec[];
} cst_expr_t;

cst_node_t cst_make_node(struct CST *cst, int tag, const char *str, size_t len);
cst_node_t cst_hint_size(struct CST *cst, cst_node_t list, size_t len);
cst_node_t cst_push_node(struct CST *cst, cst_node_t list, cst_node_t item);
cst_node_t cst_flip_node(struct CST *cst, cst_node_t list);

#endif
