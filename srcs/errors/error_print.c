/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 17:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:15:32 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "minishell.h"

static void	write_err_str(const char *s)
{
	if (!s)
		return ;
	write(STDERR_FILENO, s, ft_strlen(s));
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

static const char	*error_msg(t_ercod code)
{
	if (code == PERR_NOT_FOUND)
		return ("command not found");
	if (code == PERR_PERMISSION_DENIED)
		return ("Permission denied");
	if (code == PERR_NO_SUCH_FILE)
		return ("No such file or directory");
	if (code == PERR_NOT_A_DIRECTORY)
		return ("Not a directory");
	if (code == PERR_FILENAME_REQUIRED)
		return ("filename argument required");
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
	{
		write_err_str("minishell: ");
		write_err_str(err->subject);
		write_err_str(": ");
		write_err_str(msg);
		write_err_str("\n");
	}
	else
	{
		write_err_str("minishell: ");
		write_err_str(msg);
		write_err_str("\n");
	}
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
	write_err_str("minishell: syntax error near unexpected token `");
	write_err_str(s);
	write_err_str("'\n");
}
