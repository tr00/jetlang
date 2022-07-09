#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

extern inline ast_node_t *ast_node_encode(ast_node_t *, uintptr_t);
extern inline ast_node_t *ast_node_decode(ast_node_t *);
extern inline uintptr_t ast_node_typeof(ast_node_t *);

static ast_node_t *_ast_alloc_node()
{
    ast_node_t *node = malloc(sizeof(ast_node_t));

    return node;
}

ast_node_t *ast_create_atom(enum AST_ATOM_HEAD head, const char *str, size_t len)
{
    ast_node_t *node = _ast_alloc_node();

    node->atom.head = head;

    char *buf = malloc(len + 1);

    memcpy(buf, str, len);

    buf[len] = '\0';

    node->atom.body = (struct AST_ATOM_BODY){
        .str = buf,
        .len = len,
    };

    return ast_node_encode(node, AST_NODE_FLAG_ATOM);
}

ast_node_t *ast_create_expr(enum AST_EXPR_HEAD head)
{
    ast_node_t *node = _ast_alloc_node();

    node->expr.head = head;

    return ast_node_encode(node, AST_NODE_FLAG_EXPR);
}

ast_node_t *ast_expr_push(ast_node_t *node, ast_node_t *item)
{
    return node;
}

static void _ast_expr_body_reverse(ast_expr_body_t *body)
{
    size_t lo, hi;

    ast_node_t **vec, *tmp;

    lo = 0;
    hi = body->len - 1;

    vec = body->vec;

    while (lo < hi)
    {
        tmp = vec[lo];
        vec[lo] = vec[hi];
        vec[hi] = tmp;
    }
}

ast_node_t *ast_expr_reverse(ast_node_t *node)
{
    _ast_expr_body_reverse(&ast_node_decode(node)->expr.body);

    return node;
}

static void _ast_pretty_print(ast_node_t *ptr, int depth)
{
    static const char *atom2str[] = {
        [AST_ATOM_HEAD_NIL] = "nil",
        [AST_ATOM_HEAD_SYM] = "sym",
        [AST_ATOM_HEAD_INT] = "int",
    };

    static const char *expr2str[] = {
        [AST_EXPR_HEAD_INVOKE] = "invoke",
    };

    ast_node_t *node;
    int flag;

    flag = ast_node_typeof(ptr);
    node = ast_node_decode(ptr);

    for (int i = 0; i < depth; ++i)
        printf("  ");

    if (flag == AST_NODE_FLAG_ATOM)
    {
        ast_atom_t *atom = &node->atom;

        printf("<atom:%s> %s", atom2str[atom->head], atom->body.str);
    }
    else
    {
        ast_expr_t *expr = &node->expr;

        printf("<expr:%s>", expr2str[expr->head]);

        for (size_t i = 0; i < expr->body.len; ++i)
            _ast_pretty_print(expr->body.vec[i], depth + 1);
    }
}

void ast_pretty_print(ast_node_t *ptr)
{
    _ast_pretty_print(ptr, 0);
}
