#pragma once

#ifndef __ALLOC_H_
#define __ALLOC_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>


#if 1
#define callback(n) printf("alloc: %d bytes", n);
#else
#define callback(n) 0
#endif


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

static void *jet_alloc_u(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    callback(size);

    return ptr;
}

static void *jet_alloc_z(size_t size)
{
    void *ptr = calloc(1, size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    callback(size);

    return ptr;
}

static void *jet_realloc_a(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    callback(size);

    return ptr;
}

static void *jet_alloc_p()
{
    int prot = PROT_READ | PROT_WRITE;
    int flag = MAP_PRIVATE | MAP_ANONYMOUS;

    void *page = mmap(NULL, 4096, prot, flag, -1, 0);

    if (page == MAP_FAILED)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    callback(4096);

    return page;
}

static void jet_dealloc_a(void *ptr)
{
    free(ptr);
}

static void jet_dealloc_p(void *ptr)
{
    if (munmap(ptr, 4096) == -1)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }
}

#pragma clang diagnostic pop

#endif
