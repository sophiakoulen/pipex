#include "pipex.h"

void	launch_child(t_command_list	*cl, int **pipes, int index)
{
	t_px_error	err;
	int			i;
	int			n;
	t_command	*current_program;

	current_program = &cl->arr[index];
	n = cl->size;
	err = px_fork(&current_program->pid);
	if (err != PX_SUCCESS)
	{
		px_print_error("fork failed", err); //overkill?
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close_unused_pipe_ends(pipes, index, n);

		if (i == 0) //is this the first program ?
		{
			current_program->fdin = open(current_program->infile, O_RDONLY);
			if (current_program->fdin < 0)
			{
				px_print_error(current_program->infile, px_set_error(PX_SEE_ERRNO));
				cleanup_command_list(cl);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			current_program->fdin = pipes[index - 1][0];
		}
		if (i == n - 1) //is this the last program ?
		{
			current_program->fdout = open(current_program->outfile, O_WRONLY | O_CREAT, 0644);
			if (current_program->fdout < 0)
			{
				px_print_error(current_program->outfile, px_set_error(PX_SEE_ERRNO));
				cleanup_command_list(cl);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			current_program->fdin = pipes[index][1];
		}

		//parsing the raw_string
		err = px_split_command(cmd, &args);
		if (err.status != PX_SUCCESS)
		{
			px_print_error(cmd, err);
			cleanup_command_list(cl);
			exit(EXIT_FAILURE);
		}

		err = px_find_command(args[0], environ, &exec_path);
		if (err.status != PX_SUCCESS)
		{
			px_print_error(args[0], err);
			cleanup_command_list(cl);
			exit(EXIT_FAILURE);
		}

		//and finally...
		err = px_exec_io(exec_path, args, fdin, fdout);
		if (err.status != PX_SUCCESS)
		{
			cleanup_command_list(cl);
			exit(EXIT_FAILURE);
		}
		//should not be executed
	}
	return (pid);
}

void	cleanup_command_list(t_command_list *cl)
{
	int	size;
	int	i;

	size = cl->size;
	while (i < size)
	{
		free(cl->arr[i].exec_path);
		cleanup_args(cl->arr[i].args);
		i++;
	}
}

void	close_unused_pipe_ends(int **pipes, int index, int n)
{
	int	i;

	i = 0;
	while (i < n - 1) //there are n programs, n - 1 pipes
	{
		if (i > index || i < index)
			close(pipes[i][1]); //close write end of pipe
		if (i > index - 1 || index < index - 1)
			close(pipes[i][0]);
		i++;
	}
}

t_px_error	prepare_program_io(t_command *program)
{
	t_px_error	err;

	
}

t_px_error	px_exec_io(char *exec_path, char **args, int fdin, int fdout)
{
	dup2(fdin, STDIN_FILENO);
	close(fdin);
	dup2(fdout, STDOUT_FILENO);
	close(fdout);
	if (execve(exec_path, args, envp) == -1)
	{
		return (px_set_error(PX_SEE_ERRNO));
	}
}