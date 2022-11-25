#include "pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	int		pid1;
	int		pid2;
	int		fd[2];
	int		fdin;
	int		fdout;
	char	**args;
	char	*path;
	char	*exec_path;
	int		status;

	if (argc != 5)
	{
		ft_dprintf(2, "usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
		exit(1);
	}
	if (pipe(fd) == -1)
	{
		ft_dprintf(2, "error while opening pipe: %s\n", strerror(errno));
		exit(1);
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		ft_dprintf(2, "error while forking: %s\n", strerror(errno));
	}
	if (pid1 == 0)
	{
		fdin = open(argv[1], O_RDONLY);
		if (fdin < 0)
		{
			ft_dprintf(2, "%s: %s\n", argv[1], strerror(errno));
			exit(1);
		}
		args = ft_split2(argv[2], " \t");
		if (!args)
		{
			ft_dprintf(2, "%s: %s\n", argv[2], strerror(errno));
			exit(1);
		}
		path = 0;
		if (px_ispath(args[0]))
		{
			exec_path = args[0];
		}
		else
		{
			path = px_find_command(args[0], envp);
			exec_path = path;
			if (!path)
			{
				if (errno == ENOENT)
					ft_dprintf(2, "%s: command not found\n", args[0]);
				else
					ft_dprintf(2, "%s: %s\n", strerror(errno));
				cleanup_args(args1);
				exit(1);
			}
		}
		close(fd[0]);
		dup2(fdin, STDIN_FILENO);
		close(fdin);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (exec_path)
			execve(exec_path, args1, envp);
		ft_dprintf(2, "%s: %s\n", args[0], strerror(errno));
		free(path);
		cleanup_args(args);
		close(fd[1]);
		exit(1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		//unlink(argv[4]);
		fdout = open(argv[4], O_WRONLY | O_CREAT, 0644);
		if (fdout < 0)
		{
			ft_dprintf(2, "%s: %s\n", argv[4], strerror(errno));
			exit(1);
		}
		args = ft_split2(argv[3], " \t");
		if (!args)
		{
			ft_dprintf(2, "%s: %s\n", argv[3], strerror(errno));
			exit(1);
		}
		path = 0;
		if (px_ispath(args[0]))
		{
			exec_path = args[0];
		}
		else
		{
			path = px_find_command(args[0], envp);
			exec_path = path;
			if (!path)
			{
				if (errno == ENOENT)
					ft_dprintf(2, "%s: command not found\n", args[0]);
				else
					ft_dprintf(2, "%s: %s\n", args[0], strerror(errno));
				cleanup_args(args2);
				exit(1);
			}
		}
		close(fd[1]); //close write end of pipe
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		
		if (exec_path)
			execve(exec_path, args2, envp);
		ft_dprintf(2, "%s: %s\n", args[0], strerror(errno));
		free(path);
		cleanup_args(args);
		close(fd[0]); //close read end of pipe
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, 0, 0);
	waitpid(pid2, &status, 0);
	//pid2 or last one to finish??
	return (px_exit_status(status));
}

void	cleanup_args(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = 0;
}

int px_exit_status(int status)
{
	int exit_code;
	int term_signal;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (!!exit_code);
	}
	else
	{
		term_signal = WTERMSIG(status);
		return (128 + term_signal);
	}
}