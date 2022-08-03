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
#define DEBUG(n, f, l) printf("[utils/alloc]: %zu bytes allocated @ %s:%d\n", n, f, l);
#else
#define DEBUG(n) 0
#endif


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"

#define jet_alloc_u(size) _jet_alloc_u(size, __FILE__, __LINE__) 

static void *_jet_alloc_u(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    DEBUG(size, file, line);

    return ptr;
}

#define jet_alloc_z(size) _jet_alloc_z(size, __FILE__, __LINE__) 

static void *_jet_alloc_z(size_t size, const char *file, int line)
{
    void *ptr = calloc(1, size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    DEBUG(size, file, line);

    return ptr;
}

#define jet_realloc_a(ptr, size) _jet_realloc_a(ptr, size, __FILE__, __LINE__)

static void *_jet_realloc_a(void *ptr, size_t size, const char *file, int line)
{
    ptr = realloc(ptr, size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    DEBUG(size, file, line);

    return ptr;
}

#define jet_alloc_p() _jet_alloc_p(__FILE__, __LINE__)

static void *_jet_alloc_p(const char *file, int line)
{
    int prot = PROT_READ | PROT_WRITE;
    int flag = MAP_PRIVATE | MAP_ANONYMOUS;

    void *page = mmap(NULL, 4096, prot, flag, -1, 0);

    if (page == MAP_FAILED)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

    DEBUG(4096ul, file, line);

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

#undef DEBUG

#endif
