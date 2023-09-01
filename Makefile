SRCS = ./srcs/change_buff.c ./srcs/change_headers.c ./srcs/change_symbols.c ./srcs/create_code.c ./srcs/woody.c ./srcs/write_file.c ./srcs/xor.c
INCLUDES = ./includes/woody.h ./libft/libft.h ./libelf/libftelf.h
PATH_LIBFT = /home/pgoudet/Documents/ft_ping/libft
PATH_LIBFTELF = /home/pgoudet/Documents/ft_ping/libelf
LIB = ft
LIBELF = ftelf
NAME = packer
FLAGS = -Wall -Wextra -Werror


OBJS = ${SRCS:.c=.o}

%.o : %.c ${INCLUDES}
        gcc ${FLAGS} -c $< -o $@

${NAME}: ${OBJS}
        $(MAKE) -C ./libft
		$(MAKE) -C ./libelf
        gcc $(FLAGS) $(SRCS) -L${PATH_LIBFT} -l${LIB} -L${PATH_LIBFTELF} -l${LIBELF} -o ${NAME} -lm

all: $(NAME)

clean :
        rm -rf $(OBJS)
        $(MAKE) clean -C ./libft
		$(MAKE) clean -C ./libelf

fclean :        clean
        rm -rf $(NAME)
		$(MAKE) fclean -C ./libft
		$(MAKE) fclean -C ./libelf

re: fclean all

.PHONY: all fclean clean re