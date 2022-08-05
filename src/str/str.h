#ifndef __STR_H_
#define __STR_H_

typedef struct
{
    int len;
    char data[]; // actual capacity is a mutliple of 8
} string_t;

#endif
