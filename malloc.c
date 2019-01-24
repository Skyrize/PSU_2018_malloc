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

void	my_putchar(char c)
{
	write(1, &c, 1);
}

int	my_putnbr(int nb)
{
	if (nb < 0) {
		my_putchar('-');
		nb *= -1;
	}
	if (nb > 9)
		my_putnbr(nb / 10);
	my_putchar(nb % 10 + '0');
	return (0);
}

void    *create_page()
{
    void *ptr = sbrk(getpagesize());

    if (!ptr)
        return (NULL);
    // write(1, "CREATE PAGE\n", 12);
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
    // write(1, "CREATE BLOCK OF ", 16);
    // my_putnbr(size);
    // write(1, "\n", 1);
}

void    *browse_alloc(size_t size)
{
    void *current = head_ptr;
    int new_block_size = 0;
    int diff_head_break = 0;
    int save_block_size = 0;

    // write(1, "MALLOC - BEFORE BROWSE\n", 23);
    while (((info_t *)current)->is_free != 2) {
        // write(1, "MALLOC - BROWSE : ", 18);
        // my_putnbr(((info_t *)current)->size);
        // write(1, "\n", 1);
        if (((info_t *)current)->is_free == 1 &&
        size <= ((info_t *)current)->size) {
            // write(1, "OCCUP FREE\n", 11);
            save_block_size = ((info_t *)current)->size;
            if (save_block_size - size >= 10) {
                // write(1, "REWRITE ON FREE : ", 18);
                // my_putnbr(save_block_size - size - info_size);
                // write(1, "\n", 1);
                create_info_block(&current, size);
                current += info_size + ((info_t *)current)->size;
                create_info_block(&current, save_block_size - size - info_size);
                ((info_t *)current)->is_free = 1;
                return (current - size);
            }
            ((info_t *)current)->is_free = 0;
            return (current + info_size);
        }
        current += info_size + ((info_t *)current)->size;
        // write(1, "MALLOC - BROWSE AFTER ADD : ", 28);
        // my_putnbr(((info_t *)current)->is_free);
        // write(1, "\n", 1);
    }
    // write(1, "MALLOC - AFTER BROWSE\n", 23);
    while (size + info_size >= ((info_t *)current)->size) {
        if (!create_page())
            return (NULL);
        ((info_t *)current)->size += getpagesize();
    }
    // write(1, "MALLOC - BEFORE ALLOC\n", 22);
    create_info_block(&current, size);
    current += info_size + ((info_t *)current)->size;
    diff_head_break = current - head_ptr + info_size;
    new_block_size = (((int)(diff_head_break / getpagesize()) + 1) * getpagesize()) - diff_head_break;
    // my_putnbr(new_block_size);
    // new_block_size = ((getpagesize() - ((current - head_ptr) % getpagesize()) - info_size) + getpagesize()) % getpagesize();
    create_info_block(&current, new_block_size);
    ((info_t *)current)->is_free = 2;
    // write(1, "MALLOC - I PUT 2 AT : ", 22);
    // my_putnbr((int)current);
    // write(1, "\n", 1);

    return (current - size);
}

void    *malloc(size_t size)
{
    // write(1, "MALLOC OF ", 10);
    // my_putnbr(size);
    // write(1, "\n", 1);
    if (size == 0)
        return (NULL);
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
    void *current = head_ptr;
    while (((info_t *)current)->is_free != 2) {
        if (current + info_size == ptr) {
            // write(1, "FREE\n", 5);
            ((info_t *)current)->is_free = 1;
            return;
        }
        current += info_size + ((info_t *)current)->size;
    }
}

void    *realloc(void *ptr, size_t size)
{
    // write(1, "REALLOC\n", 8);
    void *current = head_ptr;
    void *ptr2 = malloc(size);

    while (((info_t *)current)->is_free != 2) {
        if (current + info_size == ptr) {
            for (size_t i = 0; i < ((info_t *)current)->size && i < size; i++) {
                // write(1, "I\n", 2);
                ((char *)ptr2)[i] = ((char *)current)[i];
            }
            // write(1, "REALLOC BEFORE FREE\n", 20);
            free(ptr);
            break;
        }
        current += info_size + ((info_t *)current)->size;
    }
    return (ptr2);
}

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
            printf("%p - %p : %lu bytes\n", first_ptr, second_ptr, ((info_t *)current)->size);
        }
        current += info_size + ((info_t *)current)->size;
    }
}