/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/16 18:24:18 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c) 
{
	return (c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
			c == '\f');
}

int	is_key(char c)
{
	return (c == '|'  || c == '<' || c == '>');
}

int	word_len(char *line, int i)
{
	int		i_initial;
	char	q;
	
	i_initial = i;
	q = 0;
	while(line[i] && (!is_space(line[i]) || q != 0) && (!is_key(line[i]) || q != 0))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			q = line[i];
			i++;
			while (line[i] && line[i] != q)
				i++;
			if (line[i] && line[i] == q)
			{	
				q = 0;
				i++;
			}
			else
				return (-1);
		}
		else
			i++;
	}
	return (i - i_initial);
}

int	word_len_quote(char *line, int i)
{
	char	q;
	int		out_len;
	
	out_len = 0;
	q = 0;
	while(line[i] && (!is_space(line[i]) || q != 0) && (!is_key(line[i]) || q != 0))
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			q = line[i];
			i++;
			while (line[i] && line[i] != q)
			{
				out_len++;
				i++;
			}
			if (line[i] && line[i] == q)
			{	
				q = 0;
				i++;
			}
			else
				return (-1);
		}
		else
		{
			out_len++;
			i++;
		}
	}
	return (out_len);
}

char	*word_dup(char *line, int i, int wordlen)
{
	int		j;
	char	q;
	char	*word;
	int		end;
	int		out_len;

	end = i + wordlen;
	out_len = word_len_quote(line, i);
	if (out_len < 0)
		return (NULL);
	word = malloc(sizeof(char) * out_len + 1);
	if (!word)
		return (NULL);
	j = 0;
	q = 0;
	while (i < end)
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			q = line[i];
			i++;
			while (i < end && line[i] && line[i] != q)
			{
				word[j] = line[i];
				i++;
				j++;
			}
			if (i < end && line[i] == q)
			{
				q = 0;
				i++;
			}
		}
		else
		{
			word[j] = line[i];
				i++;
				j++;
		}
			
	}
	word[j] = '\0';
	return (word);
}