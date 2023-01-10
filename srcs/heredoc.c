/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 10:09:45 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 14:59:06 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	redirect_heredoc(t_pipex *p)
{
	p->stdout = dup(STDOUT_FILENO);
	if (pipe(p->heredoc_pipe) != 0)
		return (-1);
	dup2(p->heredoc_pipe[1], STDOUT_FILENO);
	close(p->heredoc_pipe[1]);
	return (0);
}

static int	undo_heredoc(t_pipex *p)
{
	dup2(p->stdout, STDOUT_FILENO);
	close(p->stdout);
	return (0);
}

void	heredoc(t_pipex *p)
{
	char	*line;
	size_t	len;
	char	*str;

	len = ft_strlen(p->delim);
	str = malloc(len + 2);
	if (!str)
	{
		//DO SOMETHING
		perror(0);
		exit(1);
	}
	strlcpy(str, p->delim, len + 2);
	str[len] = '\n';
	while(1)
	{
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(str, line, len + 1) == 0)
		{
			free(str);
			free(line);
			break ;
		}
		ft_putstr_fd(line, STDOUT_FILENO);
		free(line);
	}
	close(STDOUT_FILENO);
	undo_heredoc(p);
}
