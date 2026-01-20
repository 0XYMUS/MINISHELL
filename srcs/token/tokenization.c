/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:58:11 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/20 16:36:57 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	type_word(char *line, int *i, t_token **lst)
{
	int		wordlen;
	char	*word;
	t_token	*token;
	int		space;

	token = NULL;
	wordlen = word_len(line, *i);
	if (wordlen < 0)
		return (-1);
	word = word_dup(line, *i, wordlen);
	if (!word)
		return (-1);
	if (is_space(line[*i + wordlen + 1]))
		space = 1;
	else
		space = 0;
	return (0);
	token = token_new(TOK_WORD, word, 1);
	if (!token)
		return (free(word), -1);
	token_add_back(lst, token);
	*i = *i + wordlen;
	return (0);
}


static int	type_operator(t_token_type type, int *i, t_token **lst, char *line)
{
	t_token	*token;

	if (is_space(line[*i + 1]))
		token = token_new(type, NULL, 1);
	else
		token = token_new(type, NULL, 0);
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
