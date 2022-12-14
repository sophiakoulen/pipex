/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 14:41:01 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/08 15:15:25 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	is_end(char *str, t_parse_state s);

/*
	iter: Return the next character of the word, and moves to pointer to
	the next character to be processed.

	Because of quoting and escaping, the next character of the word we
	are generating isn't necessarily the next character of the input string.

	Examples:
		[\'hello] needs to generate the word ['hello]:
		The next character of the word is ['] and we want to move the pointer
		to the string [hello] to continue processing the word.

		[abc] needs to generate the word [abc]:
		The next character of the word is [a] and we want to move the pointer
		to the string [bc] to continue.

		[""] is an empty word:
		0 is returned. ow we know we are at the end of the word.
*/
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

/*
	Moves the pointer to start of the next word to be processed.
	Returns:
		1 if there was a word to skip.
		0 if there were no more words.
*/
int	next_word(char **str)
{
	*str = trim(*str);
	if (!**str)
		return (0);
	while (iter(str))
		;
	*str = trim(*str);
	return (1);
}

static int	is_end(char *str, t_parse_state s)
{
	if (!*str)
		return (1);
	if ((ft_isspace(*str) && !s.squote && !s.dquote && !s.escaped))
		return (1);
	return (0);
}
