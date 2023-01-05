/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:10:23 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/05 16:35:52 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*alloc_word(char *str)
{
	char	*word;
	int		len;

	str = trim(str);
	len = word_len(str);
	if (len)
	{
		word = malloc((len + 1) * sizeof(*word));
		return (word);
	}
	return (0);
}

static void	fill_word(char *str, char *word)
{
	int		i;
	char	*c;

	str = trim(str);
	i = 0;
	while (1)
	{
		c = iter(&str);
		if (!c)
		{
			word[i] = 0;
			break ;
		}
		else
		{
			word[i] = *c;
		}
		i++;
	}
}

static char	*get_word(char *str)
{
	char	*word;

	word = alloc_word(str);
	if (!word)
		return (0);
	fill_word(str, word);
	return (word);
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
	{
		i++;
	}
	return (i);
}

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
