/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 16:50:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static const char	*error_msg(t_errcode code)
{
	if (code == PERR_NOT_FOUND)
		return ("command not found");
	if (code == PERR_PERMISSION_DENIED)
		return ("Permission denied");
	if (code == PERR_IS_DIRECTORY)
		return ("Is a directory");
	if (code == PERR_EXEC_FORMAT)
		return ("Exec format error");
	return (NULL);
}

static int	print_known_error(const t_error *err)
{
	const char	*msg;

	msg = error_msg(err->code);
	if (!msg)
		return (0);
	if (err->subject)
		fprintf(stderr, "minishell: %s: %s\n", err->subject, msg);
	else
		fprintf(stderr, "minishell: %s\n", msg);
	return (1);
}

void	error_print(const t_error *err)
{
	const char	*s;

	if (!err || err->code == PERR_NONE)
		return ;
	if (print_known_error(err))
		return ;
	if (err->code == PERR_OOM)
	{
		perror("minishell");
		return ;
	}
	s = near_to_str(err->near);
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", s);
}
