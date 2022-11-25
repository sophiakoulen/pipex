#include "pipex.h"

int	main(int argc, char *argv[], char *envp[])
{
	int		pid1;
	int		pid2;
	int		fd[2];
	int		fdin;
	int		fdout;
	char	**args1;
	char	**args2;
	char	*path;
	char	*exec_path;

	if (argc != 5)
	{
		ft_dprintf(2, "usage: %s infile cmd1 cmd2 outfile\n", argv[0]);
		exit(1);
	}
	if (pipe(fd))
		return (1);
	pid1 = fork();
	if (pid1 == 0)
	{
		fdin = open(argv[1], O_RDONLY);
		if (fdin < 0)
			exit(1);
		args1 = ft_split2(argv[2], " \t");
		if (!args1)
			exit(1);
		

		path = 0;
		if (px_ispath(args1[0]))
		{
			exec_path = args1[0];
		}
		else
		{
			path = px_find_command(args1[0], envp);
			exec_path = path;
		}

		close(fd[0]);
		dup2(fdin, STDIN_FILENO);
		close(fdin);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		
		if (exec_path)
			execve(exec_path, args1, envp);
		
		//print error message corresponding to errno
		free(path);
		cleanup_args(args1);
		close(fd[1]);
		exit(1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		fdout = open(argv[4], O_WRONLY | O_CREAT, 0644);
		if (fdout < 0)
		{
			exit(1);
		}
		args2 = ft_split2(argv[3], " \t");
		if (!args2)
			exit(1);

		path = 0;
		if (px_ispath(args2[0]))
		{
			exec_path = args2[0];
		}
		else
		{
			path = px_find_command(args2[0], envp);
			exec_path = path;
		}

		close(fd[1]); //close write end of pipe
		dup2(fdout, STDOUT_FILENO);
		close(fdout);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		
		if (exec_path)
			execve(path, args2, envp);
		//an error occured
		free(path);
		cleanup_args(args2);
		close(fd[0]); //close read end of pipe
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, 0, 0);
	waitpid(pid2, 0, 0);
	return (0);
}

void	cleanup_args(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		free(tab[i]);
	free(tab);
}