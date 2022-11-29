/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:16:59 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/29 12:38:44 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	1. parse argc and argv into a command_list structure,
	which stores also the number of programs we're going to run

	2. Allocate space for n - 1 pipes for interprocess communication

	3. Open the previously allocated pipes

	4. For each program we'd like to run, launch a child process which
	will take care of parsing the program string, opening input or output
	files, redirecting input and output, and freeing all memory if execve
	could not be called.

	5. Close all pipes.

	6. Wait for all child processes to finish and save state of last process

	7. Free all heap-allocated memory, which in our case are the pipes and
	the command_list.

	8. Compute the exit status and return.

*/
int	main(int argc, char *argv[])
{
	int				**pipes;
	int				status;
	t_command_list	cl;

	if (px_parse_args(argc, argv, &cl) != 0)
	{
		exit(EXIT_FAILURE);
	}
	if (allocate_pipes(&pipes, cl.size) != 0)
	{
		cleanup_command_list(&cl);
		exit(EXIT_FAILURE);
	}
	if (open_pipes(pipes, cl.size) != 0)
	{
		cleanup_pipes(pipes, cl.size);
		cleanup_command_list(&cl);
		exit(EXIT_FAILURE);
	}
	launch_all_children(&cl, pipes);
	close_pipes(pipes, cl.size);
	status = wait_for_all_children();
	cleanup_pipes(pipes, cl.size);
	cleanup_command_list(&cl);
	return (px_exit_status(status));
}
