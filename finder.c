/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:27:32 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/05 16:55:02 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (path)
	{
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
	int		errno_value;
	char	**path_var;

	if (px_ispath(filename))
	{
		errno_value = px_check_abspath(filename, res);
		error_printer_abs(errno_value, filename);
		return (res_code(errno_value));
	}
	else
	{
		path_var = px_getpath(envp);
		errno_value = px_check_allpaths(filename, path_var, res);
		cleanup_strs(path_var);
		error_printer_cmd(errno_value, filename);
		return (res_code(errno_value));
	}
	return (0);
}
