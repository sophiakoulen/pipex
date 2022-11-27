#include "pipex.h"

/*
	1. parse argc and argv into a command_list structure,
	which stores also the number of programs we're going to run

	2. Allocate space for n - 1 pipes for interprocess communication

	3. Open the previously allocated pipes

	4. For each program we'd like to run, launch a child process which
	will take care of parsing the program string, opening input or output
	files, redirecting input and output, and freeing all memory if execve
	could not be called.

	5. Close all pipes.

	6. Wait for all child processes to finish and save state of last process

	7. Free all heap-allocated memory, which in our case are the pipes and
	the command_list.

	8. Compute the exit status and return.

*/
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

	err = allocate_pipes(&pipes, cl.size);
	if (err.status != PX_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}
	err = open_pipes(pipes, cl.size);
	if (err.status != PX_SUCCESS)
	{
		cleanup_pipes(pipes, cl.size);
		exit(EXIT_FAILURE);
	}

	launch_all_children(&cl, pipes);

	close_pipes(pipes, cl.size);

	status = wait_for_all_children();

	cleanup_pipes(pipes, cl.size);
	cleanup_command_list(&cl);

	return (px_exit_status(status));
}

/*
	Open the needed amount of pipes for n programs,
	that is, n - 1 pipes.

	If we couldn't open a pipe,
	we print and error message and return immediately with
	an error code.

	Should we immediately free the pipes if there is an error or
	let the calling process handle it????
*/
t_px_error	open_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("error while opening pipes");
			return (px_set_error(PX_SEE_ERRNO));
		}
		i++;
	}
	return (px_set_error(PX_SUCCESS));
}

/*
	Allocate exactly enough memory for the amount of pipes we need,
	that is n - 1, if n is the amount of commands we'd like to run.

	In case there is a malloc failure,
	we print an error message, cleanup the array of pipes and
	return with an error code.
*/
t_px_error	allocate_pipes(int ***pipes, int n)
{
	int			i;

	err = px_set_error(PX_SUCCESS);
	pipes = malloc(sizeof(*pipes) * (n - 1));
	if (!pipes)
	{
		perror("malloc error");
		return (px_set_error(PX_SEE_ERRNO));
	}
	i = 0;
	while (i < (n - 1))
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			perror("malloc error");
			cleanup_int_tab_midway(pipes, i); //need to create this
			return (px_set_error(PX_SEE_ERRNO));
		}
		i++;
	}
	return (px_set_error(PX_SUCCESS));
}

/*
	no comment
*/
void	launch_all_children(t_command_list *cl, int **pipes)
{
	int	i;

	i = 0;
	while (i < cl->size)
	{
		launch_child(cl, pipes, i);
		i++;
	}
}

/*
	waits for all children and returns status of last process to finish
*/
int wait_for_all_children(void)
{
	int	status;

	while (wait(&status) > 0)
		;
	return (status);
}

/*
	n is the number of programs,
	thus there are n - 1 pipes
*/
void	close_pipes(int **pipes, int n)
{
	int i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/*
	counts the arguments,
	if not enough return error code.

	allocate memory in command_list
	then write arguments into command_list.
*/
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

/*
	frees an null-terminated array of strings
*/
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

/*
	computes the appropriate exit status,
	using the exit status of the last child
*/
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

/*
	print the error message correspondind to the error code,
	and an additional string
*/
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
