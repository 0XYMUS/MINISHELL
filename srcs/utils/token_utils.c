/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/13 17:26:26 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>

static int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$' || c == '`' || c == '\n');
}

static int	is_key(char c)
{
	return (c == '|'  || c == '<' || c == '>');
}

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static char	qmask_from_quote(char q)
{
	if (q == '\'')
		return ('1');
	if (q == '"')
		return ('2');
	return ('0');
}

static int	append_plain(char *word, char *qmask, int *j, char c)
{
	word[*j] = c;
	qmask[*j] = '0';
	(*j)++;
	return (0);
}

static void	append_quoted(char *word, char *qmask, int *j, char q, char c)
{
	word[*j] = c;
	qmask[*j] = qmask_from_quote(q);
	(*j)++;
}

static int	handle_backslash(char *line, int *i, int end, char q,
				char *word, char *qmask, int *j)
{
	if (line[*i] != '\\' || q == '\'' || *i + 1 >= end)
		return (0);
	if (q == 0)
	{
		append_plain(word, qmask, j, line[*i + 1]);
		*i += 2;
		return (1);
	}
	if (q == '"' && is_dquote_escapable(line[*i + 1]))
	{
		if (line[*i + 1] != '\n')
			append_quoted(word, qmask, j, q, line[*i + 1]);
		*i += 2;
		return (1);
	}
	append_quoted(word, qmask, j, q, line[*i]);
	(*i)++;
	return (1);
}

static int	handle_quote(char *line, int *i, char *q)
{
	if (line[*i] != '\'' && line[*i] != '"')
		return (0);
	if (*q == 0)
		*q = line[(*i)++];
	else if (*q == line[*i])
	{
		*q = 0;
		(*i)++;
	}
	else
		return (0);
	return (1);
}

static int	word_len_step(char *line, int *i, char *q)
{
	if (line[*i] == '\\' && *q != '\'' && line[*i + 1])
	{
		if (*q == 0)
			*i += 2;
		else if (*q == '"' && is_dquote_escapable(line[*i + 1]))
			*i += 2;
		else
			(*i)++;
		return (1);
	}
	if (is_quote(line[*i]))
	{
		if (*q == 0)
			*q = line[*i];
		else if (*q == line[*i])
			*q = 0;
		(*i)++;
		return (1);
	}
	return (0);
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
		if (word_len_step(line, &i, &q))
			continue ;
		i++;
	}
	if (q != 0)
	{
		errno = EINVAL;
		return (-1);
	}
	return (i - i_initial);
}

static int	alloc_word_qmask(int wordlen, char **word, char **qmask)
{
	*word = malloc(sizeof(char) * wordlen + 1);
	if (!*word)
		return (0);
	*qmask = malloc(sizeof(char) * wordlen + 1);
	if (!*qmask)
	{
		free(*word);
		return (0);
	}
	return (1);
}

static void	word_dup_finish(char *word, char *qmask, int j,
				char **qmask_out)
{
	word[j] = '\0';
	qmask[j] = '\0';
	if (qmask_out)
		*qmask_out = qmask;
	else
		free(qmask);
}

char	*word_dup(char *line, int i, int wordlen, char **qmask_out)
{
	char	*word;
	char	*qmask;
	int		end;
	int		j;
	char	q;

	end = i + wordlen;
	if (!alloc_word_qmask(wordlen, &word, &qmask))
		return (NULL);
	j = 0;
	q = 0;
	while (i < end)
	{
		if (handle_backslash(line, &i, end, q, word, qmask, &j))
			continue ;
		if (handle_quote(line, &i, &q))
			continue ;
		append_quoted(word, qmask, &j, q, line[i]);
		i++;
	}
	word_dup_finish(word, qmask, j, qmask_out);
	return (word);
}
