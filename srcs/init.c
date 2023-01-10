/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:56 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 14:58:33 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	check_argc(t_pipex *p, int argc, char **argv);
static int	init_pids(t_pipex *p);
static int	init_statuses(t_pipex *p);
static int	init_pipex(t_pipex *p);

/*
	Call all initialization functions to parse the arguments
	and initialize the pipex structure.

	If check_args or init_statuses fail, we cannot continue
	executing the function.
	
	However, init_redir, init_pipes, init_cmds_and_paths and init_pids
	are independent. If one of them fails, we need to cleanup
	previously allocated memory.
*/
int	init(int argc, char **argv, t_pipex *p)
{
	int	error;

	error = 0;
	init_pipex(p);
	if (check_argc(p, argc, argv) != 0)
		return (-1);
	if (init_statuses(p) != 0)
		return (-1);
	if (init_path(p) != 0)
		return (-1);
	if (p->heredoc)
		redirect_heredoc(p);
	error |= init_redir(p) != 0;
	error |= init_pipes(p) != 0;
	error |= init_cmds_and_paths(p) != 0;
	error |= init_pids(p) != 0;
	if (error)
	{
		cleanup_pipex(p);
		return (-1);
	}
	return (0);
}

/*
	Check if the number of arguments received is correct.

	If so, we can deduce from it the number of commands we'll need
	to execute and the number of pipes we'll need to open.
*/
static int	check_argc(t_pipex *p, int argc, char **argv)
{
	const char	*msg;

	msg = "usage: %s infile cmd1 cmd2 [cmd3 ...] outfile\n\
	or\n%s here_doc DELIMITER cmd1 cmd2 [cmd3 ...] outfile\n";
	if (argc < 5)
	{
		ft_dprintf(2, msg, argv[0], argv[0]);
		return (-1);
	}
	if (ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		if (argc < 6)
		{
			ft_dprintf(2, msg, argv[0], argv[0]);
			return (-1);
		}
		else
		{
			p->heredoc = 1;
			p->n_cmds = argc - 4;
			p->n_pipes = argc - 5;
			p->raw_list = argv + 2;
		}
	}
	else
	{
		p->heredoc = 0;
		p->n_cmds = argc - 3;
		p->n_pipes = argc - 4;
		p->raw_list = argv + 1;
	}
	return (0);
}

/*
	Allocate an array to hold the process id's of the
	child processes we'll launch for each command.

	This will be used to wait for the child processes to finish
	and retrieve the exit status of the last command.
*/
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

/*
	Allocate an array to hold the status of each command.

	This will be the exit code of the child processes of each command.

	This is also used to not look into PATH for a command if we've
	already established that that file's input or output file has a problem,
	and to not try to execute a command if we've already established that
	the command could not be found in PATH.
*/
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
	p->path = 0;
	return (0);
}
