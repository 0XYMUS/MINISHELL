/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/14 13:00:13 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_status(const t_error *err)
{
	if (!err || err->code == PERR_NONE)
		return (0);
	if (err->code == PERR_NOT_FOUND)
		return (127);
	if (err->code == PERR_PERMISSION_DENIED)
		return (126);
	if (err->code == PERR_IS_DIRECTORY)
		return (126);
	if (err->code == PERR_EXEC_FORMAT)
		return (126);
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
	err->subject = NULL;
}

void	error_set(t_error *err, t_errcode code,
			t_near near)
{
	if (!err)
		return ;
	err->code = code;
	err->near = near;
	err->subject = NULL;
}

void	error_set_subject(t_error *err, t_errcode code,
			t_near near, const char *subject)
{
	if (!err)
		return ;
	err->code = code;
	err->near = near;
	err->subject = subject;
}

int	error_emit(t_error *err, t_errcode code, t_near near)
{
	error_set(err, code, near);
	error_print(err);
	return (error_status(err));
}

int	error_emit_subject(t_error *err, t_errcode code, t_near near,
		const char *subject)
{
	error_set_subject(err, code, near, subject);
	error_print(err);
	return (error_status(err));
}

int	error_fail(t_error *err, t_errcode code, t_near near)
{
	error_set(err, code, near);
	return (-1);
}

int	error_fail_subject(t_error *err, t_errcode code, t_near near,
		const char *subject)
{
	error_set_subject(err, code, near, subject);
	return (-1);
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
	const char	*subject;

	if (!err || err->code == PERR_NONE)
		return ;
	subject = err->subject;
	if (err->code == PERR_NOT_FOUND)
	{
		if (subject)
			fprintf(stderr, "minishell: %s: command not found\n", subject);
		else
			fprintf(stderr, "minishell: command not found\n");
		return ;
	}
	if (err->code == PERR_PERMISSION_DENIED)
	{
		if (subject)
			fprintf(stderr, "minishell: %s: Permission denied\n", subject);
		else
			fprintf(stderr, "minishell: Permission denied\n");
		return ;
	}
	if (err->code == PERR_IS_DIRECTORY)
	{
		if (subject)
			fprintf(stderr, "minishell: %s: Is a directory\n", subject);
		else
			fprintf(stderr, "minishell: Is a directory\n");
		return ;
	}
	if (err->code == PERR_EXEC_FORMAT)
	{
		if (subject)
			fprintf(stderr, "minishell: %s: Exec format error\n", subject);
		else
			fprintf(stderr, "minishell: Exec format error\n");
		return ;
	}
	if (err->code == PERR_OOM)
	{
		perror("minishell");
		return ;
	}
	s = near_to_str(err->near);
	fprintf(stderr,
		"minishell: syntax error near unexpected token `%s'\n", s);
}
