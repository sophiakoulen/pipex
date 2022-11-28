#include "pipex.h"

/*
	print the error message correspondind to the error code,
	and an additional string
*/
void	px_print_error(const char *str, t_px_error err)
{
	if (err.status == PX_CMD_NOT_FOUND)
	{
		ft_dprintf(2, "%s: command not found\n", str);
	}
	else if (err.status == PX_WRONG_USAGE)
	{
		ft_dprintf(2, "usage: %s infile cmd1 cmd2 outfile\n", str);
	}
	else
	{
		ft_dprintf(2, "%s: %s\n", str, strerror(err.errno_value));
	}
}

/*
	Returns an error structure composed by 
	a status that can be PX_SUCCESS, PX_WRONG_USAGE,
	PX_SEE_ERRNO or PX_CMD_NOT_FOUND.
	
	If status is PX_SEE_ERNNO, the errno_value is
	set to the current value of global variable errno.
*/
t_px_error	px_set_error(int status)
{
	t_px_error	err;

	err.status = status;
	if (err.status == PX_SEE_ERRNO)
		err.errno_value = errno;
	return (err);
}