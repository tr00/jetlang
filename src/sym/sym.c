#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sym.h"

typedef unsigned char uchar;

#define JUDY_MASK 0x0full
#define TYPEOF(jp) ((jp) & 0x0ful)
#define DECODE(jp) ((jp) & ~0x0ful)

enum
{ JUDY_NULL_NODE = 0,
    JUDY_LEAF_NODE,
    JUDY_TRIE_NODE,
    JUDY_MASK_NODE,
};

typedef uintptr_t JP; 

typedef struct LEAF_NODE
{

} judy_leaf_t;

typedef struct JUDY_TRIE_NODE
{
    uintptr_t vec[256];
} judy_trie_node_t;

typedef struct JUDY_MASK_NODE
{
    struct SUBEXP
    {
        uint64_t mask;
        struct 
        { 
            JP vec[32]; 
        } *subarray[2];
    } subexp[4];
} judy_mask_node_t;

struct {
    JP root;
} judy;

static sym_string_t *_alloc_string()
{
    return malloc(sizeof(sym_string_t));
}

static sym_string_t *_judy_find(const uchar *str, size_t len)
{
    JP jp = judy.root;

    while (len > 0)
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
            case JUDY_MASK_NODE:
            {
                uchar cc = *str;

                judy_mask_node_t *node = (judy_mask_node_t *)DECODE(jp);

                struct SUBEXP *subexp = &node->subexp[cc >> 5];

                printf("%p\n", (void *)subexp);

                break;
            }
        }
    }

    return TYPEOF(jp) == JUDY_LEAF_NODE ? DECODE(jp) : NULL;
}

static sym_string_t *_judy_push(const uchar *str, size_t len)
{
    JP *jp = &judy.root;

    while (len > 0)
    {
        switch (TYPEOF(*jp))
        {
            case JUDY_NULL_NODE:
            {
                *jp = _judy_suffix(str, len);
                break;
            }
            case JUDY_LEAF_NODE:
                return NULL;
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

    return NULL;
}





