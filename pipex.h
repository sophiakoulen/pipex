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

typedef struct s_px_error
{
	int	status;
	int errno_value;
}	t_px_error;

typedef struct s_command
{
	char	*raw_string;
	char	*infile;
	char	*outfile;
	char	**args;
	char	*exec_path;
	int		fdin;
	int		fdout;
	int		pid;
} t_command;

typedef	struct s_command_list
{
	int			size;
	t_command	*arr;
} t_command_list;

# define PX_SUCCESS	0
# define PX_CMD_NOT_FOUND 1
# define PX_SEE_ERRNO 2
# define PX_WRONG_USAGE 3

# define READ 0
# define WRITE 1

void		cleanup_args(char **tab);
t_px_error	px_find_command(char *filename, char **envp, char **res);
int			px_ispath(const char *str);

#endif