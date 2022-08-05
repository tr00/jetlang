#include "../utils/alloc.h"

#include <string.h>

#include "str.h"

string_t *str_make_string(const char *data, int len)
{
    size_t size = (((len + 1) & 7) + 8) * sizeof(char) + sizeof(string_t);

    string_t *str = jet_alloc_u(size);

    str->len = len;

    memcpy(str->data, data, len);

    return str;
}

string_t *str_append_string_inplace(string_t *restrict a, string_t *restrict b)
{
}

string_t *str_append_string_buffer(string_t *restrict a, string_t *restrict b)
{
}
