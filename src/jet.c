#include <stdio.h>

#include "pcc/pcc.h"

int main()
{

    ast_node_t *res = pcc_parse_all();

    ast_pretty_print(res);

    return 0;
}
