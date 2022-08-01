#include "../utils/alloc.h"

#include "cst.h"

typedef struct EDGELIST
{
    size_t len;
    cst_atom_t vec[];
} _cst_edgelist_t;

#define DECODE(ptr) ((uintptr_t)(ptr) & ~1ul)
#define ENCODE(ptr, tag) ((uintptr_t)(ptr) | (uintptr_t)(tag))

static int _cst_push_atom(struct SVEC *vec, int s, int e)
{
    if (vec->len == vec->cap)
    {
        vec->cap *= 2;

        vec->data = jet_realloc_a(vec->data, vec->cap);
    }

    int idx = vec->len;

    vec->data[idx].s = s;
    vec->data[idx].e = e;

    ++vec->len;

    return idx;
}

cst_node_t cst_make_atom(struct CST *cst, int tag, int s, int e)
{
    cst_atom_t atom;

    switch (tag)
    {
    case CST_TAG_NIL:
        atom.idx = 0;
        break;
    case CST_TAG_INT:
        atom.idx = _cst_push_atom(&cst->ints, s, e);
        break;
    case CST_TAG_SYM:
        atom.idx = _cst_push_atom(&cst->syms, s, e);
        break;
    case CST_TAG_STR:
        atom.idx = _cst_push_atom(&cst->strs, s, e);
        break;
    }

    atom.tag = tag;
    atom.___ = 0;

    return *(cst_node_t *)&atom;
}

cst_node_t cst_make_expr(struct CST *cst)
{
    cst_expr_t *expr = (cst_expr_t *)jet_alloc_u(64);

    expr->len = 0;

    return (cst_node_t)ENCODE(expr, 1);
}

cst_node_t cst_hint_size(struct CST *cst, cst_node_t list, size_t len)
{
    cst_expr_t *expr = (cst_expr_t *)DECODE(list);

    size_t size = sizeof(cst_expr_t) + len * sizeof(cst_node_t);

    expr = jet_realloc_u(expr, size);

    return (cst_node_t)ENCODE(expr, 1);
}

cst_node_t cst_push_node(struct CST *cst, cst_node_t list, cst_node_t item)
{
    cst_expr_t *expr = (cst_expr_t *)DECODE(list);

    if ((expr->len + 1) % 8 == 0)
    {
        list = cst_hint_size(cst, list, expr->len + 8);

        expr = (cst_expr_t *)DECODE(list);
    }

    expr->vec[expr->len++] = item;

    return list;
}

cst_node_t cst_flip_node(struct CST *cst, cst_node_t list)
{
    cst_expr_t *expr = (cst_expr_t *)DECODE(list);

    cst_node_t *vec = expr->vec;

    size_t lo = 0;
    size_t hi = expr->len;

    cst_node_t *tmp;

    while (lo < hi)
    {
        tmp = vec[lo];
        vec[lo] = vec[hi];
        vec[hi] = tmp;

        ++lo;
        --hi;
    }

    return list;
}
