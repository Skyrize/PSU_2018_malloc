/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** malloc
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "malloc.h"

const int info_size = 9;

void    *page_ptr;

void    create_info_block(void * *ptr, size_t size)
{
    printf("CREATE BLOCK OF %lu bytes\n", size);
    info_t *info_block = *ptr;

    info_block->is_free = 0; //0 = false, 1 = true, 2 = end;
    info_block->size = size;
}

void    *browse_alloc(size_t size)
{
    void *current = page_ptr;

    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 1 &&
        size <= ((info_t *)current)->size) {
            ((info_t *)current)->is_free = 0;
            return (current);
        }
        current += info_size + ((info_t *)current)->size;
        printf("NOT NULL\n");
    }
    create_info_block(&current, size);
    printf("%p\n", current);
    current += info_size + ((info_t *)current)->size;
    printf("%p\n", current);
    create_info_block(&current, getpagesize() - (current - page_ptr) - info_size);
    ((info_t *)current)->is_free = 2;
    return (current);
}

void    *malloc(size_t size)
{
    if (!page_ptr) {
        page_ptr = sbrk(getpagesize());
        if (!page_ptr)
            return (NULL);
        create_info_block(&page_ptr, getpagesize() - info_size);
        ((info_t *)page_ptr)->is_free = 2;
    }
    return (browse_alloc(size));
}