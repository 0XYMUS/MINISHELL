/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 00:00:00 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 12:07:02 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*ejecuta el builtin correspondiente dentro del hijo*/
static int	exec_builtin(t_command *pl, t_shell *sh)
{
	if (pl->builtin == BI_ECHO)
		return (xy_echo(pl, sh));
	if (pl->builtin == BI_CD)
		return (xy_cd(pl, sh));
	if (pl->builtin == BI_PWD)
		return (xy_pwd(pl, sh));
	if (pl->builtin == BI_ENV)
		return (xy_env(pl, sh));
	if (pl->builtin == BI_EXPORT)
		return (xy_export(pl, sh));
	if (pl->builtin == BI_UNSET)
		return (xy_unset(pl, sh));
	if (pl->builtin == BI_EXIT)
		return (xy_exit(pl, sh));
	return (0);
}

/*comprueba si la ruta apunta a un directorio*/
static int	is_directory_path(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

/*resuelve la ruta del ejecutable entre acceso directo o PATH*/
static int	resolve_exec_path(t_command *pl, t_shell *sh, char **path)
{
	if (ft_strchr(pl->argv[0], '/'))
		*path = ft_strdup(pl->argv[0]);
	else
		*path = find_exec_path(sh->envp, pl->argv[0]);
	return (*path != NULL);
}

/*ejecuta un comando externo y gestiona los errores previos a execve*/
int	execute_external(t_command *pl, t_shell *sh)
{
	char		*path;
	t_errcode	code;

	if (!resolve_exec_path(pl, sh, &path))
		return (error_emit_subject(&sh->err, PERR_NOT_FOUND,
				PNEAR_WORD, pl->argv[0]));
	if (is_directory_path(path))
		return (free(path), error_emit_subject(&sh->err, PERR_IS_DIRECTORY,
				PNEAR_WORD, pl->argv[0]));
	if (access(path, X_OK) != 0)
		return (free(path), error_emit_subject(&sh->err, PERR_PERMISSION_DENIED,
				PNEAR_WORD, pl->argv[0]));
	execve(path, pl->argv, sh->envp);
	free(path);
	if (errno == EACCES)
		code = PERR_PERMISSION_DENIED;
	else if (errno == EISDIR)
		code = PERR_IS_DIRECTORY;
	else if (errno == ENOEXEC)
		code = PERR_EXEC_FORMAT;
	else
		code = PERR_NOT_FOUND;
	return (error_emit_subject(&sh->err, code, PNEAR_WORD, pl->argv[0]));
}

/*elige entre builtin o ejecución externa*/
int	exec_choice(t_command *pl, t_shell *sh)
{
	if (!pl || !pl->argv || !pl->argv[0])
		return (0);
	if (pl->argv[0][0] == '\0')
		return (0);
	if (pl->type == CMD_BUILTIN)
		return (exec_builtin(pl, sh));
	return (execute_external(pl, sh));
}
