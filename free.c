/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** free
*/

#include "malloc.h"

void shrink(void *first_free, size_t total_freed, size_t page_size)
{
    if (total_freed <= page_size) {
        ((info_t *)first_free)->size = total_freed;
        ((info_t *)first_free)->is_free = 2;
        return;
    }
    sbrk(-page_size);
    shrink(first_free, total_freed - page_size, page_size);
}

int free_pointer(void * current)
{
    void *next = current + sizeof(info_t) + ((info_t *)current)->size;
    if (((info_t *)next)->is_free == 2) {
        ((info_t *)current)->is_free = 2;
        ((info_t *)current)->size += ((info_t *)next)->size + sizeof(info_t);
        return (1);
    }
    ((info_t *)current)->is_free = 1;
    return (0);
}

size_t update_first_free(void * current, void * first_free, size_t total_free)
{
    if (((info_t *)current)->is_free == 1) {
        if (first_free == NULL)
            first_free = current;
        return (total_free + ((info_t *)current)->size + sizeof(info_t));
    }
    first_free = NULL;
    return (0);
}

void check_shrink(void * current, void * first_free, size_t total_free,
size_t page_size)
{
    if (first_free) {
        total_free += ((info_t *)current)->size + sizeof(info_t);
        if (total_free >= page_size)
            shrink(first_free, total_free, page_size);
    }
}

void free(void *ptr)
{
    static size_t page_size = 0;
    void *current = head_ptr;
    void *first_free = NULL;
    size_t total_free = 0;

    if (ptr < head_ptr || ptr > sbrk(0))
        return;
    if (page_size == 0)
        page_size = getpagesize();
    pthread_mutex_lock(&mut);
    while (((info_t *)current)->is_free != 2) {
        if (current + sizeof(info_t) == ptr && free_pointer(current) == 1)
            break;
        total_free = update_first_free(current, first_free, total_free);
        current += sizeof(info_t) + ((info_t *)current)->size;
    }
    check_shrink(current, first_free, total_free, page_size);
    pthread_mutex_unlock(&mut);
}