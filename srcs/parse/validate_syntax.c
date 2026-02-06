/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/06 12:34:37 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_near	near_from_token(t_token_type token)
{
	if (token == TOK_PIPE)
		return (PNEAR_PIPE);
	if (token == TOK_REDIR_IN)
		return (PNEAR_REDIR_IN);
	if (token == TOK_REDIR_OUT)
		return (PNEAR_REDIR_OUT);
	if (token == TOK_APPEND)
		return (PNEAR_APPEND);
	if (token == TOK_HEREDOC)
		return (PNEAR_HEREDOC);
	return (PNEAR_WORD);
}

int	validate_syntax(t_token *token, t_error *err)
{
	parse_error_init(err);
	if (!token)
		return (0);
	if (token->type == TOK_PIPE)
		return (parse_error_set(err, PERR_PIPE_START, PNEAR_PIPE), -1);
	while (token)
	{
		if (token->type == TOK_PIPE)
		{
			if (!token->next)
				return (parse_error_set(err, 2, 1), -1);
			if (token->next->type == TOK_PIPE)
				return (parse_error_set(err, 3, 2), -1);
		}
		if (is_redir(token->type))
		{
			if (!token->next)
				return (parse_error_set(err, 4, PNEAR_NEWLINE), -1);
			if (token->next->type != TOK_WORD)
				return (parse_error_set(err, 4, near_from_token(token->next->type)), -1);
		}
		token = token->next;
	}
	return (0);
}
