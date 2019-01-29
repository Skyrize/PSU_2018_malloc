/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** tools
*/

#include "malloc.h"

void *create_page(size_t page_size)
{
    void *ptr = sbrk(page_size);

    if (!ptr)
        return (NULL);
    return (ptr);
}

void create_info_block(void * *ptr, size_t size)
{
    info_t *info_block = *ptr;

    info_block->is_free = 0;
    info_block->size = size;
}