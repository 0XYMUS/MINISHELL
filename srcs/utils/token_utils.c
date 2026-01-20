/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/20 17:51:40 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c) 
{
	return (c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
			c == '\f');
}

static int	is_key(char c)
{
	return (c == '|'  || c == '<' || c == '>' || c == '\'' || c == '"');
}

int	word_len(char *line, int i)
{
	int		i_initial;
	char	q;
	
	i_initial = i;
	q = 0;
	if (line[i] == '\'' || line[i] == '"')
	{
		q = line[i];
		i++;
		while (line[i] && line[i] != q)
			i++;
		if (line[i] == q)
			i++;
		else
			return (-1);
	}
	else
	{
		while (line[i] && !is_space(line[i]) && !is_key(line[i]))
			i++;
	}
	return (i - i_initial);
}

char	*word_dup(char *line, int i, int wordlen)
{
	char	*word;
	int		end;
	int		j;

	end = i + wordlen;
	word = malloc(sizeof(char) * wordlen + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (i < end)
	{
		word[j] = line[i];
		j++;
		i++;
	}
	word[j] = '\0';
	return (word);
}
