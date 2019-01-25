##
## EPITECH PROJECT, 2019
## malloc
## File description:
## Makefile
##

CC		=	gcc

CFLAGS	=	-W -Wall -Wextra -c -fpic -g3

CFLAGSDEV	= -W -Wall -Wextra -g3

SRC		=	malloc.c	\

SRCDEV	=	main.c	\

OBJ		=	$(SRC:.c=.o)

OBJDEV	=	$(SRCDEV:.c=.o)

NAME	=	libmy_malloc.so

NAMEDEV	=	malloc

all:		$(NAME)

$(NAME):	$(OBJ)
		$(CC) -shared -o $(NAME) $(OBJ)

dev:	$(OBJDEV)
		# $(CC) $(CFLAGSDEV) $(OBJDEV) -o $(NAMEDEV) -ldl
		$(CC) $(CFLAGSDEV) $(SRCDEV) -o $(NAMEDEV) -L./ -lmy_malloc

clean:
		rm -f $(OBJ)
		rm -f $(OBJDEV)

fclean:		clean
		rm -f $(NAME)
		rm -f $(NAMEDEV)

re:		fclean all
