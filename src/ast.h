#ifndef __AST_H_
#define __AST_H_

#include <stddef.h>
#include <stdint.h>

typedef struct AST_ATOM ast_atom_t;
typedef struct AST_EXPR ast_expr_t;
typedef struct AST_NODE ast_node_t;

struct AST_ATOM
{
    enum AST_ATOM_HEAD
    {
        AST_ATOM_HEAD_NIL,
        AST_ATOM_HEAD_SYM,
        AST_ATOM_HEAD_INT,
        // string
        // float
        // ...
    } head;

    struct AST_ATOM_BODY
    {
        size_t len;
        const char *str;
    } body;
};

struct AST_EXPR
{
    enum AST_EXPR_HEAD
    {
        AST_EXPR_HEAD_INVOKE,
        // sequel
        // lambda
        // assign
        // ...
    } head;

    struct AST_EXPR_BODY
    {
        size_t len;
        size_t cap;
        ast_node_t *vec;
    } body;
};

struct AST_NODE
{
    union
    {
        ast_atom_t atom;
        ast_expr_t expr;        
    };
};

#define AST_NODE_FLAG_ATOM 0ull
#define AST_NODE_FLAG_EXPR 1ull

extern __always_inline ast_node_t *ast_node_decode(ast_node_t *node) 
{
    return (ast_node_t *)((uintptr_t)node & ~1ul);
}

extern __always_inline ast_node_t *ast_node_encode(ast_node_t *node, uintptr_t flag)
{
    return (ast_node_t *)((uintptr_t)node | flag);
}

#endif
