CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

SRCS = main.c \
init.c init_utils.c init_pipes.c\
exec.c exec_utils.c \
find_cmd.c find_cmd_utils.c \
cleanup.c cleanup_utils.c \
splitter.c splitter_utils.c

OBJS = $(SRCS:.c=.o)

DEBUG=1
FSAN=1

ifdef DEBUG
	CFLAGS += -g3
endif

ifdef FSAN
	CFLAGS += -fsanitize=address
endif

all: $(NAME)

libft/libft.a:
	make -C libft

ft_printf/libftprintf.a:
	make -C ft_printf LIBFT_PATH=../libft

$(NAME): $(OBJS) libft/libft.a ft_printf/libftprintf.a
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lft -lftprintf -Llibft -Lft_printf

.c.o:
	$(CC) $(CFLAGS) -I. -Ilibft -Ift_printf -c $<

clean:
	rm -f $(OBJS)
	make clean -C libft
	make clean -C ft_printf LIBFT_PATH=../libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft
	make fclean -C ft_printf LIBFT_PATH=../libft

re: fclean all

.PHONY: all clean fclean re