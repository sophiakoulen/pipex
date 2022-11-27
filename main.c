#include "pipex.h"

int	main(int argc, char *argv[])
{
	int				**pipes;
	int				status;
	t_command_list	cl;
	t_px_error		err;

	err = px_parse_args(argc, argv, &cl);
	if (err.status != PX_SUCCESS)
	{
		px_print_error(argv[0]);
		exit(EXIT_FAILURE);
	}

	err = px_pipe(pipe_fd);
	if (err.status != PX_SUCCESS) //overkill
	{
		px_print_error("error while opening pipe", err);
		exit(EXIT_FAILURE);
	}

	pid1 = launch_child(argv[1], 0, 2, argv[0], 0, &pipe_fd);
	pid2 = launch_child(argv[2], 1, 2, 0, argv[1], &pipe_fd);

	close(pipe_fd[0]);
	close(pipe_fd[1]);

	waitpid(pid1, 0, 0);
	waitpid(pid2, &status, 0);
	//pid2 or last one to finish??
	return (px_exit_status(status));
}

t_px_error	px_parse_args(int argc, char **argv, t_command_list *cl)
{
	t_px_error	err;
	int			i;

	err = px_set_error(PX_SUCCESS);
	if (argc < 5)
	{
		err = px_set_error(PX_WRONG_USAGE);
		return (err);
	}
	cl->size = argc - 3;
	cl->arr = malloc(sizeof(*(cl->arr)) * cl->size);
	if (!cl->arr)
	{
		err = px_set_error(PX_SEE_ERRNO);
		return (err);
	}
	i = 0;
	while (i < cl->size)
	{
		cl->arr[i].raw_string = argv[i + 1];
		i++;
	}
	cl->arr[0].infile = argv[0];
	cl->arr[cl->size - 1].outfile = argv[argc - 1];
	return (err);
}

void	cleanup_args(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = 0;
}

int px_exit_status(int status)
{
	int exit_code;
	int term_signal;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (!!exit_code);
	}
	else
	{
		term_signal = WTERMSIG(status);
		return (128 + term_signal);
	}
}

void	px_print_error(const char *str, t_px_error err)
{
	if (err.status == PX_CMD_NOT_FOUND)
	{
		ft_dprintf(2, "%s: command not found\n", str);
	}
	else if (err.status == PX_WRONG_USAGE)
	{
		ft_dprintf(2, "usage: %s infile cmd1 cmd2 outfile\n", str);
	}
	else
	{
		ft_dprintf(2, "%s: %s\n", str, strerror(err.errno_value));
	}
}

void	px_clean_exit(char *exec_path, char **args)
{
	free(exec_path);
	cleanup_args(args);
	exit(1);
}