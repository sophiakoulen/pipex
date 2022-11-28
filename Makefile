CC = gcc
CFLAGS = -Wall -Wextra -g3 -fsanitize=address

NAME = pipex

SRCS = main.c \
px_utils1.c \
px_utils2.c \
px_utils3.c \
px_utils4.c \
px_utils5.c \
px_utils6.c \
px_utils7.c \
px_utils8.c

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