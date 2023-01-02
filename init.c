/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:56 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/02 17:47:21 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

int	check_argc(int argc, char **argv)
{
	if (argc < 5)
	{
		ft_dprintf(2,
			"usage: %s infile cmd1 cmd2 [cmd3 ...] outfile\n", argv[0]);
		return (-1);
	}
	else
	{
		return (0);
	}
}

int	init_redir(int argc, char **argv, t_pipex *p)
{
	int	input_redir;
	int	output_redir;

	input_redir = open(argv[1], O_RDONLY);
	if (input_redir < 0)
	{
		perror(argv[1]);
	}
	output_redir = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_redir < 0)
	{
		perror(argv[argc - 1]);
	}
	p->input_redir = input_redir;
	p->output_redir = output_redir;
	return (0);
}

int init_pipes(int n, t_pipex *p)
{
	int	**pipes;
	int	i;

	pipes = malloc(n * sizeof(*pipes));
	if (!pipes)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < n)
	{
		pipes[i] = malloc(2 * sizeof(int));
		if (!pipes[i])
		{
			perror(0);
			cleanup_pipes(n, pipes);
			return (-1);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror(0);
			cleanup_pipes(n, pipes); //need to close first!!
			return (-1);
		}
		i++;
	}
	p->pipes = pipes;
	return (0);
}

int	init_cmds(int argc, char **argv, t_pipex *p)
{
	char	***cmds;
	int		n;
	int		i;

	n = argc - 3;
	cmds = malloc(n * sizeof(*cmds));
	if (!cmds)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < n)
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

int	init_paths(int argc, t_pipex *p)
{
	char	**paths;
	int		n;
	int		i;

	n = argc - 3;
	paths = malloc(n * sizeof(*paths));
	if (!paths)
	{
		perror(0);
		return (-1);
	}
	i = 0;
	while (i < n)
	{
		p->statuses[i] = px_find_command(p->cmds[i][0], environ, &paths[i]);
		i++;
	}
	p->paths = paths;
	return (0);
}

int	init_pids(int argc, t_pipex *p)
{
	int	*pids;
	int	n;

	n = argc - 3;
	pids = malloc(n * sizeof(*pids));
	if (!pids)
	{
		perror(0);
		return (-1);
	}
	p->pids = pids;
	return (0);
}

int	init_statuses(int argc, t_pipex *p)
{
	int	*statuses;
	int	n;

	n = argc - 3;
	statuses = malloc(n * sizeof(*statuses));
	if (!statuses)
	{
		perror(0);
		return (-1);
	}
	p->statuses = statuses;
	return (0);
}

int init(int argc, char **argv, t_pipex *p)
{
	if (check_argc(argc, argv) != 0)
		return (-1);
	if (init_statuses(argc, p) != 0) //allocates mem
	{
		cleanup_pipex(argc, p);
		return (-1);
	}
	if (init_redir(argc, argv, p) != 0)
		return (-1);
	if (init_pipes(argc - 4, p) != 0) //allocates mem
	{
		cleanup_pipex(argc, p);
		return (-1);
	}
	if (init_cmds(argc, argv, p) != 0) //allocates mem
	{
		cleanup_pipex(argc, p);
		return (-1);
	}
	if (init_paths(argc, p) != 0) //allocates mem
	{
		cleanup_pipex(argc, p);
		return (-1);
	}
	if (init_pids(argc, p) != 0) //allocates mem
	{
		cleanup_pipex(argc, p);
		return (-1);
	}
	return (0);
}
