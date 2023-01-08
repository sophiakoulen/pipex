/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 15:27:42 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 15:30:08 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup_pipes(int n, int **pipes)
{
	int	i;

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

	if (!strs)
		return ;
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

	if (!cmds)
		return ;
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

	if (!paths)
		return ;
	i = 0;
	while (i < n)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}
