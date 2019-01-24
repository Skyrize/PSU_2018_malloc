/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** malloc
*/

#ifndef MALLOC_H_
	#define MALLOC_H_

typedef struct info_s {
	char	is_free;
	size_t	size;
} __attribute__((packed)) info_t;

void	show_alloc_mem();

#endif /* !MALLOC_H_ */
