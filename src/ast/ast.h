#ifndef __AST_H_
#define __AST_H_

#include <stddef.h>
#include <stdint.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpedantic"

typedef enum AST_ATOM_HEAD ast_atom_head_t;
typedef enum AST_EXPR_HEAD ast_expr_head_t;

#pragma clang diagnostic pop

typedef struct AST_ATOM_BODY ast_atom_body_t;
typedef struct AST_EXPR_BODY ast_expr_body_t;

typedef struct AST_ATOM ast_atom_t;
typedef struct AST_EXPR ast_expr_t;
typedef struct AST_NODE ast_node_t;

enum AST_ATOM_HEAD
{
    AST_ATOM_HEAD_NIL,
    AST_ATOM_HEAD_SYM,
    AST_ATOM_HEAD_INT,
    // string
    // float
    // ...
};

struct AST_ATOM_BODY
{
    size_t len;
    const char *str;
};

struct AST_ATOM
{
    ast_atom_head_t head;
    ast_atom_body_t body;
};

enum AST_EXPR_HEAD
{
    AST_EXPR_HEAD_INVOKE,
    // sequel
    // lambda
    // assign
    // ...
};
    
struct AST_EXPR_BODY
{
    size_t len;
    size_t cap;
    ast_node_t **vec;
};

struct AST_EXPR
{
    ast_expr_head_t head;
    ast_expr_body_t body;
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

inline ast_node_t *ast_node_encode(ast_node_t *node, uintptr_t flag)
{
    return (ast_node_t *)((uintptr_t)node | flag);
}

inline ast_node_t *ast_node_decode(ast_node_t *node)
{
    return (ast_node_t *)((uintptr_t)node & ~1ull);
}

inline uintptr_t ast_node_typeof(ast_node_t *node)
{
    return (uintptr_t)node & 1ull;
}

ast_node_t *ast_create_atom(ast_atom_head_t head, const char *str, size_t len);
ast_node_t *ast_create_expr(ast_expr_head_t head);
ast_node_t *ast_expr_push(ast_node_t *, ast_node_t *);
ast_node_t *ast_expr_reverse(ast_node_t *node);
void ast_pretty_print(ast_node_t *);

#endif
