/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_cmd_prepare.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:31:12 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 11:41:56 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

extern char	**environ;

/*
	opens an input file corresponding to a program and sets the input file
	descriptor of the t_command structure accordingly.
	If opening fails, an error message is printed and an error code is returned.
*/
int	prepare_program_infile(t_command *program)
{
	program->fdin = open(program->infile, O_RDONLY);
	if (program->fdin < 0)
	{
		perror(program->infile);
		return (1);
	}
	return (0);
}

/*
	opens an output file corresponding to a program and sets the output file
	descriptor of the t_command structure accordingly.
	If opening fails, an error message is printed and an error code is returned.
*/
int	prepare_program_outfile(t_command *program)
{
	program->fdout = open(program->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (program->fdout < 0)
	{
		perror(program->outfile);
		return (1);
	}
	return (0);
}

/*
	assigns the right file descriptors to the program.
	that means:
	- compute which pipe ends will be input / output for the program
	- if the program is at the beginning or the end of the chain, open an 
	input file and / or an output file

*/
int	prepare_program_io(t_command *program, int **pipes, int index, int n)
{
	if (index == 0)
	{
		if (prepare_program_infile(program) != 0)
			return (1);
	}
	else
	{
		program->fdin = pipes[index - 1][0];
	}
	if (index == n - 1)
	{
		if (prepare_program_outfile(program) != 0)
			return (1);
	}
	else
	{
		program->fdout = pipes[index][1];
	}
	return (0);
}

static	void	cleanup_after_program(t_command_list *cl,
										int **pipes, int index, int n)
{
	cleanup_command_list(cl);
	close_used_pipe_ends(pipes, index, n);
	cleanup_pipes(pipes, n);
}

/*
	performs several operations on the program structure.
	
	1. prepares assigns the programs proper input and output file descriptors.
	2. split the raw string corresponding to the command into arguments
	3. find the absolute path of the command, if the command is valid.

	In case of failure, just return the error code
*/
void	prepare_program(t_command_list *cl, int **pipes, int index, int n)
{
	t_command	*program;
	int			res_code;

	program = cl->arr + index;
	
	if (prepare_program_io(program, pipes, index, n) != 0)
	{
		cleanup_after_program(cl, pipes, index, n);
		exit(1);
	}
	
	if (px_split_command(program->raw_string, &program->args) != 0)
	{
		cleanup_after_program(cl, pipes, index, n);
		exit(1);
	}
	
	res_code = px_find_command(program->args[0], environ, &program->exec_path);
	if (res_code != 0)
	{
		cleanup_after_program(cl, pipes, index, n);
		exit(res_code);
	}
}
