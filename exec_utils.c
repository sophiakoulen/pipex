/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:39:52 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 15:24:27 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/*
	n is number of programs
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

int	close_used_fd(int i, t_pipex *p)
{
	if (i == 0)
	{
		close(p->input_fd);
	}
	else
	{
		close(p->pipes[i - 1][0]);
	}
	if (i == p->n_cmds - 1)
	{
		close(p->output_fd);
	}
	else
	{
		close(p->pipes[i][1]);
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

int	compute_return_value(int status)
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
