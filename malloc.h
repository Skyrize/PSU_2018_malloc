/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** malloc
*/

#ifndef MALLOC_H_
	#define MALLOC_H_

#include <unistd.h>
#include <stdlib.h>

void    *head_ptr;

typedef struct info_s {
	char	is_free;
	size_t	size;
} __attribute__((packed)) info_t;

void	my_putchar(char c);
void	my_putstr(char *str);
int		my_putnbr(int nb);
int 	my_strlen(char *base);
void 	my_putnbr_base(unsigned long long nb, char *base);

void    *create_page(size_t page_size);
void    create_info_block(void * *ptr, size_t size);

void    show_alloc_mem();

#endif /* !MALLOC_H_ */
