# pipex

## introduction

The aim of this project is to start working with Unix processes.
In this project, we have to reproduce Bash's pipe operator (`|`), whose effect is to redirect a commands output to be the input of another command.

## how to use

After running `make` in the project's directory, you can run commands that have the following form:

`./pipex input.txt cmd1 cmd2 output.txt`

is equivalent to Bash's:

`< input.txt cmd1 | cmd2 > output.txt`

I support multiple pipes, so:

`./pipex input.txt cmd1 cmd2 cmd3 cmd4 output.txt`

is equivalent to Bash's:

`< input.txt cmd1 | cmd2 | cmd3 | cmd4 > output.txt`

## authorized functions

- `open`: opens a file and returns a file descriptor
- `close`: closes a file pointed to by a file descriptor
- `read`: reads from a file descriptor
- `write`: writes to a file descriptor
- `malloc`: allocates memory on the heap
- `free`: frees memory on the heap
- `perror`: prints an error message to stderr
- `strerror`: returns string corresponding to an error code
- `access`: checks if we have access to a given filename
- `dup`: duplicates a file descriptor
- `dup2`: duplicates a file descriptor and gives it the value we want
- `execve`: replaces the current process with the program given
- `exit`: exits a program with a given return status
- `fork`: duplicates the current process
- `pipe`: creates a pair of file descriptors that can allow two processes to communicate
- `unlink`: remove a file
- `wait`: waits for a child process to finish
- `waitpid`: waits for a given child process to finish
