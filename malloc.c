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

void    *head_ptr = NULL;

void    *create_page()
{
    void *ptr = sbrk(getpagesize());

    if (!ptr)
        return (NULL);
    // printf("CREATE PAGE\n");
    return (ptr);
}

void    create_info_block(void * *ptr, size_t size)
{
    info_t *info_block = *ptr;

    info_block->is_free = 0; //0 = false, 1 = true, 2 = end;
    info_block->size = size;
    // printf("%p --> %d %lu\n", info_block, info_block->is_free, info_block->size);
    // printf("CREATE BLOCK OF %lu bytes\n", size);
}

void    *browse_alloc(size_t size)
{
    void *current = head_ptr;
    int block_size = 0;

    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 1 &&
        size <= ((info_t *)current)->size) {
            // printf("OCCUP FREE\n");
            ((info_t *)current)->is_free = 0;
            return (current + info_size);
        }
        current += info_size + ((info_t *)current)->size;
    }
    while (size + info_size >= ((info_t *)current)->size) {
        if (!create_page())
            return (NULL);
        ((info_t *)current)->size += getpagesize();
    }
    create_info_block(&current, size);
    current += info_size + ((info_t *)current)->size;
    block_size = ((getpagesize() - ((current - head_ptr) % getpagesize()) - info_size) + getpagesize()) % getpagesize();
    create_info_block(&current, block_size == 0 ? getpagesize() : block_size);
    ((info_t *)current)->is_free = 2;
    return (current - size);
}

void    *malloc(size_t size)
{
    // write(1, "MALLOC\n", 7);
    if (!head_ptr) {
        if (!(head_ptr = create_page()))
            return (NULL);
        create_info_block(&head_ptr, getpagesize() - info_size);
        ((info_t *)head_ptr)->is_free = 2;
    }
    return (browse_alloc(size));
}

void    free(void *ptr)
{
    // write(1, "FREE\n", 5);
    // printf("FREE\n");
    info_t *info = ptr - info_size;
    info->is_free = 1;
    
}

// void    *realloc(void *ptr, size_t size)
// {
//     void *ptr2 = malloc(size);
//     info_t *ptr_info = ptr - info_size;

//     for (size_t i = 0; i < ptr_info->size && i < size; i++) {
//         write(1, "I\n", 2);
//         ((char *)ptr2)[i] = ((char *)ptr)[i];
//     }
//     // printf("%lu\n", ptr_info->size);
//     // write(1, "REALLOC\n", 8);
//     free(ptr);
//     return (ptr2);
// }

void    show_alloc_mem()
{
    void *current = head_ptr;
    void *first_ptr;
    void *second_ptr;

    while (((info_t *)current)->is_free != 2)
        current += info_size + ((info_t *)current)->size;
    current += info_size + ((info_t *)current)->size;
    printf("break : %p\n", current);
    current = head_ptr;
    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 0) {
            first_ptr = current + info_size;
            second_ptr = current + info_size + ((info_t *)current)->size;
            printf("break : %p - %p : %lu bytes\n", first_ptr, second_ptr, ((info_t *)current)->size);
        }
        current += info_size + ((info_t *)current)->size;
    }
}