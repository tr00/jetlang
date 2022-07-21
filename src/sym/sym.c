#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include <sys/mman.h>

#include "sym.h"

typedef unsigned char uchar;

#define JUDY_MASK 0x0full
#define TYPEOF(jp) ((jp) & 0x0ful)
#define DECODE(jp) ((jp) & ~0x0ful)
#define ENCODE(jp, flag) ((uintptr_t)(jp) & (uintptr_t)(flag))

static void __attribute__((noinline, noreturn)) memcrash()
{
    fprintf(stderr, "error: memory corruption in the backend!\n");
    exit(EXIT_FAILURE);
}

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

symbol_t *getsym(const char *str, size_t len)
{
    symbol_t *res;

    res = _judy_find(str, len);

    if (res != NULL)
        return res;

    res = _judy_push(str, len);

    return res;
}

typedef struct NODE
{
    size_t size;
    struct NODE *next;
} *root;

static void *_alloc_page()
{
    int prot = PROT_READ | PROT_WRITE;
    int flag = MAP_PRIVATE | MAP_ANONYMOUS;

    void *page = mmap(NULL, 4096, prot, flag, -1, 0);

    if (page == MAP_FAILED)
        memcrash();

    return page;
}

static void _dealloc_page(void *page)
{
    if (munmap(page, 4096) == MAP_FAILED)
        memcrash();
}


static void *_alloc(size_t size)
{
    struct NODE *node = root;

    while (node->size < size)
    {
        if (node->next == NULL)
        {
            node->next = _alloc_page();
            node->size = 4096 - sizeof(struct NODE);

            node = node->next;
            break;
        }
            
        node = node->next;
    }

    void *res = (uintptr_t)node + (4096 - node->size);

    node->size -= size;

    return res;
}

static symbol_t *_store_symbol(const char *str, size_t len)
{
    size_t size = (len & 0x03ul) + 4;

    symbol_t *sym = _alloc(size);

    sym->len = len;
    
    memcpy(sym->data, str, len);
    memset(sym->data + len, 0, size - len);

    return sym;
}
