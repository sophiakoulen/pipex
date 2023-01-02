/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:39:52 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/02 18:03:42 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/*
	n is number of programs
*/
int	close_unused_fd(int i, int n, t_pipex *p)
{
	int	j;

	if (i != 0)
	{
		dprintf(2, "%d closed file %d\n", getpid(), p->input_redir);
		close(p->input_redir);
	}
	if (i != n - 1)
	{
		dprintf(2, "%d closed file %d\n", getpid(), p->output_redir);
		close(p->output_redir);
	}
	j = 0;
	while (j < n - 1)
	{
		if (j != i)
		{
			dprintf(2, "%d closed pipe %d\n", getpid(), p->pipes[j][1]);
			close(p->pipes[j][1]);
		}
		if (j != i - 1)
		{
			dprintf(2, "%d closed pipe %d\n", getpid(), p->pipes[j][0]);
			close(p->pipes[j][0]);
		}
		j++;
	}
	return (0);
}

int	close_used_fd(int i, int n, t_pipex *p)
{
	if (i == 0)
	{
		dprintf(2, "%d closed file %d\n", getpid(), p->input_redir);
		close(p->input_redir);
	}
	else
	{
		dprintf(2, "%d closed pipe %d\n", getpid(), p->pipes[i - 1][0]);
		close(p->pipes[i - 1][0]);
	}
	if (i == n - 1)
	{
		dprintf(2, "%d closed file %d\n", getpid(), p->output_redir);
		close(p->output_redir);
	}
	else
	{
		dprintf(2, "%d closed pipe %d\n", getpid(), p->pipes[i][1]);
		close(p->pipes[i][1]);
	}
	return (0);
}

int redirect(int i, int n, t_pipex *p)
{
	int	i_fd;
	int	o_fd;

	if (i == 0)
		i_fd = p->input_redir;
	else
		i_fd = p->pipes[i - 1][0];
	if (i == n - 1)
		o_fd = p->output_redir;
	else
		o_fd = p->pipes[i][1];
	dprintf(2, "%i has io %i %i\n", getpid(), i_fd, o_fd);
	dup2(i_fd, STDIN_FILENO);
	close(i_fd);
	dup2(o_fd, STDOUT_FILENO);
	close(o_fd);
	return (0);
}

int is_broken(int i, int n, t_pipex *p)
{
	if (i == 0)
	{
		if (p->input_redir < 0)
		{
			p->statuses[i] = 1;
			return (1);
		}
	}
	if (i == n - 1)
	{
		if (p->output_redir < 0)
		{
			p->statuses[i] = 1;
			return (1);
		}
	}
	return (0);
}

int compute_return_value(int status)
{
	int	exit_code;
	int	term_signal;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else
	{
		term_signal = WTERMSIG(status);
		return (128 + term_signal);
	}
}
