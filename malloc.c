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

const size_t info_size = sizeof(info_t);

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

int my_strlen(char *base)
{
    int len = 0;

    for (; base[len]; len++);
    return len;
}

void my_putnbr_base(unsigned long long nb, char *base)
{
	unsigned long long length = my_strlen(base);

	if (nb >= length) {
		my_putnbr_base(nb / length, base);
	}
	my_putchar(base[nb % length]);
}

void    *create_page()
{
    void *ptr = sbrk(getpagesize());

    if (!ptr)
        return (NULL);
    return (ptr);
}

void    create_info_block(void * *ptr, size_t size)
{
    info_t *info_block = *ptr;

    info_block->is_free = 0; 
    info_block->size = size;
}

void    *browse_alloc(size_t size)
{
    void *current = head_ptr;
    int new_block_size = 0;
    int diff_head_break = 0;
    int save_block_size = 0;

    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 1 &&
        size <= ((info_t *)current)->size) {
            save_block_size = ((info_t *)current)->size;
            if (save_block_size - size >= 10) {
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
    }
    while (size + info_size >= ((info_t *)current)->size) {
        if (!create_page())
            return (NULL);
        ((info_t *)current)->size += getpagesize();
    }
    create_info_block(&current, size);
    current += info_size + ((info_t *)current)->size;
    diff_head_break = current - head_ptr + info_size;
    new_block_size = (((int)(diff_head_break / getpagesize()) + 1) * getpagesize()) - diff_head_break;
    create_info_block(&current, new_block_size);
    ((info_t *)current)->is_free = 2;

    return (current - size);
}

void    *malloc(size_t size)
{
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

void     free_shrink(void *ptr_free)
{
    void *current = ptr_free;
    int nb_shrink = 0;

    current += info_size + ((info_t *)current)->size;
    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 0) {
            ((info_t *)current)->is_free = 1;
            write(1, "FREE - DO NOT SHRINK\n", 21);
            return;
        }
        current += info_size + ((info_t *)current)->size;
    }
    nb_shrink = (current - ptr_free + ((info_t *)current)->size + info_size) / getpagesize();
    if (nb_shrink > 0) {
        write(1, "FREE - SHRINK : ", 16);
        my_putnbr(nb_shrink);
        write(1, "\n", 1);

        sbrk(-nb_shrink);
    }

    write(1, "FREE - PTR FREE : ", 18);
    my_putnbr(((info_t *)ptr_free)->size);
    write(1, "\n", 1);
    write(1, "FREE - ADD TO LAST : ", 21);
    my_putnbr(((info_t *)current)->size + info_size - (getpagesize() * nb_shrink));
    write(1, "\n", 1);

    ((info_t *)ptr_free)->size += ((info_t *)current)->size + info_size - (getpagesize() * nb_shrink);
    
    write(1, "FREE - PTR FREE AFTER : ", 24);
    my_putnbr(((info_t *)ptr_free)->size);
    write(1, "\n", 1);

    ((info_t *)ptr_free)->is_free = 2;
    if (((info_t *)ptr_free)->size == -info_size)
        head_ptr = NULL;
}

void    free(void *ptr)
{
    void *current = head_ptr;

    while (((info_t *)current)->is_free != 2) {
        if (current + info_size == ptr) {
            // write(1, "FREE\n", 5);
            free_shrink(current);
            return;
        }
        current += info_size + ((info_t *)current)->size;
    }
}

void    *realloc(void *ptr, size_t size)
{
    void *current = head_ptr;
    void *ptr2 = malloc(size);

    while (((info_t *)current)->is_free != 2) {
        if (current + info_size == ptr) {
            for (size_t i = 0; i < ((info_t *)current)->size && i < size; i++) {
                ((char *)ptr2)[i] = ((char *)current)[i];
            }
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

    if (!head_ptr)
        return;
    while (((info_t *)current)->is_free != 2)
        current += info_size + ((info_t *)current)->size;
    current += info_size + ((info_t *)current)->size;
    write(1, "break : 0x", 11);
    my_putnbr_base((unsigned long long)current, "0123456789abcdef");
    write(1, "\n", 1);
    current = head_ptr;
    while (((info_t *)current)->is_free != 2) {
        if (((info_t *)current)->is_free == 0) {
            first_ptr = current + info_size;
            second_ptr = current + info_size + ((info_t *)current)->size;
            write(1, "0x", 2);
            my_putnbr_base((unsigned long long)first_ptr, "0123456789abcdef");
            write(1, " - 0x", 6);
            my_putnbr_base((unsigned long long)second_ptr, "0123456789abcdef");
            write(1, " : ", 4);
            my_putnbr(((info_t *)current)->size);
            write(1, " bytes\n", 8);
        }
        current += info_size + ((info_t *)current)->size;
    }
}