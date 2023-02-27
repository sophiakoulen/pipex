NAME = pipex

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = $(addprefix srcs/, main.c \
init.c init_utils.c init_pipes.c\
exec.c exec_utils.c \
find_cmd.c find_cmd_utils.c \
cleanup.c cleanup_utils.c \
split_cmd.c split_cmd_utils.c)

OBJS = $(SRCS:srcs/%.c=objs/%.o)

ifdef DEBUG
	CFLAGS += -g3 -fsanitize=address
endif

INCLUDES_PATH = -I. -Ilibft/includes -Ift_printf

all: $(NAME)

libft/libft.a:
	make -C libft

ft_printf/libftprintf.a:
	make -C ft_printf LIBFT_PATH=../libft

$(NAME): $(OBJS) libft/libft.a ft_printf/libftprintf.a
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lft -lftprintf -Llibft -Lft_printf

bonus: $(NAME)

objs/%.o: srcs/%.c
	mkdir -p objs
	$(CC) $(CFLAGS) $(INCLUDES_PATH) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -rf objs
	make clean -C libft
	make clean -C ft_printf LIBFT_PATH=../libft

fclean: clean
	rm -f $(NAME)
	make fclean -C libft
	make fclean -C ft_printf LIBFT_PATH=../libft

re: fclean all

.PHONY: all clean fclean re