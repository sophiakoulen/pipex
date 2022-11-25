CC = gcc
CFLAGS = -Wall -Wextra

NAME = pipex

SRCS = main.c \
px_utils.c

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