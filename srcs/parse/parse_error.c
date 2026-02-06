/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/06 12:51:15 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_status(const t_error *err)
{
	if (!err || err->code == PERR_NONE)
		return (0);
	if (err->code == PERR_OOM)
		return (1);
	return (2);
}

void	error_init(t_error *err)
{
	if (!err)
		return ;
	err->code = PERR_NONE;
	err->near = PNEAR_NONE;
}

void	error_set(t_error *err, t_errcode code,
			t_near near)
{
	if (!err)
		return ;
	err->code = code;
	err->near = near;
}

static const char	*near_to_str(t_near near)
{
	if (near == PNEAR_NEWLINE)
		return ("newline");
	if (near == PNEAR_PIPE)
		return ("|");
	if (near == PNEAR_REDIR_IN)
		return ("<");
	if (near == PNEAR_REDIR_OUT)
		return (">");
	if (near == PNEAR_APPEND)
		return (">>");
	if (near == PNEAR_HEREDOC)
		return ("<<");
	if (near == PNEAR_WORD)
		return ("word");
	return ("?");
}

void	error_print(const t_error *err)
{
	const char	*s;

	if (!err || err->code == PERR_NONE)
		return ;
	if (err->code == PERR_OOM)
	{
		perror("minishell");
		return ;
	}
	s = near_to_str(err->near);
	fprintf(stderr,
		"minishell: syntax error near unexpected token `%s'\n", s);
}
