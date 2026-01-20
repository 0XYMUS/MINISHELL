/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/20 13:56:19 by jojeda-p         ###   ########.fr       */
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
	if (line[i] != '\'' || line[i] != '"')
		q = line[i];
	while (line[i] && !is_space(line[i]) )
	
	return (i - i_initial);
}

int	word_len_quote_helper(char *line, int out_len, int *i, char *q)
{
	if (line[*i] == '\'' || line[*i] == '"')
	{
		(*q) = line[*i];
		(*i)++;
		while (line[*i] && line[*i] != *q)
		{
			out_len++;
			(*i)++;
		}
		if (line[*i] && line[*i] == *q)
		{	
			(*q) = 0;
			(*i)++;
		}
		else
			return (-1);
	}
	else
	{
		out_len++;
		(*i)++;
	}
	return (out_len);
}

int	word_len_quote(char *line, int i)
{
	char	q;
	int		out_len;
	
	out_len = 0;
	q = 0;
	while(line[i] && (!is_space(line[i]) || q != 0) && (!is_key(line[i]) || q != 0))
		out_len = word_len_quote_helper(line, out_len, &i, &q);
	if (out_len == -1)
		return (-1);
	return (out_len);
}

char	*word_dup_helper2(char *word, char *line, int *i, int *j)
{
	word[*j] = line[*i];
	(*i)++;
	(*j)++;
	return (word);
}

char	*word_dup_helper(char *line, char *word, int i, int end)
{
	char	q;
	int		j;
	
	q = 0;
	j = 0;
	while (i < end)
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			q = line[i];
			i++;
			while (i < end && line[i] && line[i] != q)
				word = word_dup_helper2(word, line, &i, &j);
			if (i < end && line[i] == q)
			{
				q = 0;
				i++;
			}
		}
		else
			word = word_dup_helper2(word, line, &i, &j);
	}
	word[j] = '\0';
	return (word);
}

char	*word_dup(char *line, int i, int wordlen)
{
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
	word = word_dup_helper(line, word, i, end);
	return (word);
}
