#include "pipex.h"

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

int	px_ispath(const char *str)
{
	return (!!ft_strchr(str, '/'));
}

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

t_px_error	px_split_command(char *str, char ***res)
{
	char		**res;
	t_px_error	err;

	err.status = PX_SUCCESS;
	*res = ft_split2(str, " \t");
	if (!(*res))
	{
		err.status = PX_SEE_ERRNO;
		err.errno_value = errno;
	}
	return (err);
}

t_px_error	px_open(char *filename, int iswrite, int *fd)
{
	t_px_error	err;

	err.status = PX_SUCCESS;
	if (iswrite)
		*fd = open(filename, O_WRONLY | O_CREAT, 0644);
	else
		*fd = open(filename, O_RDONLY);
	if (*fd < 0)
	{
		err.status = PX_SEE_ERRNO;
		err.errno_value = errno;
	}
	return (err);
}

t_px_error	px_fork(int *ret)
{
	t_px_error	err;
	int			pid;

	err.status = PX_SUCCESS;
	pid = fork();
	if (pid == -1)
	{
		err.status = PX_SEE_ERRNO;
		err.errno_value = errno;
	}
	*ret = pid;
	return (err);
}

t_px_error	px_pipe(int fd[2])
{
	t_px_error	err;
	int			ret;

	err.status = PX_SUCCESS;
	ret = pipe(fd);
	if (ret == -1)
	{
		err.status = PX_SEE_ERRNO;
		err.errno_value = errno;
	}
	return (err);
}

t_px_error	px_set_error(int status)
{
	t_px_error	err;

	err.status = status;
	if (err.status == PX_SEE_ERRNO)
		err.errno_value = errno;
	return (err);
}