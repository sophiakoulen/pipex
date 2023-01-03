/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 14:24:59 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 15:22:31 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	init_redir(t_pipex *p, char **argv)
{
	int	input_fd;
	int	output_fd;

	input_fd = open(argv[1], O_RDONLY);
	if (input_fd < 0)
	{
		perror(argv[1]);
		p->statuses[0] = 1;
	}
	output_fd = open(argv[p->n_cmds + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd < 0)
	{
		perror(argv[p->n_cmds + 2]);
		p->statuses[p->n_cmds - 1] = 1;
	}
	p->input_fd = input_fd;
	p->output_fd = output_fd;
	return (0);
}

static int	alloc_pipes(t_pipex *p)
{
	int	**pipes;
	int	i;

	pipes = ft_calloc(p->n_pipes, sizeof(*pipes));
	if (!pipes)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < p->n_pipes)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes[i])
		{
			perror(0);
			cleanup_pipes(p->n_pipes, pipes);
			return (-1);
		}
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
	p->pipes = pipes;
	return (0);
}

int	init_pipes(t_pipex *p)
{
	int	i;

	if (alloc_pipes(p) == -1)
	{
		return (-1);
	}
	i = 0;
	while (i < p->n_pipes)
	{
		if (pipe(p->pipes[i]) == -1)
		{
			perror(0);
			close_all_fd(p);
			cleanup_pipes(p->n_pipes, p->pipes);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_cmds(t_pipex *p, char **argv)
{
	char	***cmds;
	int		i;

	cmds = malloc(p->n_cmds * sizeof(*cmds));
	if (!cmds)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < p->n_cmds)
	{
		cmds[i] = ft_split2(argv[i + 2], " \t");
		if (!cmds[i])
		{
			perror(0);
			cleanup_cmds(i, cmds);
			return (-1);
		}
		i++;
	}
	p->cmds = cmds;
	return (0);
}

int	init_paths(t_pipex *p)
{
	char	**paths;
	int		i;

	paths = ft_calloc(p->n_cmds, sizeof(*paths));
	if (!paths)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < p->n_cmds)
	{
		if (p->statuses[i] == 0)
		{
			p->statuses[i] = px_find_command(p->cmds[i][0], environ, &paths[i]);
		}
		i++;
	}
	p->paths = paths;
	return (0);
}
