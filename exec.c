/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:49 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 13:41:40 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

static int	wait_all(t_pipex *p)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	while (i < p->n_cmds)
	{
		waitpid(p->pids[i], &status, 0);
		i++;
	}
	return (status);
}

static int	try_exec(int i, t_pipex *p)
{
	close_unused_fd(i, p);
	redirect(i, p);
	if (p->statuses[i] == 0)
	{
		if (execve(p->paths[i], p->cmds[i], environ) == -1)
		{
			perror("failed to execute program");
			p->statuses[i] = 1;
		}
	}
	close(0);
	close(1);
	exit(p->statuses[i]);
}

static int	launch_all(t_pipex *p)
{
	int	i;

	i = 0;
	while (i < p->n_cmds)
	{
		p->pids[i] = fork();
		if (p->pids[i] == -1)
		{
			perror("forking failed");
			return (1);
		}
		if (p->pids[i] == 0)
		{
			try_exec(i, p);
		}
		i++;
	}
	return (0);
}

int	exec_pipeline(t_pipex *p)
{
	int	status;

	launch_all(p);
	close_all_fd(p);
	status = wait_all(p);
	return (compute_return_value(status));
}
