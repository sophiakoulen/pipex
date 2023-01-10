/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 14:08:49 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 15:01:12 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//the environ global variable contains the environment variables
extern char	**environ;

static int	wait_all(t_pipex *p);
static int	try_exec(int i, t_pipex *p);
static int	launch_all(t_pipex *p);

/*
	exec_pipeline:

	- Launch all needed child processes.
	- Close all opened file descriptors.
	- Wait for all children to finish.
	- Extract the exit code that needs to be returned.
*/
int	exec_pipeline(t_pipex *p)
{
	int	status;

	launch_all(p);
	close_all_fd(p);
	if (p->heredoc)
		heredoc(p);
	status = wait_all(p);
	return (compute_return_value(status));
}

/*
	wait_all: Wait for all launched processes to finish and
	return exit status of last command.
*/
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

/*
	try_exec: Try executing the i-th command.

	All unused pipe ends and file descriptors are closed.
	(This is necessary, else other processes might wait for
	input from those pipes).

	Redirect input and output for the to be executed command.

	If no problem has been established with the command, try
	executing the program.

	If a problem has been established or if execve failed,
	close input and output and exit with the status corresponding
	to the problem (could be 127, 126 or 1).
*/
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

/*
	launch_all: Spawn a child process for each command and try executing it.

	This is done using fork().
	If fork fails, the parent should not exit immediately, but instead wait
	for the child processes to finish.
	(Else, we might end up with zombie children.)

	Note that a child process is launched even if we've already established
	that there is a problem with the command.
	This seemed to me like a simple way to reproduce bash's behavior.
*/
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
