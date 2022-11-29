/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_pipes.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:36:03 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 12:43:56 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	Open the needed amount of pipes for n programs,
	that is, n - 1 pipes.

	If we couldn't open a pipe,
	we print and error message and return immediately with
	an error code.

	Should we immediately free the pipes if there is an error or
	let the calling process handle it????
*/
int	open_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < (n - 1))
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("error while opening pipes");
			return (1);
		}
		i++;
	}
	return (0);
}

/*
	closes the the pipe ends that are not used by the process corresponding
	to the index,
	that is, all the pipe ends except the write end of pipe[index] and the read end
	of pipe[index - 1]
*/
void	close_unused_pipe_ends(int **pipes, int index, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (i > index || i < index)
			close(pipes[i][1]);
		if (i > index - 1 || index < index - 1)
			close(pipes[i][0]);
		i++;
	}
}

/*
	close the pipe ends that would normally have been used
	if the child process hadn't encountered an error.

	This is so that he other processes can detect EOF.
*/
void	close_used_pipe_ends(int **pipes, int index, int n)
{
	if (index > 0)
	{
		close(pipes[index - 1][0]);
	}
	if (index < (n - 1))
	{
		close(pipes[index][1]);
	}
}

/*
	n is the number of programs,
	thus there are n - 1 pipes
*/
void	close_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

/*
	Allocate exactly enough memory for the amount of pipes we need,
	that is n - 1, if n is the amount of commands we'd like to run.

	In case there is a malloc failure,
	we print an error message, cleanup the array of pipes and
	return with an error code.
*/
int	allocate_pipes(int ***pipes, int n)
{
	int			i;

	*pipes = malloc(sizeof(**pipes) * (n - 1));
	if (!*pipes)
	{
		perror("malloc error");
		return (1);
	}
	i = 0;
	while (i < (n - 1))
	{
		(*pipes)[i] = malloc(sizeof(int) * 2);
		if (!(*pipes)[i])
		{
			cleanup_pipes_halfway((*pipes), i);
			perror("malloc error");
			return (1);
		}
		i++;
	}
	return (0);
}
