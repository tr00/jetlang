#include <stdio.h>

#include "pcc.h"

int main()
{

    pcc_context_t *ctx = pcc_create(NULL);

    ast_node_t *res = NULL;

    int err = pcc_parse(ctx, &res);

    if (err == 0)
    {
        ast_pretty_print(res);
    }

    pcc_destroy(ctx);

    return err;
}
