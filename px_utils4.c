/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   px_utils4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 15:30:45 by skoulen           #+#    #+#             */
/*   Updated: 2022/11/28 17:35:03 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
	Splits the a raw string representing a command with arguments into
	a heap-allocated null-terminated array containing duplicates of the
	strings.
	In case of malloc failure, an error code is returned. 
*/
int	px_split_command(char *str, char ***res)
{
	*res = ft_split2(str, " \t");
	if (!(*res))
	{
		perror("malloc failure");
		return (-1);
	}
	return (0);
}
