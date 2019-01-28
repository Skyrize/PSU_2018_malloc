/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** malloc
*/

#include "malloc.h"

void    *head_ptr = NULL;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void    *alloc_in_freed_block(void *current, size_t size)
{
    int save_block_size = ((info_t*)current)->size;

    if (save_block_size - size >= 10) {
        create_info_block(&current, size);
        current += sizeof(info_t) + ((info_t*)current)->size;
        create_info_block(&current, save_block_size - size - sizeof(info_t));
        ((info_t*)current)->is_free = 1;
        return (current - size);
    }
    ((info_t*)current)->is_free = 0;
    return (current + sizeof(info_t));
}

void    *alloc_in_new_block(void *current, size_t size, size_t page_size)
{
    int new_block_size = 0;
    int diff_head_break = 0;

    while (size + sizeof(info_t) >= ((info_t*)current)->size) {
        if (!create_page(page_size))
            return (NULL);
        ((info_t*)current)->size += page_size;
    }
    create_info_block(&current, size);
    current += sizeof(info_t) + ((info_t*)current)->size;
    diff_head_break = current - head_ptr + sizeof(info_t);
    new_block_size = ((diff_head_break / page_size + 1) * page_size);
    new_block_size -= diff_head_break;
    create_info_block(&current, new_block_size);
    ((info_t*)current)->is_free = 2;
    return (current - size);
}

void    *browse_alloc(size_t size, size_t page_size)
{
    void *current = head_ptr;

    while (((info_t*)current)->is_free != 2) {
        if (((info_t*)current)->is_free == 1 &&
        size <= ((info_t*)current)->size)
            return (alloc_in_freed_block(current, size));
        current += sizeof(info_t) + ((info_t*)current)->size;
    }
    return (alloc_in_new_block(current, size, page_size));
}

void    *malloc(size_t size)
{
    static size_t page_size = 0;
    void *space = NULL;

    if (size == 0)
        return (NULL);
    if (page_size == 0)
        page_size = getpagesize();
    pthread_mutex_lock(&mut);
    if (!head_ptr) {
        if (!(head_ptr = create_page(page_size))) {
            pthread_mutex_unlock(&mut);
            return (NULL);
        }
        create_info_block(&head_ptr, page_size - sizeof(info_t));
        ((info_t*)head_ptr)->is_free = 2;
    }
    space = browse_alloc(size, page_size);
    pthread_mutex_unlock(&mut);
    return (space);
}