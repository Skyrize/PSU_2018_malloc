/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** realloc
*/

#include "malloc.h"

void    realloc_copy(void *current, void *ptr2, size_t size)
{
    for (size_t i = 0; i < ((info_t *)current)->size && i < size; i++)
        ((char *)ptr2)[i] = ((char *)current)[i];
}

void    *realloc(void *ptr, size_t size)
{
    void *current = head_ptr;
    void *ptr2 = malloc(size);

    while (((info_t *)current)->is_free != 2) {
        if (current + sizeof(info_t) == ptr) {
            realloc_copy(current, ptr2, size);
            free(ptr);
            break;
        }
        current += sizeof(info_t) + ((info_t *)current)->size;
    }
    return (ptr2);
}