#include "pipex.h"

/*
	forks,
	prepares the process corresponding to the given index in the command_list
	and executes it
	in case of failure, all memory allocated must be freed. IN this case,
	that means, the command_list and the pipes.

*/
void	launch_child(t_command_list	*cl, int **pipes, int index)
{
	int			n;
	t_command	*current_program;

	current_program = &cl->arr[index];
	n = cl->size;
	current_program->pid = fork();
	if (current_program->pid == -1)
	{
		perror("fork failed"); //overkill?
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		close_unused_pipe_ends(pipes, index, n);
		if (prepare_program(current_program, pipes, index, n).status != PX_SUCCESS)
		{
			cleanup_command_list(cl);
			cleanup_pipes(pipes);
			exit(EXIT_FAILURE);
		}
		if (px_exec_io(current_program).status != PX_SUCCESS)
		{
			cleanup_command_list(cl);
			cleanup_pipes(pipes);
			exit(EXIT_FAILURE);
		}
	}
	return (pid);
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
	while (i < size)
	{
		free(cl->arr[i].exec_path);
		cleanup_args(cl->arr[i].args);
		i++;
	}
}

/*
	closes the the pipe ends that are not used by the process corresponding to the index,
	that is, all the pipe ends except the write end of pipe[index] and the read end
	of pipe[index - 1]
*/
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

/*
	performs several operations on the program structure.
	
	1. prepares assigns the programs proper input and output file descriptors.
	2. split the raw string corresponding to the command into arguments
	3. find the absolute path of the command, if the command is valid.

	In case of failure, just return the error code
*/
t_px_error	prepare_program(t_command *program, int **pipes, int index, int n)
{
	t_px_error	err;

	err = prepare_program_io(program, pipes, index, n);
	if (err.status != PX_SUCCESS)
	{
		return (err);
	}
	err = px_split_command(program->raw_string, program->args);
	if (err.status != PX_SUCCESS)
	{
		px_print_error(program->raw_string, err);
		return (err);
	}
	err = px_find_command(program->args[0], environ, program->exec_path);
	if (err.status != PX_SUCCESS)
	{
		px_print_error(program->args[0], err);
		return (err);
	}
}

/*
	assigns the right file descriptors to the program.
	that means:
	- compute which pipe ends will be input / output for the program
	- if the program is at the beginning or the end of the chain, open an 
	input file and / or an output file

*/
t_px_error	prepare_program_io(t_command *program, int **pipes, int index, int n)
{
	t_px_error	err;

	err = px_set_error(PX_SUCCESS);
	if (index == 0)
	{
		err = prepare_program_infile(program, index, n);
	}
	else
	{
		program->fdin = pipes[index - 1][0];
	}
	if (index == n - 1)
	{
		err = prepare_program_outfile(program, index, n);
	}
	else
	{
		program->fdout = pipes[index][1];
	}
	return (err);
}

/*
	opens an input file corresponding to a program and sets the input file
	descriptor of the t_command structure accordingly.
	If opening fails, an error message is printed and an error code is returned.
*/
t_px_error	prepare_program_infile(t_command *program, int index, int n)
{
	t_px_error	err;

	err = px_set_error(PX_SUCCESS);
	current_program->fdin = open(current_program->infile, O_RDONLY);
	if (current_program->fdin < 0)
	{
		err = px_set_error(PX_SEE_ERRNO);
		px_print_error(current_program->infile, err);
	}
	return (err);
}

/*
	opens an output file corresponding to a program and sets the output file
	descriptor of the t_command structure accordingly.
	If opening fails, an error message is printed and an error code is returned.
*/
t_px_error	prepare_program_outfile(t_command *program, int index, int n)
{
	t_px_error	err;

	err = px_set_error(PX_SUCCESS);
	current_program->fdout = open(current_program->outfile, O_WRONLY | O_CREAT, 0644);
	if (current_program->fdout < 0)
	{
		err = px_set_error(PX_SEE_ERRNO);
		px_print_error(current_program->outfile, px_set_error(PX_SEE_ERRNO));
	}
	return (err);
}

/*
	dupplicates and closes the input and output file descriptors of the
	program so that the input file descriptor becomes the input and the output
	file descriptor becomes the output.
	Then, execve is called.
	If it fails, an error code is returned.
*/
t_px_error	px_exec_io(t_command *program)
{
	dup2(program->fdin, STDIN_FILENO);
	close(program->fdin);
	dup2(program->fdout, STDOUT_FILENO);
	close(program->fdout);
	if (execve(program->exec_path, program->args, environ) == -1)
	{
		//error message could be done immediately
		return (px_set_error(PX_SEE_ERRNO));
	}
}