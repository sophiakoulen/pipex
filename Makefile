CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

SRCS = main.c \
init.c init_utils.c\
exec.c exec_utils.c \
finder.c finder_utils.c \
cleanup.c cleanup_utils.c

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

$(NAME): $(SRCS) libft/libft.a ft_printf/libftprintf.a
	$(CC) $(CFLAGS) $(SRCS) -o $@ -lft -lftprintf -Ilibft -I. -Llibft -Ift_printf -Lft_printf

clean:
	make clean -C libft
	make clean -C ft_printf LIBFT_PATH=../libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft
	make fclean -C ft_printf LIBFT_PATH=../libft

re: fclean all