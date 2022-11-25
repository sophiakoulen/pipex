#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <unistd.h>
# include <sys/wait.h>

# include "libft.h"
# include "ft_printf.h"

void	cleanup_args(char **tab);
char	*px_find_command(char *filename, char **envp);
int		px_ispath(const char *str);

#endif