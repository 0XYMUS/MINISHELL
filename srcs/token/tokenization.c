/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:58:11 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/10 12:09:34 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>

static char	find_unclosed_quote(const char *line, int i)
{
	char	q;

	q = 0;
	while (line[i])
	{
		if (line[i] == '\\' && q != '\'' && line[i + 1])
		{
			i += 2;
			continue ;
		}
		if (line[i] == '\'' || line[i] == '"')
		{
			if (q == 0)
				q = line[i];
			else if (q == line[i])
				q = 0;
		}
		i++;
	}
	return (q);
}

static void	print_unclosed_quote(char q)
{
	if (!q)
		q = '\'';
	fprintf(stderr,
		"minishell: unexpected EOF while looking for matching `%c'\n", q);
	fprintf(stderr,
		"minishell: syntax error: unexpected end of file\n");
}

static int	type_word(char *line, int *i, t_token **lst)
{
	int		wordlen;
	char	*word;
	char	*qmask;
	t_token	*token;
	int		space;

	token = NULL;
	wordlen = word_len(line, *i);
	if (wordlen < 0)
	{
		if (errno == EINVAL)
			print_unclosed_quote(find_unclosed_quote(line, *i));
		return (-1);
	}
	qmask = NULL;
	word = word_dup(line, *i, wordlen, &qmask);
	if (!word)
		return (-1);
	space = 0;
	if (is_space(line[*i + wordlen]))
		space = 1;
	token = token_new(TOK_WORD, word, qmask, space);
	if (!token)
	{
		free(word);
		free(qmask);
		return (-1);
	}
	token_add_back(lst, token);
	*i = *i + wordlen;
	return (0);
}


static int	type_operator(t_token_type type, int *i, t_token **lst, char *line)
{
	t_token	*token;

	if (is_space(line[*i + 1]))
		token = token_new(type, NULL, NULL, 1);
	else
		token = token_new(type, NULL, NULL, 0);
	if (!token)
		return (-1);
	token_add_back(lst, token);
	if (type == TOK_APPEND || type == TOK_HEREDOC)
		(*i) += 2;
	else
		(*i)++;
	return (0);
}

static int	tokenizer2(char *line, t_token	**lst, int *i)
{
	int	error;
	
	error = 0;
	if (line[*i] == '|')
		error = type_operator(TOK_PIPE, i, lst, line);
	else if (line[*i] == '<' && line[*i + 1] == '<')
		error = type_operator(TOK_HEREDOC, i, lst, line);
	else if (line[*i] == '>' && line[*i + 1] == '>')
		error = type_operator(TOK_APPEND, i, lst, line);
	else if (line[*i] == '<')
		error = type_operator(TOK_REDIR_IN, i, lst, line);
	else if (line[*i] == '>')
		error = type_operator(TOK_REDIR_OUT, i, lst, line);
	else if (type_word(line, i, lst) == -1)
		error = -1;
	if (error == -1)
		return (token_free_all(lst), -1);
	return (0);
}

t_token	*tokenizer(char *line)
{
	int		i;
	t_token	*lst;

	lst = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (tokenizer2(line, &lst, &i) == -1)
			return (token_free_all(&lst), NULL);
	}
	return (lst);
}
