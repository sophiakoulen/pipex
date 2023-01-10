/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:39:52 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 11:28:43 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	Close the file descriptors that will not be used by
	the i-th process.

	That means close all opened file descriptors except the two
	that will be used for input and output.
*/
int	close_unused_fd(int i, t_pipex *p)
{
	int	j;

	if (i != 0)
	{
		close(p->input_fd);
	}
	if (i != p->n_cmds - 1)
	{
		close(p->output_fd);
	}
	j = 0;
	while (j < p->n_pipes)
	{
		if (j != i)
		{
			close(p->pipes[j][1]);
		}
		if (j != i - 1)
		{
			close(p->pipes[j][0]);
		}
		j++;
	}
	return (0);
}

int	close_all_fd(t_pipex *p)
{
	int	i;

	close(p->input_fd);
	close(p->output_fd);
	i = 0;
	while (i < p->n_cmds - 1)
	{
		close(p->pipes[i][0]);
		close(p->pipes[i][1]);
		i++;
	}
	return (0);
}

/*
	Redirect input and output using dup2.

	It works like this:
	1) dup2 will make STDIN_FILENO (usually 0), the file descriptor
	associated to standard input, point to the same object as the file
	descriptor we actually want to read from.
	2) Now, we have to file descriptors pointing to the same object,
	STDIN_FILENO and the other file descriptor. So now we can close that
	other file descriptor.
	Same goes for STDOUT_FILENO.

*/
int	redirect(int i, t_pipex *p)
{
	int	i_fd;
	int	o_fd;

	if (i == 0)
		i_fd = p->input_fd;
	else
		i_fd = p->pipes[i - 1][0];
	if (i == p->n_pipes)
		o_fd = p->output_fd;
	else
		o_fd = p->pipes[i][1];
	dup2(i_fd, STDIN_FILENO);
	close(i_fd);
	dup2(o_fd, STDOUT_FILENO);
	close(o_fd);
	return (0);
}

/*
	Compute the value our pipex program needs to return based
	on the exit status of the last command.

	There are 2 possibilities:
	1) Child processes exited normally, and then we need to return
	the exit status of that process.
	2) Execution of child process was interrupted by a signal, and then
	we need to return (128 + sig_num), sig_num being the signal code.
*/
int	compute_return_value(int status)
{
	int	exit_code;
	int	sig_num;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else
	{
		sig_num = WTERMSIG(status);
		return (128 + sig_num);
	}
}
