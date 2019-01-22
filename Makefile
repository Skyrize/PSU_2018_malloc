##
## EPITECH PROJECT, 2019
## malloc
## File description:
## Makefile
##

CC		=	gcc

CFLAGS	=	-W -Wall -Wextra -g3

SRC		=	malloc.c	\

SRCDEV	=	main.c	\

OBJ		=	$(SRC:.c=.o)

OBJDEV	=	$(SRCDEV:.c=.o)

NAME	=	libmy_malloc.so

NAMEDEV	=	malloc

all:		$(NAME)

$(NAME):
		$(CC) -c $(CFLAGS) -fpic $(SRC)
		$(CC) -shared -o $(NAME) $(OBJ)

dev:		$(OBJDEV)
		$(CC) $(CFLAGS) $(SRCDEV) -o $(NAMEDEV) -ldl

clean:
		rm -f $(OBJ)
		rm -f $(OBJDEV)

fclean:		clean
		rm -f $(NAME)
		rm -f $(NAMEDEV)

re:		fclean all
