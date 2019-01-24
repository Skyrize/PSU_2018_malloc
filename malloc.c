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

const int info_size = sizeof(info_t);


void    *head_ptr;

void    *create_page()
{
    void *ptr = sbrk(getpagesize());

    if (!ptr)
        return (NULL);
    printf("CREATE PAGE\n");
    return (ptr);
}

void    create_info_block(void * *ptr, size_t size)
{
    info_t *info_block = *ptr;

    info_block->is_free = 0; //0 = false, 1 = true, 2 = end;
    info_block->size = size;
    printf("%p --> %d %lu\n", info_block, info_block->is_free, info_block->size);
    printf("CREATE BLOCK OF %lu bytes\n", size);
}

void    *browse_alloc(size_t size)
{
    void *current = head_ptr;
    int block_size = 0;

    // printf("%p\n", current);
    printf("%d\n", ((info_t *)current)->is_free);
    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 1 &&
        size <= ((info_t *)current)->size) {
            ((info_t *)current)->is_free = 0;
            return (current + info_size);
        }
        // printf("%p\n", current);
        current += info_size + ((info_t *)current)->size;
        // printf("%p\n", current);
        // printf("NOT NULL\n");
        printf("%d\n", ((info_t *)current)->is_free);
    }
    // printf("DECALE FINAL: %lu\n", current - head_ptr);
    while (size + info_size >= ((info_t *)current)->size) {
        if (!create_page())
            return (NULL);
        ((info_t *)current)->size += getpagesize();
        // printf("%lu\n", ((info_t *)current)->size);
    }
    create_info_block(&current, size);
    // printf("%p\n", current);
    current += info_size + ((info_t *)current)->size;
    // printf("%p\n", current);
    block_size = ((getpagesize() - ((current - head_ptr) % getpagesize()) - info_size) + getpagesize()) % getpagesize();
    create_info_block(&current, block_size == 0 ? getpagesize() : block_size);
    ((info_t *)current)->is_free = 2;
    // printf("%d\n", ((info_t *)current)->is_free);
    // printf("%p\n", current);
    return (current - size);
}

void    *malloc(size_t size)
{
    if (!head_ptr) {
        if (!(head_ptr = create_page()))
            return (NULL);
        create_info_block(&head_ptr, getpagesize() - info_size);
        ((info_t *)head_ptr)->is_free = 2;
    }
    return (browse_alloc(size));
}

// void    free(void *ptr)
// {
//     (void)ptr;
//     return;
// }