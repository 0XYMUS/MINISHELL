/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:24:02 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 17:26:38 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*mapea errno a un error de minishell despues de execve*/
t_ercod	execve_errno_code(void)
{
	if (errno == EACCES)
		return (PERR_PERMISSION_DENIED);
	if (errno == ENOENT)
		return (PERR_NO_SUCH_FILE);
	if (errno == ENOTDIR)
		return (PERR_NOT_A_DIRECTORY);
	if (errno == EISDIR)
		return (PERR_IS_DIRECTORY);
	if (errno == ENOEXEC)
		return (PERR_EXEC_FORMAT);
	return (PERR_NOT_FOUND);
}

/*gestiona errores previos a execve cuando hay ruta resuelta*/
int	return_exec_path_error(t_cmd *pl, t_shell *sh, char *path)
{
	if (access(path, X_OK) == 0)
		return (0);
	if (errno == ENOENT)
		return (free(path), error_emit_subject(&sh->err,
				PERR_NO_SUCH_FILE, PNEAR_WORD, pl->argv[0]));
	if (errno == EACCES)
		return (free(path), error_emit_subject(&sh->err,
				PERR_PERMISSION_DENIED, PNEAR_WORD, pl->argv[0]));
	if (errno == ENOTDIR)
		return (free(path), error_emit_subject(&sh->err,
				PERR_NOT_A_DIRECTORY, PNEAR_WORD, pl->argv[0]));
	return (free(path), error_emit_subject(&sh->err, PERR_NOT_FOUND,
			PNEAR_WORD, pl->argv[0]));
}
