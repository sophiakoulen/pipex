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

char	*px_find_command(char *filename, char **envp)
{
	return (px_search_path(filename, px_getpath(envp)));
}