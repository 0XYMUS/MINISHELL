/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:27:52 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/14 15:57:37 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_directory_path(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

int	apply_redirs(t_redir *redirs)
{
	int fd;

	while (redirs)
	{
		if (redirs->type == R_IN)
		{
			fd = open(redirs->target, O_RDONLY);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), -1);
			close(fd);
		}
		else if (redirs->type == R_OUT)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), -1);
			close(fd);
		}
		else if (redirs->type == R_APPEND)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), -1);
			close(fd);
		}
		redirs = redirs->next;
	} // falta heredoc
	return (0);
}

int	exec_choice(t_command *pl, t_shell *sh)
{
	if (!pl || !pl->argv || !pl->argv[0])
		return (0);
	if (pl->type == CMD_BUILTIN)
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
	}
	return (execute_external(pl, sh));
}

int	execute_external(t_command *pl, t_shell *sh)
{
	char	*path;

	/* Si contiene '/', el usuario ya dio una ruta (absoluta o relativa). */
	if (ft_strchr(pl->argv[0], '/'))
		path = ft_strdup(pl->argv[0]);
	else
		/* Si no contiene '/', se busca en PATH. */
		path = find_exec_path(sh->envp, pl->argv[0]);
	if (!path)
		/* No existe en PATH (o no existe la ruta dada). */
		return (error_emit_subject(&sh->err, PERR_NOT_FOUND,
			PNEAR_WORD, pl->argv[0]));
	/* Existe la ruta, pero es un directorio: no es ejecutable. */
	if (is_directory_path(path))
		return (free(path), error_emit_subject(&sh->err, PERR_IS_DIRECTORY,
			PNEAR_WORD, pl->argv[0]));
	/* Existe pero no tiene permiso de ejecución. */
	if (access(path, X_OK) != 0)
		return (free(path), error_emit_subject(&sh->err, PERR_PERMISSION_DENIED,
			PNEAR_WORD, pl->argv[0]));
	/* Si execve vuelve, siempre hubo error. */
	execve(path, pl->argv, sh->envp);
	free(path);
	if (errno == EACCES)
		/* Error de permisos al ejecutar. */
		return (error_emit_subject(&sh->err, PERR_PERMISSION_DENIED,
			PNEAR_WORD, pl->argv[0]));
	if (errno == EISDIR)
		/* El destino era directorio. */
		return (error_emit_subject(&sh->err, PERR_IS_DIRECTORY,
			PNEAR_WORD, pl->argv[0]));
	if (errno == ENOEXEC)
		/* El archivo existe, pero no tiene formato ejecutable válido. */
		return (error_emit_subject(&sh->err, PERR_EXEC_FORMAT,
			PNEAR_WORD, pl->argv[0]));
	/* Resto de errores de ruta/resolución se tratan como no encontrado. */
	return (error_emit_subject(&sh->err, PERR_NOT_FOUND,
		PNEAR_WORD, pl->argv[0]));
}

void child_process(int prev_read, t_command *pl, int *pipefd, t_shell *sh)
{
	if (prev_read != -1)				// checkeo de si hay comando anterior
		dup2(prev_read, STDIN_FILENO);	// para que coja su salida como entarda del nuevo
	if (pl->next)						// si hay siguiente comando redirigimos la salida de este
		dup2(pipefd[1], STDOUT_FILENO); // al siguiente comando
	if (prev_read != -1)
		close(prev_read);
	if (pl->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (apply_redirs(pl->redirs) == -1)
		exit(1);
	exit(exec_choice(pl, sh));
}