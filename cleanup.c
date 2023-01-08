/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 13:47:35 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/08 17:06:59 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	cleanup_pids(int *pids);
static void	cleanup_statuses(int *statuses);
static void	cleanup_path(char **path);

void	cleanup_pipex(t_pipex *p)
{
	cleanup_pipes(p->n_pipes, p->pipes);
	cleanup_cmds(p->n_cmds, p->cmds);
	cleanup_paths(p->n_cmds, p->paths);
	cleanup_pids(p->pids);
	cleanup_statuses(p->statuses);
	cleanup_path(p->path);
}

static void	cleanup_pids(int *pids)
{
	free(pids);
}

static void	cleanup_statuses(int *statuses)
{
	free(statuses);
}

static void	cleanup_path(char **path)
{
	cleanup_strs(path);
}
