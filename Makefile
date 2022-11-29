CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = pipex

SRCS = main.c \
px_cleanup.c \
px_cmd_finder.c \
px_cmd_finder2.c \
px_cmd_prepare.c \
px_launcher.c \
px_main_funcs.c \
px_pipes.c \
px_splitter.c

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