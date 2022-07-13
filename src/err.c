#include <stdio.h>

#include "err.h"

void __attribute__((noreturn)) error(const char *msg)
{
    fprintf("%s\n", msg);
    exit(1);
}