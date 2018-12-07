ROOT_DIR	=	$(realpath .)

SRC	=	$(ROOT_DIR)/cd.c \
		$(ROOT_DIR)/exit.c \
		$(ROOT_DIR)/minishell.c \
		$(ROOT_DIR)/utils.c\

CC	=	gcc

NAME	=	minishell

OBJ	=	$(SRC:.c=.o)

CFLAGS	+=	-std=c99 -fPIC -Wall -Wextra ##-Werror

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
