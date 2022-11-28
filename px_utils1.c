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
int	px_ispath(const char *str)
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
	strlcpy(res, path_1, buffer_size);
	strlcat(res, "/", buffer_size);
	strlcat(res, path_2, buffer_size);
	return (res);
}

/*
	searches for a program with executable permissions in the path
	variable specified.
	If such a program is found, the absolute path, heap-allocated, is returned.
	Else, 0 is returned, and errno is set to ENOENT is no such file exists,
	or to EACCESS if such a file exists but the user doesn't have permission
	to execute that file.
*/
static char	*px_search_path(const char *filename, char **path)
{
	char	*concat;
	int		i;
	int		exist;

	errno = 0;
	exist = 0;
	i = 0;
	while (path[i])
	{
		concat = px_path_combine(path[i], filename);
		if (!concat)
			return (0);
		if (access(concat, F_OK) == 0)
			exist = 1;
		if (access(concat, X_OK) == 0)
		{
			errno = 0;
			return (concat);
		}
		free(concat);
		i++;
	}
	if (exist)
		errno = EACCES;
	return (0);
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
*/
t_px_error	px_find_command(char *filename, char **envp, char **res)
{
	t_px_error	err;

	err = px_set_error(PX_SUCCESS);
	if (px_ispath(filename))
	{
		if (access(filename, X_OK) == -1)
			err = px_set_error(PX_SEE_ERRNO);
		*res = ft_strdup(filename);
		if (!*res)
			err = px_set_error(PX_SEE_ERRNO);
	}
	else
	{
		*res = px_search_path(filename, px_getpath(envp));
		if (!*res)
		{
			if (errno == ENOENT)
				err = px_set_error(PX_CMD_NOT_FOUND);
			else
				err = px_set_error(PX_SEE_ERRNO);
		}
	}
	return (err);
}