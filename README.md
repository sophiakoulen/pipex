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