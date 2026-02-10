/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/09 16:32:20 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>

int	is_space(char c) 
{
	return (c == ' '  || c == '\t' || c == '\n' || c == '\r' || c == '\v' ||
			c == '\f');
}

static int	is_key(char c)
{
	return (c == '|'  || c == '<' || c == '>');
}

static int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$' || c == '`' || c == '\n');
}

int	word_len(char *line, int i)
{
	int		i_initial;
	char	q;

	i_initial = i;
	q = 0;
	while (line[i])
	{
		if (!q && (is_space(line[i]) || is_key(line[i])))
			break ;
		if (line[i] == '\\' && q != '\'' && line[i + 1])
		{
			if (q == 0)
				i += 2;
			else if (q == '"' && is_dquote_escapable(line[i + 1]))
				i += 2;
			else
				i++;
			continue ;
		}
		if (line[i] == '\'' || line[i] == '"')
		{
			if (q == 0)
				q = line[i];
			else if (q == line[i])
				q = 0;
			i++;
			continue ;
		}
		i++;
	}
	if (q != 0)
	{
		errno = EINVAL;
		return (-1);
	}
	return (i - i_initial);
}

char	*word_dup(char *line, int i, int wordlen)
{
	char	*word;
	int		end;
	int		j;
	char	q;

	end = i + wordlen;
	word = malloc(sizeof(char) * wordlen + 1);
	if (!word)
		return (NULL);
	j = 0;
	q = 0;
	while (i < end)
	{
		if (line[i] == '\\' && q != '\'' && i + 1 < end)
		{
			if (q == 0)
			{
				word[j++] = line[i + 1];
				i += 2;
				continue ;
			}
			if (q == '"' && is_dquote_escapable(line[i + 1]))
			{
				if (line[i + 1] != '\n')
					word[j++] = line[i + 1];
				i += 2;
				continue ;
			}
			word[j++] = line[i++];
			continue ;
		}
		if (line[i] == '\'' || line[i] == '"')
		{
			if (q == 0)
				q = line[i++];
			else if (q == line[i])
			{
				q = 0;
				i++;
			}
			else
				word[j++] = line[i++];
			continue ;
		}
		word[j++] = line[i++];
	}
	word[j] = '\0';
	return (word);
}
