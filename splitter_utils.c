/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 14:41:01 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/04 14:45:06 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_end(char *str, t_parse_state s)
{
	if (!*str)
		return (1);
	if ((ft_isspace(*str) && !s.squote && !s.dquote && !s.escaped))
		return (1);
	return (0);
}

char	*iter(char **ptr)
{
	static t_parse_state	s = {0, 0, 0};
	char					*str;

	str = *ptr;
	while (!is_end(str, s))
	{
		if (*str == '\'' && !s.dquote && !s.escaped)
			s.squote = !s.squote;
		else if (*str == '"' && !s.squote && !s.escaped)
			s.dquote = !s.dquote;
		else if (*str == '\\' && !s.escaped)
			s.escaped = 1;
		else
		{
			s.escaped = 0;
			(*ptr)++;
			return (str);
		}
		(*ptr)++;
		str = *ptr;
	}
	s = (t_parse_state){0, 0, 0};
	return (0);
}

char	*trim(char *str)
{
	while (ft_isspace(*str))
		str++;
	return (str);
}

int	word_len(char *str)
{
	int	i;

	str = trim(str);
	i = 0;
	while (iter(&str))
	{
		i++;
	}
	return (i);
}

int	next_word(char **str)
{
	*str = trim(*str);
	while (iter(str))
		;
	return (!!**str);
}
