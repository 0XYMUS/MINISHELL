/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:15:32 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "minishell.h"

/*maneja error ENOMEM*/
int	ft_fail(t_cmd **node, t_error *err)
{
	errno = ENOMEM;
	error_fail(err, PERR_OOM, PNEAR_NONE);
	if (node && *node)
		pipeline_free_all(node);
	return (-1);
}

int	error_status(const t_error *err)
{
	if (!err || err->code == PERR_NONE)
		return (0);
	if (err->code == PERR_NOT_FOUND)
		return (127);
	if (err->code == PERR_NO_SUCH_FILE)
		return (127);
	if (err->code == PERR_PERMISSION_DENIED)
		return (126);
	if (err->code == PERR_NOT_A_DIRECTORY)
		return (126);
	if (err->code == PERR_FILENAME_REQUIRED)
		return (2);
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

int	error_emit_subject(t_error *err, t_ercod code, t_near near,
		const char *subject)
{
	if (err)
	{
		err->code = code;
		err->near = near;
		err->subject = subject;
	}
	error_print(err);
	return (error_status(err));
}

int	error_fail(t_error *err, t_ercod code, t_near near)
{
	if (err)
	{
		err->code = code;
		err->near = near;
		err->subject = NULL;
	}
	return (-1);
}
