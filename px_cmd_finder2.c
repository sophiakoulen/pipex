/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_cmd_finder2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:48:50 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 15:56:51 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	extracts the path variable from the environment,
	in the form of a null-terminated array of strings.
	In case of malloc-failure, NULL is returned.
	If the PATH variable isn't set the environment,
	NULL is returned.
*/
char	**px_getpath(char **envp)
{
	int		i;

	if (!envp)
		return (0);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
		{
			return (ft_split(envp[i] + 5, ':'));
		}
		i++;
	}
	return (0);
}

/*
	returns true if a filename is actually a path,
	that is, the string contains at least a slash ('/').
*/
int	px_ispath(const char *str)
{
	return (!!ft_strchr(str, '/'));
}

/*
	combines the two strings into a new string path, heap-allocated.
	the resulting string is a concatenation of the two, separated by
	a slash ('/').
*/
char	*px_path_combine(const char *path_1, const char *path_2)
{
	char	*res;
	size_t	buffer_size;

	if (*path_1 == '\0' || *path_2 == '\0')
		return (0);
	buffer_size = ft_strlen(path_1) + ft_strlen(path_2) + 2;
	res = malloc(buffer_size);
	if (!res)
		return (0);
	ft_strlcpy(res, path_1, buffer_size);
	ft_strlcat(res, "/", buffer_size);
	ft_strlcat(res, path_2, buffer_size);
	return (res);
}

/*
	Checks whether we have access to an absolute path.
	If file not exists, 127 is returned,
	If file no permission, 126 is returned,
	If malloc failure, 1 is returned.
	If something else, 1 is returned.
	In any case, ann error message is printed.

	If OK, res is set to a heap-allocated duplicate of the path.
	If NOT OK, res is set to 0.
*/
int	px_check_abspath(char *abspath, char **res)
{
	*res = 0;
	if (access(abspath, X_OK) == -1)
	{
		return (errno);
	}
	*res = ft_strdup(abspath);
	if (!res)
	{
		return (errno);
	}
	return (0);
}
