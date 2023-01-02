/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 13:47:35 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/02 16:39:00 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup_pipex(int argc, t_pipex *p)
{
	cleanup_pipes(argc - 3, p->pipes);
	cleanup_cmds(argc - 2, p->cmds);
	cleanup_paths(argc - 2, p->paths);
	cleanup_pids(p->pids);
	cleanup_statuses(p->statuses);
}

void	cleanup_pipes(int n, int **pipes)
{
	int i;

	if (!pipes)
		return ;
	i = 0;
	while (i < n)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	cleanup_strs(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	cleanup_cmds(int n, char ***cmds)
{
	int	i;

	i = 0;
	while (i < n)
	{
		cleanup_strs(cmds[i]);
		i++;
	}
	free(cmds);
}

void	cleanup_paths(int n, char **paths)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

void	cleanup_pids(int *pids)
{
	free(pids);
}

void	cleanup_statuses(int *statuses)
{
	free(statuses);
}