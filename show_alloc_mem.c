/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** show_alloc_mem
*/

#include "malloc.h"

void show_break(void *current)
{
    while (((info_t *)current)->is_free != 2)
        current += sizeof(info_t) + ((info_t *)current)->size;
    current += sizeof(info_t) + ((info_t *)current)->size;
    my_putstr("break : 0x");
    my_putnbr_base((unsigned long long)current, "0123456789abcdef");
    my_putchar('\n');
}

void show_alloc(void *current)
{
    void *first_ptr;
    void *second_ptr;

    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 0) {
            first_ptr = current + sizeof(info_t);
            second_ptr = current + sizeof(info_t) + ((info_t *)current)->size;
            my_putstr("0x");
            my_putnbr_base((unsigned long long)first_ptr, "0123456789abcdef");
            my_putstr(" - 0x");
            my_putnbr_base((unsigned long long)second_ptr, "0123456789abcdef");
            my_putstr(" : ");
            my_putnbr(((info_t *)current)->size);
            my_putstr(" bytes\n");
        }
        current += sizeof(info_t) + ((info_t *)current)->size;
    }
}

void show_alloc_mem(void)
{
    void *current = head_ptr;

    if (!head_ptr)
        return;
    show_break(current);
    current = head_ptr;
    show_alloc(current);
}