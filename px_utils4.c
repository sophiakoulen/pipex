#include "pipex.h"

/*
	Splits the a raw string representing a command with arguments into
	a heap-allocated null-terminated array containing duplicates of the
	strings.
	In case of malloc failure, an error code is returned. 
*/
t_px_error	px_split_command(char *str, char ***res)
{
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

