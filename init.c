/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:56 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 15:24:04 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	check_argc(t_pipex *p, int argc, char **argv)
{
	const char	*msg;

	msg = "usage: %s infile cmd1 cmd2 [cmd3 ...] outfile\n";
	if (argc < 5)
	{
		ft_dprintf(2, msg, argv[0]);
		return (-1);
	}
	else
	{
		p->n_cmds = argc - 3;
		p->n_pipes = argc - 4;
		return (0);
	}
}

static int	init_pids(t_pipex *p)
{
	int	*pids;

	pids = malloc(p->n_cmds * sizeof(*pids));
	if (!pids)
	{
		perror(0);
		return (-1);
	}
	p->pids = pids;
	return (0);
}

static int	init_statuses(t_pipex *p)
{
	int	*statuses;

	statuses = ft_calloc(p->n_cmds, sizeof(*statuses));
	if (!statuses)
	{
		perror(0);
		return (-1);
	}
	p->statuses = statuses;
	return (0);
}

static int	init_pipex(t_pipex *p)
{
	p->input_fd = -1;
	p->output_fd = -1;
	p->statuses = 0;
	p->pids = 0;
	p->cmds = 0;
	p->paths = 0;
	p->pipes = 0;
	p->n_cmds = 0;
	p->n_pipes = 0;
	return (0);
}

int	init(int argc, char **argv, t_pipex *p)
{
	int	error;

	error = 0;
	if (init_pipex(p) != 0)
		return (-1);
	if (check_argc(p, argc, argv) != 0)
		return (-1);
	if (init_statuses(p) != 0)
		return (-1);
	error |= init_redir(p, argv) != 0;
	error |= init_pipes(p) != 0;
	error |= init_cmds(p, argv) != 0;
	error |= init_paths(p) != 0;
	error |= init_pids(p) != 0;
	if (error)
	{
		cleanup_pipex(p);
		return (-1);
	}
	return (0);
}
