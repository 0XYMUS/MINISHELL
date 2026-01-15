/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:58:11 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/15 12:55:47 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	type_word_helper(char *line, t_token **lst, int i, int wordlen)
{
	char	*word;
	t_token	*token;

	token = NULL;
	word = word_dup(line, i, wordlen);
	if (!word)
		return (-1);
	token = token_new(TOK_WORD, word);
	if (!token)
		return (free(word), -1);
	token_add_back(lst, token);
	return (0);
}

static int	type_word(char *line, int *i, t_token **lst)
{
	int		wordlen;
	char	q;

	q = 0;
	if (line[*i] == '\'' || line[*i] == '"')
	{
		q = line[*i];
		(*i)++;
		wordlen = str_len_quote(line, (*i));
		if (wordlen == -1 || line[*i + wordlen] != q)
			return (-1);
	}
	else
		wordlen = str_len_space(line, *i);
	if (type_word_helper(line, lst, *i, wordlen) == -1)
		return (-1);
	*i = *i + wordlen;
	if (q != 0)
		(*i)++;
	return (0);
}

static int	type_operator(t_token_type type, int *i, t_token **lst)
{
	t_token	*token;

	token = token_new(type, NULL);
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
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		if (type_operator(TOK_HEREDOC, i, lst) == -1)
			return (token_free_all(lst), -1);
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		if (type_operator(TOK_APPEND, i, lst) == -1)
			return (token_free_all(lst), -1);
	}
	else if (line[*i] == '<')
	{
		if (type_operator(TOK_REDIR_IN, i, lst) == -1)
			return (token_free_all(lst), -1);
	}
	else if (line[*i] == '>')
	{
		if (type_operator(TOK_REDIR_OUT, i, lst) == -1)
			return (token_free_all(lst), -1);
	}
	else if (type_word(line, i, lst) == -1)
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
		while (is_space(line[i]) && line[i])
			i++;
		if (!line[i])
			break ;
		if (line[i] == '|')
		{
			if (type_operator(TOK_PIPE, &i, &lst) == -1)
				return (token_free_all(&lst), NULL);
		}
		else
		{
			if (tokenizer2(line, &lst, &i) == -1)
				return (token_free_all(&lst), NULL);
		}
	}
	return (lst);
}
