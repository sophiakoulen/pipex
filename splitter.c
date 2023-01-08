/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:10:23 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/08 13:39:13 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*get_word(char *str);
static int	word_count(char *str);

/*
	Splits the string into multiple words, taking into account whitespace
	and quotes and backslashes for escaping characters.

	Return value is an array of strings ending with NULL allocated with
	malloc, or NULL of an error occured.

	Examples:
		"hello world" => {"hello", "world", 0}
		"	a 	a a a" => {"a", "a", "a", "a", 0}
		"this\ is\ a\ test." => {"this is a test", 0}
		""forty two"" => {"forty two", 0}
		"'always writing' new' 'bugs" => {"always writing", "new bugs", 0}

*/
char	**split_cmd(char *str)
{
	char	**words;
	int		wc;
	int		i;

	wc = word_count(str);
	words = ft_calloc(wc + 1, sizeof(*words));
	if (!words)
		return (0);
	i = 0;
	while (i < wc)
	{
		words[i] = get_word(str);
		if (!words[i])
		{
			cleanup_strs(words);
			return (0);
		}
		next_word(&str);
		i++;
	}
	return (words);
}

static char	*get_word(char *str)
{
	int		i;
	char	*c;
	int		len;
	char	*word;

	str = trim(str); //do i need trim?
	len = word_len(str);
	if (len)
	{
		word = malloc((len + 1) * sizeof(*word));
		if (word)
		{
			i = 0;
			while (i < len)
			{
				c = iter(&str);
				word[i] = *c;
				i++;
			}
			word[i] = 0;
			return (word);
		}
	}
	return (0);
}

static int	word_count(char *str)
{
	char	*cpy;
	int		i;

	cpy = trim(str);
	if (!*cpy)
		return (0);
	i = 1;
	while (next_word(&cpy))
		i++;
	return (i);
}
