#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    size_t len;
    const char *str;
} strn_t;

struct
{
    size_t len;
    size_t cap;

    struct 
    {
        const char *str;
        size_t len;
    } *keys;
    uint32_t *vals;
} smap;

uint32_t lookup(strn_t key)
{
    for (int i = 0; i < smap.len; ++i)
    {
        if (key.len != keys[i].len)
            continue;

        if (memcmp(key.str, keys[i].str, key.len)) 
            return vals[i];
    }

    return 0;
}

