#include <stdlib.h>
#include <string.h>

#include "ast.h"

extern ast_node_t *ast_node_decode(ast_node_t *);
extern ast_node_t *ast_node_encode(ast_node_t *, uintptr_t);

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

    return node;
}
