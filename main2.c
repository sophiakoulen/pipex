#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
# include <string.h>
# include <stdio.h>
// void func(void)
// {
// 	//int fd = open("file", O_CREAT | O_WRONLY, 0644);
// 	write(1, "hello\n", 7);
// 	//close(fd);
// }

int main(int argc, char **argv, char **envp)
{
	int pid = fork();
	int fd = open("file", O_CREAT | O_WRONLY, 0644);
	if (pid == 0)
	{
		dup2(fd, 1);
		close(fd);
		char *args[] = {0};
		execve("./lol", args, envp);
		perror(0);
	}
	wait(0);
	return (0);
}