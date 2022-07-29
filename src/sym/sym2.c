#include "../utils/alloc.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "sym.h"

typedef unsigned char uchar;

#define TYPEOF(ptr) ((uintptr_t)(ptr) & 0x7ul)
#define DECODE(ptr) ((uintptr_t)(ptr) & ~0x7ul)
#define ENCODE(ptr, tag) ((uintptr_t)(ptr) | (uintptr_t)(tag))

struct
{
    uintptr_t root;
} judy;

enum
{
    JUDY_NULL_NODE,
    JUDY_LEAF_NODE,
    JUDY_TRIE_NODE,
};

typedef struct
{
    uintptr_t vec[256];
} judy_trie_node_t;

static symbol_t *_judy_find(const uchar *str, int len)
{
    uintptr_t jp = judy.root;

    while (len >= 0)
    {
        switch (TYPEOF(jp))
        {
        case JUDY_NULL_NODE:
        case JUDY_LEAF_NODE:
            return NULL;
        case JUDY_TRIE_NODE:
        {
            judy_trie_node_t *node = (judy_trie_node_t *)DECODE(jp);

            jp = node->vec[*str];

            ++str;
            --len;

            break;
        }
        }
    }

    return TYPEOF(jp) == JUDY_LEAF_NODE ? (symbol_t *)DECODE(jp) : (symbol_t *)NULL;
}

static symbol_t *_judy_push(const uchar *str, int len, symbol_t *val)
{
    uintptr_t *jp = &judy.root;

    while (len >= 0)
    {
        switch (TYPEOF(*jp))
        {
        case JUDY_NULL_NODE:
        {
            judy_trie_node_t *node = (judy_trie_node_t *)jet_alloc_z(sizeof(judy_trie_node_t));

            *jp = ENCODE(node, JUDY_TRIE_NODE);

            break;
        }
        // case JUDY_LEAF_NODE: break; // can never occur
        case JUDY_TRIE_NODE:
        {
            judy_trie_node_t *node = (judy_trie_node_t *)DECODE(*jp);

            jp = &node->vec[*str];

            ++str;
            --len;

            break;
        }
        }
    }

    *jp = ENCODE(val, JUDY_LEAF_NODE);

    return val;
}

static symbol_t *_make_sym(const char *str, int len)
{
    size_t size = sizeof(symbol_t) + (len + 1) * sizeof(char);

    symbol_t *sym = jet_alloc_u(size);

    sym->len = len;

    memcpy(sym->data, str, len + 1);

    return sym;
}

symbol_t *getsym(const char *str, int len)
{
    symbol_t *res;
    
    res = _judy_find((const uchar *)str, len);

    if (res != NULL)
        return res;

    res = _make_sym(str, len);

    printf("allocated symbol @ %p\n", (void *)res);

    return _judy_push((const uchar *)str, len, res);
}

symbol_t *gensym()
{
    return NULL;
}

void judy_init()
{
    judy.root = (uintptr_t)NULL;
}

void judy_exit()
{

}
