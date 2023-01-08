/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 15:58:06 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/08 15:00:12 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	alloc_pipes(t_pipex *p);

/*
	Open the pipes we'll need.

	If there is a problem while trying to open a pipe, we'll
	need to close all previously allocated pipes and free the
	associated heap allocated memory.
*/
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

/*
	Allocate an array of pipes.

	If there are n commands, there'll be n - 1 pipes.

	A pipe is a pair of file descriptors. They will be used
	to redirect to standard output of a command to the standard
	input of another command.
*/
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
