#pragma once

#ifndef __ALLOC_H_
#define __ALLOC_H_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>

#include <sys/mman.h>

static void *jet_alloc_u(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {
        fprintf(stderr, "error: out of memory!\n");
        exit(1);
    }

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

    return page;
}

static void jet_dealloc_u(void *ptr)
{
    free(ptr);
}

static void jet_dealloc_z(void *ptr)
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

#endif
