/*
** EPITECH PROJECT, 2019
** malloc
** File description:
** lib
*/

#include "malloc.h"

void	my_putchar(char c)
{
	write(1, &c, 1);
}

void    my_putstr(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        my_putchar(str[i]);
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