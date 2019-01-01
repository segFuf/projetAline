SRC	=	commands.c \
		exec.c \
		minishell.c \
		utils.c\
		redirections.c\

CC	=	gcc

NAME	=	minishell

OBJ	=	$(SRC:.c=.o)

CFLAGS	+=	-std=c99 -fPIC -Wextra -D_XOPEN_SOURCE=700 

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re
