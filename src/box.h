#ifndef __BOX_H_
#define __BOX_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{
    void *tag;
    void *val;
} box_t;

typedef struct
{
    size_t len;
    const char *str;
} jet_string_t;

typedef struct
{
    uint64_t val;
} jet_uint64_t;

typedef struct
{
    uint64_t sym;
} jet_symbol_t;

#endif
