/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:58:11 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 13:38:09 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*anade a la lista los token operadores*/
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

/*discrimina si es word o operador y llama a su correspondiente*/
static int	handle_token(char *line, int *i, t_token **lst)
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
	else if (token_add_word(line, i, lst) == -1)
		error = -1;
	return (error);
}

/*funcion principal de la tokenizacion, recorre el promt y construye la lista*/
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
		if (handle_token(line, &i, &lst) == -1)
			return (token_free_all(&lst), NULL);
		token_mark_delimiter(lst);
	}
	return (lst);
}
