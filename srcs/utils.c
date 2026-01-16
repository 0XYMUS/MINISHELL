/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/16 11:14:55 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_space(char c) 
{
	return (c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
			c == '\f');
}

int	is_key(char c)
{
	return (c == '|'  || c == '<' || c == '>');
}

int	str_len_quote(char *str, int i, char q)
{
	int		i_initial;

	i_initial = i;
	if (q == '\'')
	{
		while (str[i])
		{
			if (str[i] == q && str[i + 1] && is_space(str[i + 1]))
				break ;
			i++;
		}
		return (i - i_initial);
	}
	else
	{
		while (str[i] && str[i] != '"' && !is_space(str[i + 1]) )
		{
			if (str[i] == q && str[i + 1] && is_space(str[i + 1]))
				break ;
			i++;
		}
		return (i - i_initial);
	}
	return (-1);
}

int	str_len_space(char *str, int i)
{
	int	i_initial;
	
	i_initial = i;
	while(!is_space(str[i]) && !is_key(str[i]) && str[i])
		i++;
	return (i -i_initial);
}

char	*word_dup(char *line, int i, int wordlen, char q)
{
	int	j;
	char *word;

	word = malloc(sizeof(char) * wordlen + 1);
	if (!word)
		return (NULL);
	j = 0;
	while (wordlen > 0)
	{
		if (line[i] == q)
			i++;
		word[j] = line[i];
		i++;
		j++;
		wordlen--;
	}
	word[j] = '\0';
	return (word);
}