/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_launcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:26:38 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 12:41:32 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

/*
	dupplicates and closes the input and output file descriptors of the
	program so that the input file descriptor becomes the input and the output
	file descriptor becomes the output.
	Then, execve is called.
	If it fails, an error code is returned.
*/
static int	px_exec_io(t_command *program)
{
	dup2(program->fdin, STDIN_FILENO);
	close(program->fdin);
	dup2(program->fdout, STDOUT_FILENO);
	close(program->fdout);
	if (execve(program->exec_path, program->args, environ) == -1)
	{
		return (1);
	}
	return (0);
}

static void	replace_process(t_command_list *cl, int **pipes, int index)
{
	int			n;
	t_command	*current_program;

	n = cl->size;
	current_program = cl->arr + index;

	close_unused_pipe_ends(pipes, index, n);
	
	prepare_program(cl, pipes, index, n);
	
	if (px_exec_io(current_program) != 0)
	{
		cleanup_command_list(cl);
		close_unused_pipe_ends(pipes, index, n);
		cleanup_pipes(pipes, n);
		exit(EXIT_FAILURE);
	}
}

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
		perror("fork failed");
		cleanup_command_list(cl);
		close_pipes(pipes, cl->size);
		cleanup_pipes(pipes, cl->size);
		exit(EXIT_FAILURE);
	}
	if (current_program->pid == 0)
	{
		replace_process(cl, pipes, index);
	}
}
