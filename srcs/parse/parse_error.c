/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/03 16:45:47 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_error_status(const t_parse_error *err)
{
	if (!err || err->code == PERR_NONE)
		return (0);
	if (err->code == PERR_OOM)
		return (1);
	return (2);
}

void	parse_error_init(t_parse_error *err)
{
	if (!err)
		return ;
	err->code = PERR_NONE;
	err->near = PNEAR_NONE;
}

void	parse_error_set(t_parse_error *err, t_parse_errcode code,
			t_parse_near near)
{
	if (!err)
		return ;
	err->code = code;
	err->near = near;
}

static const char	*near_to_str(t_parse_near near)
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

void	parse_error_print(const t_parse_error *err)
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
