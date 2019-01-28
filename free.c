/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** free
*/

#include "malloc.h"

void     shrink(void *first_free, size_t total_freed, size_t page_size)
{
    // my_putstr("TOTAL SIZE : ");
    // my_putnbr(total_freed);
    // my_putstr("\n");
    if (total_freed <= page_size) {
        // if (total_freed == 0) {
        //     // my_putstr("\n\nSET HEADER TO NULL\n\n");
        //     head_ptr = NULL;
        //     return;
        // }
        ((info_t *)first_free)->size = total_freed;
        ((info_t *)first_free)->is_free = 2;
        return;
    }
    // my_putstr("\n\nSHRINK\n\n");
    sbrk(-page_size);
    shrink(first_free, total_freed - page_size, page_size);
}

void    free(void *ptr)
{
    static size_t page_size = 0;
    void *current = head_ptr;
    void *first_free = NULL;
    void *next = NULL;
    size_t total_free = 0;

    if (ptr < head_ptr || ptr > sbrk(0))
        return;
    pthread_mutex_lock(&mut);
    if (page_size == 0)
        page_size = getpagesize();
    while (((info_t *)current)->is_free != 2) {
        if (current + sizeof(info_t) == ptr) {
            next = current + sizeof(info_t) + ((info_t *)current)->size;
            if (((info_t *)next)->is_free == 2) {
                ((info_t *)current)->is_free = 2;
                ((info_t *)current)->size += ((info_t *)next)->size + sizeof(info_t);
                break;
            }
            ((info_t *)current)->is_free = 1;
        }
        if (((info_t *)current)->is_free == 1) {
            if (first_free == NULL) {
                first_free = current;
            }
            total_free += ((info_t *)current)->size + sizeof(info_t);
        } else {
            first_free = NULL;
            total_free = 0;
        }
        current += sizeof(info_t) + ((info_t *)current)->size;
    }
    if (first_free) {
        total_free += ((info_t *)current)->size + sizeof(info_t);
        if (total_free >= page_size)
            shrink(first_free, total_free, page_size);
    }
    pthread_mutex_unlock(&mut);
}

// void     free_shrink(void *current, void *first_free, size_t page_size)
// {
//     void *last_free = current;
//     int nb_shrink = 0;

//     last_free += sizeof(info_t) + ((info_t*)last_free)->size;
//     while (((info_t*)last_free)->is_free != 2) {
//         if (((info_t*)last_free)->is_free == 0) {
//             ((info_t*)current)->is_free = 1;
//             my_putstr("FREE - RETURN NORMAL FREE\n");
//             return;
//         }
//         last_free += sizeof(info_t) + ((info_t*)last_free)->size;
//     }
//     nb_shrink = last_free - first_free;
//     nb_shrink += ((info_t*)last_free)->size + sizeof(info_t);
//     nb_shrink /= page_size;
//     my_putstr("FREE - SHRINK : ");
//     my_putnbr(nb_shrink);
//     my_putstr(" (");
//     my_putnbr(last_free - first_free + ((info_t*)last_free)->size + sizeof(info_t));
//     my_putstr(")\n");
//     my_putstr("FREE - LAST FREE SIZE IS : ");
//     my_putnbr(((info_t*)first_free)->size);
//     my_putstr("\n");
//     my_putstr("FREE - CURRENT SIZE IS : ");
//     my_putnbr(((info_t*)current)->size);
//     my_putstr("\n");
//     my_putstr("FREE - NEW CURRENT SIZE IS : ");
//     my_putnbr(((info_t*)last_free)->size);
//     my_putstr("\n");

//     my_putstr("FREE - LAST FREE IS NOW : DIFF BETWEEN LAST FREE AND NEW CURRENT : ");
//     my_putnbr(last_free - first_free);
//     my_putstr("\n");
//     ((info_t*)first_free)->size = (last_free - first_free);

//     my_putstr("FREE - ADD TO LAST FREE : NEW CURRENT SIZE : ");
//     my_putnbr(((info_t*)last_free)->size);
//     my_putstr("\n");
//     ((info_t*)first_free)->size += ((info_t*)last_free)->size;

//     my_putstr("FREE - REMOVE TO LAST FREE : EVENTUAL SHRINK : ");
//     my_putnbr(page_size * nb_shrink);
//     my_putstr("\n");
//     ((info_t*)first_free)->size -= (page_size * nb_shrink);

//     my_putstr("FREE - LAST FREE SIZE IS NOW : ");
//     my_putnbr(((info_t*)first_free)->size);
//     my_putstr("\n");

//     ((info_t*)first_free)->is_free = 2;
//     if (nb_shrink > 0) {
//         my_putstr("FREE - SHRINK OF : ");
//         my_putnbr(nb_shrink);
//         my_putstr(" BLOCK(S)\n");
//         my_putstr("CHECK HEADER TO NULL : SIZE = ");
//         my_putnbr(((info_t*)first_free)->size);
//         my_putstr("\n");
//         if (((info_t*)first_free)->size == -sizeof(info_t)) {
//             my_putstr("SET HEADER TO NULL\n");
//             head_ptr = NULL;
//         }
//         sbrk(-(nb_shrink * page_size));
//     }
//     my_putstr("FREE - RETURN FREED LAST BLOCK\n");
// }

// void    free(void *ptr)
// {
//     static size_t page_size = 0;
//     void *current = head_ptr;
//     void *first_free = current;

//     if (page_size == 0)
//         page_size = getpagesize();
//     while (((info_t*)current)->is_free != 2) {
//         if (current + sizeof(info_t) == ptr) {
//             my_putstr("\nFREE - SIZE OF : ");
//             my_putnbr(((info_t*)current)->size);
//             my_putstr("\n");
//             free_shrink(current, first_free, page_size);
//             show_alloc_mem();
//             return;
//         }
//         if ((((info_t*)current)->is_free == 1 &&
//         ((info_t*)first_free)->is_free == 0) || ((info_t*)current)->is_free == 0)
//             first_free = current;
//         current += sizeof(info_t) + ((info_t*)current)->size;
//     }
// }