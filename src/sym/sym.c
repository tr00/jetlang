#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include "sym.h"

typedef unsigned char uchar;

#define JUDY_MASK 0x0full
#define TYPEOF(jp) ((jp) & 0x0ful)
#define DECODE(jp) ((jp) & ~0x0ful)
#define ENCODE(jp, flag) ((uintptr_t)(jp) & (uintptr_t)(flag))

enum
{ 
    JUDY_NULL_NODE = 0,
    JUDY_LEAF_NODE,
    JUDY_TRIE_NODE,
    JUDY_MASK_NODE,
    JUDY_EDGE_NODE,
    JUDY_HALF_NODE,
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

typedef struct JUDY_HALF_NODE
{
    uint64_t keys;
    JP vals[7];
} judy_half_node_t;

typedef struct JUDY_EDGE_NODE
{
    char buf[32];
    size_t len;
    JP *next;
} judy_edge_node_t;

struct {
    JP root;
} judy;


static sym_string_t *_judy_find(const uchar *str, size_t len)
{
    JP jp = judy.root;

    while (len > 0)
    {
        switch (TYPEOF(jp))
        {
            case JUDY_NULL_NODE: case JUDY_LEAF_NODE:
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
            case JUDY_HALF_NODE:
            {
                judy_half_node_t *node = (judy_half_node_t *)DECODE(jp);


                uint64_t key = node->keys & 0xffffffffffffff00ul;

                uint64_t cmp = key & *str;

                if (cmp == 0)
                    return NULL;

                __builtin_clzl(cmp);


                break;
            }
        }
    }

    return TYPEOF(jp) == JUDY_LEAF_NODE ? (sym_string_t *)DECODE(jp) : NULL;
}

static void _judy_push(const uchar *str, size_t len)
{
    JP *jp = &judy.root;

    while (len > 0)
    {
        switch (TYPEOF(*jp))
        {
            case JUDY_NULL_NODE:
            {
                judy_edge_node_t *node = malloc(sizeof(judy_edge_node_t));

                node->len = len <= 32 ? len : 32;

                memcpy(node->buf, str, node->len);

                *jp = ENCODE(node, JUDY_EDGE_NODE);

                jp = &node->next;

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

// stack allocator for symbols

struct SEG
{
    uintptr_t data;
    struct SEG *next;
} *root = NULL;

static sym_string_t *_alloc_string()
{
    if (root == NULL || root->data - 4096 == (uintptr_t)root)
    {
        void *page = malloc(4096);

        struct SEG *seg = (struct SEG *)page;

        seg->data = (uintptr_t)seg + sizeof(struct SEG);
        seg->next = root;

        root = seg;
    }

    sym_string_t *sym = (sym_string_t *)seg->data;

    seg->data += sizeof(sym_string_t);

    return sym;
}

// buddy allocator for char arrays

struct PAGE
{
    uchar *p[5];
    unsigned t5 : 1;
    unsigned t4 : 1;
    unsigned t3 : 1;
    unsigned t2 : 1;
    unsigned t1 : 1;
};

static uintptr_t alloc_two(struct PAGE *page)
{
    if (p[0] == p[1])
        p[1] = t[1] ? p[2] : p[1] + 4;

    uintptr_t res = p[0];

    p[0] = t[0] ? p[1] : p[0] + 2;

    return res;
}

static uintptr_t alloc(size_t n)
{
    switch (n)
    {
        case 32:
        {
            if (p[4] == NULL)
            {
                p[4] = calloc(1, 64);
            }

            uintptr_t res = p[4];

            p[4] = t4 ? NULL : p[4] + 32;

            return res;
        }
        case 16:
        {
            if (p[3] == p[4])
            {
                p[4] = t4 ? NULL : p[4] + 32;
            }
            
            uintptr_t res = p[3];

            p[3] = t3 ? p[4] : p[3] + 16;

            return res;
        }
        case 8:
        {
            if (p[2] == p[3])
            {
                p[3] = t3 ? p[4] : p[3] + 16;
            }
            
            uintptr_t res = p[2];

            p[2] = t2 ? p[3] : p[2] + 8;

            return res;
        }
        case 4:
        {
            if (p[1] == p[2])
            {
                p[2] = t2 ? p[3] : p[2] + 8;
            }
            
            uintptr_t res = p[1];

            p[1] = t1 ? p[2] : p[1] + 4;

            return res;
        }
        case 2:
        {
            if (p[0] == p[1])
            {
                p[1] = t1 ? p[2] : p[1] + 4;
            }
            
            uintptr_t res = p[0];

            p[0] = t0 ? p[1] : p[0] + 2;

            return res;
        }
    }

    __builtin_unreachable();
}