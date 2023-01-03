/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 17:10:23 by skoulen           #+#    #+#             */
/*   Updated: 2023/01/03 18:06:12 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*trim(char *str)
{
	while (ft_isspace(*str))
		str++;
	return (str);
}

static int	word_len(char *str)
{
	int	i;
	int	squote;
	int	dquote;

	squote = 0;
	dquote = 0;
	str = trim(str);
	i = 0;
	while (str[i])
	{
		if (!squote && !dquote)
		{
			if (ft_isspace(str[i]))
				break ;
		}
		if (str[i] == '\'')
			squote = !squote;
		else if (str[i] == '"')
			dquote = !dquote;
		i++;
	}
	return (i);
}

static char	*get_word(char *str)
{
	int		len;
	char	*word;

	str = trim(str);
	len = word_len(str);
	if (len == 0)
	{
		return (0);
	}
	else
	{
		word = ft_substr(str, 0, len);
		return (word);
	}
}

static int	next_word(char **str)
{
	int	len;

	*str = trim(*str);
	len = word_len(*str);
	if (!len)
		return (0);
	*str += len;
	return (1);
}

static int	word_count(char *str)
{
	char	*cpy;
	int		i;

	cpy = str;
	i = 0;
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

	printf("str: %s\n", str);
	wc = word_count(str);
	words = ft_calloc(wc + 1, sizeof(*words));
	if (!words)
		return (0);
	i = 0;
	while (i < wc)
	{
		words[i] = get_word(str);
		printf("%s\n", words[i]);
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
