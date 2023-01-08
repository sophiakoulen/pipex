/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 14:24:59 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/08 17:06:42 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

static int	init_cmds(t_pipex *p, char **argv);
static int	init_paths(t_pipex *p);

/*
	Initialize input redirection and output redirection.

	This means attempting to open the input and output files,
	and saving the corresponding file descriptors in the pipex structure.
*/
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

/*
	It is necessary to have accomplished the splitting of commands
	before looking for each command in the PATH variable.
*/
int	init_cmds_and_paths(t_pipex *p, char **argv)
{
	if (init_cmds(p, argv) != 0)
		return (-1);
	return (init_paths(p));
}

/*
	go extract the contents of PATH environment variable
*/
int	init_path(t_pipex *p)
{
	p->path = extract_path(environ);
	return (0);
}

/*
	Initialize the command+arguments lists.

	This means splitting each command from argv and saving
	the result in the pipex structure. 
*/
static int	init_cmds(t_pipex *p, char **argv)
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
		cmds[i] = split_cmd(argv[i + 2]);
		if (!cmds[i])
		{
			perror(0); //didn't i already handle this?
			cleanup_cmds(i, cmds);
			return (-1);
		}
		i++;
	}
	p->cmds = cmds;
	return (0);
}

/*
	Initialize the command paths.

	This means, for each command, look for it in the
	PATH variable if needed.
	This step is skipped of the status of the command is not 0,
	meaning, if there is a problem with the input or output
	file associated with that command.
*/
static int	init_paths(t_pipex *p)
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
			p->statuses[i] = find_cmd(p->path, p->cmds[i][0], &paths[i]);
		}
		i++;
	}
	p->paths = paths;
	return (0);
}
