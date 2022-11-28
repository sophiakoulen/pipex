/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_utils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:28:49 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/28 17:11:13 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	Sets the heap-allocated components of the program to 0,
	to handle the case cleanup_command is called before allocation.
*/
static void	init_command(t_command *program)
{
	program->exec_path = 0;
	program->args = 0;
	program->infile = 0;
	program->outfile = 0;
}

/*
	no comment
*/
void	launch_all_children(t_command_list *cl, int **pipes)
{
	int	i;

	i = 0;
	while (i < cl->size)
	{
		launch_child(cl, pipes, i);
		i++;
	}
}

/*
	waits for all children and returns status of last process to finish
*/
int	wait_for_all_children(void)
{
	int	status;

	while (wait(&status) > 0)
		;
	return (status);
}

/*
	counts the arguments,
	if not enough return error code.

	allocate memory in command_list
	then write arguments into command_list.
*/
t_px_error	px_parse_args(int argc, char **argv, t_command_list *cl)
{
	int			i;

	if (argc < 5)
	{
		px_print_error(argv[0], px_set_error(PX_WRONG_USAGE));
		return (px_set_error(PX_WRONG_USAGE));
	}
	cl->size = argc - 3;
	cl->arr = malloc(sizeof(*(cl->arr)) * cl->size);
	if (!cl->arr)
	{
		px_print_error("malloc error", px_set_error(PX_SEE_ERRNO));
		return (px_set_error(PX_SEE_ERRNO));
	}
	i = 0;
	while (i < cl->size)
	{
		init_command(cl->arr + i);
		cl->arr[i].raw_string = argv[i + 2];
		i++;
	}
	cl->arr[0].infile = argv[1];
	cl->arr[cl->size - 1].outfile = argv[argc - 1];
	return (px_set_error(PX_SUCCESS));
}

/*
	computes the appropriate exit status,
	using the exit status of the last child
*/
int	px_exit_status(int status)
{
	int	exit_code;
	int	term_signal;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		return (exit_code);
	}
	else
	{
		term_signal = WTERMSIG(status);
		return (128 + term_signal);
	}
}
