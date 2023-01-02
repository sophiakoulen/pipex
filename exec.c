/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:49 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/02 18:06:24 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	exec_pipeline(int argc, t_pipex *p)
{
	int	n;
	int	i;
	int	status;

	n = argc - 3;
	i = 0;
	while (i < n)
	{
		p->pids[i] = fork();
		dprintf(2, "%i is %s\n", p->pids[i], p->cmds[i][0]);
		if (p->pids[i] == -1)
		{
			perror(0);
			//EXIT
			exit(EXIT_FAILURE);
		}
		if (p->pids[i] == 0) //in child process
		{
			close_unused_fd(i, n, p);
			redirect(i, n, p);
			if (p->statuses[i] == 0 && !is_broken(i, n, p))
			{
				execve(p->paths[i], p->cmds[i], environ);
				perror(0);
				p->statuses[i] = 1;
			}
			dprintf(2, "hello\n");
			close(0);
			close(1);
			exit(p->statuses[i]);
		}
		i++;
	}
	i = 0;
	while (i < n)
	{
		waitpid(p->pids[i], &status, 0);
		i++;
	}
	return (compute_return_value(status));
}
