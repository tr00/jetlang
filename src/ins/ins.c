#include "../cst/cst.h"

#include <stdint.h>
#include <string.h>

#include <x86intrin.h>

static int _atoi_u(const char *f, const char *l, int *res)
{
    // assume f != l

    int sign = (*f == '-') ? -1 : 1;

    for (; f != l; ++f)
    {
        const uint8_t cc = *f - '0';

        int c1 = __builtin_mul_overflow(*res, 10, res);
        int c2 = __builtin_add_overflow(*res, cc, res);

        if (c1 | c2)
            return 1;
    }

    __builtin_mul_overflow(*res, sign, res);

    return 0;
}

// https://stackoverflow.com/questions/35127060/how-to-implement-atoi-using-simd

static int _atoi_v(const char *f, const char *l, unsigned int *res)
{
    static const unsigned int T[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

    unsigned int v[10];

    int i = 0;

    const ptrdiff_t d = l - f;

    for (; l - i != f; ++i)
        v[i] = l[-i] - '0';

    do
    {
        v[i] = l[-i++] - '0'; // 0

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 1

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 2

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 3

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 4

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 5

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 6

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 7

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 8

        if (i == d) break;

        v[i] = l[-i++] - '0'; // 9

    } while(0);

    switch (i)
    {
    case 1: v[1] = 0;
    case 2: v[2] = 0;
    case 3: v[3] = 0;
    case 4: v[4] = 0;
    case 5: v[5] = 0;
    case 6: v[6] = 0;
    case 7: v[7] = 0;
    case 8: v[8] = 0;
    case 9: v[9] = 0;
    }

    *res = 0;

    *res += v[0] * T[0];
    *res += v[1] * T[1];
    *res += v[2] * T[2];
    *res += v[3] * T[3];
    *res += v[4] * T[4];
    *res += v[5] * T[5];
    *res += v[6] * T[6];
    *res += v[7] * T[7];
    *res += v[8] * T[8];
    *res += v[9] * T[9];

    return 0;
}
