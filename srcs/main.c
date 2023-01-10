/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:16:59 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/10 14:07:58 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	1. Initialize the pipex structure that'll contain the variable we'll
	need: file descriptors, program names and arguments, program paths,
	program statuses, program process id's.
	
	2. Execute the pipeline.

	3. Cleanup all heap-allocated memory

	4. Return the exit status of the last command. 
*/
int	main(int argc, char *argv[])
{
	t_pipex	p;
	int		ret;

	if (init(argc, argv, &p) == -1)
		return (1);
	ret = exec_pipeline(&p);
	cleanup_pipex(&p);
	return (ret);
}
