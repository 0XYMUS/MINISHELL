/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/16 16:36:14 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_near	near_from_token(t_token_type token)
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
	error_init(err);
	if (!token)
		return (0);
	if (token->type == TOK_PIPE)
		return (error_fail(err, PERR_PIPE_START, PNEAR_PIPE));
	while (token)
	{
		if (token->type == TOK_PIPE)
		{
			if (!token->next)
				return (error_fail(err, PERR_PIPE_END, PNEAR_NEWLINE));
			if (token->next->type == TOK_PIPE)
				return (error_fail(err, PERR_PIPE_DOUBLE, PNEAR_PIPE));
		}
		if (is_redir(token->type))
		{
			if (!token->next)
				return (error_fail(err, PERR_REDIR_NO_TARGET, PNEAR_NEWLINE));
			if (token->type == TOK_HEREDOC && token->next->type != TOK_DELIMITER)
				return (error_fail(err, PERR_REDIR_NO_TARGET,
						near_from_token(token->next->type)));
			if (token->type != TOK_HEREDOC && token->next->type != TOK_WORD)
				return (error_fail(err, PERR_REDIR_NO_TARGET,
						near_from_token(token->next->type)));
		}
		token = token->next;
	}
	return (0);
}
