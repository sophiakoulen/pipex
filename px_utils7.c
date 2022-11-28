#include "pipex.h"

/*
	frees the heap allocated components of the t_command
	structure
*/
void	cleanup_command(t_command *program)
{
	free(program->exec_path);
	cleanup_args(program->args);
}

/*
	frees the elements of each command that are heap-allocated.
	that is, the exec_path and the args list.
*/
void	cleanup_command_list(t_command_list *cl)
{
	int	size;
	int	i;

	size = cl->size;
	i = 0;
	while (i < size)
	{
		cleanup_command(cl->arr + i);
		i++;
	}
}

/*
	frees n pipes.
	Then frees the pipe array.
*/
void	cleanup_pipes_halfway(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}
/*
	n is the number of programs to run.
	So there are n - 1 pipes.
*/
void	cleanup_pipes(int **pipes, int n)
{
	cleanup_pipes_halfway(pipes, n - 1);
}

/*
	frees an null-terminated array of strings
*/
void	cleanup_args(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = 0;
}
