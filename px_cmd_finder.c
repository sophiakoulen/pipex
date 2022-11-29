/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_cmd_finder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:27:32 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 12:12:17 by skoulen          ###   ########.fr       */
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
static char	**px_getpath(char **envp)
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
static int	px_ispath(const char *str)
{
	return (!!ft_strchr(str, '/'));
}

/*
	combines the two strings into a new string path, heap-allocated.
	the resulting string is a concatenation of the two, separated by
	a slash ('/').
*/
static char	*px_path_combine(const char *path_1, const char *path_2)
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

static void	error_printer_cmd(int errno_value, char *str)
{
	if (errno_value == 0)
	{
		return ;
	}
	else if (errno_value == ENOENT)
	{
		ft_dprintf(2, "%s: command not found\n", str);
	}
	else
	{
		ft_dprintf(2, "%s: %s\n", str, strerror(errno_value));
	}
}

static void	error_printer_abs(int errno_value, char *str)
{
	if (errno_value == 0)
	{
		return ;
	}
	else
	{
		ft_dprintf(2, "%s: %s\n", str, strerror(errno_value));
	}
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
static int	px_check_abspath(char *abspath, char **res)
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

/*
	Checks all paths from the PATH variable.
	If not exist, 127 is returned.
	If no permission, 126 is returned.
	If malloc error, 1 is returned.
	If something else, 1 is returned.
	In any case an error message is printed.

	If OK, res is set to the abs path.
	If NOT OK, res is set to 0.

	!!!! Error message may be innacurate if something
	else is happening.
*/
static int	px_check_allpaths(const char *filename, char **path, char **res)
{
	int		i;
	int		res_code;
	int		ret;
	char	*concat;

	ret = ENOENT;
	i = 0;
	while (path[i])
	{
		concat = px_path_combine(path[i], filename);
		if (!concat)
			return (errno);
		res_code = px_check_abspath(concat, res);
		if (res_code == 0)
			return (0);
		if (res_code != ENOENT)
			ret = res_code;
		free(concat);
		i++;
	}
	*res = 0;
	return (ret);
}

static int	res_code(int errno_value)
{
	if (errno_value == 0)
		return (0);
	if (errno_value == ENOENT)
		return (127);
	if (errno_value == ENOMEM)
		return (1);
	return (126);
}


/*
	Finds a specified command and sets a heap-allocated string containing
	the absolute path to the program.
	
	res is set to 0 if no such file with executable permissions is found,
	and an appropriate error code is returned.

	if filename is a path and the path is executable, the filename is
	duplicated in res.

	if filename is not a path and such a command is found in the PATH,
	res is set to the path to that command.

	This function prints the appropriate error message.

*/

int	px_find_command(char *filename, char **envp, char **res)
{
	int errno_value;

	if (px_ispath(filename))
	{
		errno_value = px_check_abspath(filename, res);
		error_printer_abs(errno_value, filename);
		return (res_code(errno_value));
	}
	else
	{
		errno_value = px_check_allpaths(filename, px_getpath(envp), res);
		error_printer_cmd(errno_value, filename);
		return (res_code(errno_value));
	}
	return (0);
}
