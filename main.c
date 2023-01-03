/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:16:59 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 14:04:30 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
