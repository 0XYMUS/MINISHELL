/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:27:52 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/03/31 13:12:36 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void apply_redirs(t_redir *redirs)
{
	int fd;

	while (redirs)
	{
		if (redirs->type == R_IN) // ENTRADA: < (2 params: archivo existe)
		{
			fd = open(redirs->target, O_RDONLY); // Solo lectura
			dup2(fd, STDIN_FILENO);				 // Redirige entrada
			close(fd);
		}
		else if (redirs->type == R_OUT) // SALIDA: > (3 params: O_TRUNC vacía)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 0644 = rw-r--r--
			dup2(fd, STDOUT_FILENO);									   // Redirige salida
			close(fd);
		}
		else if (redirs->type == R_APPEND) // APPEND: >> (3 params: O_APPEND añade)
		{
			fd = open(redirs->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
			dup2(fd, STDOUT_FILENO); // Redirige salida
			close(fd);
		}
		redirs = redirs->next; // falta heredoc pero debe ir en el parseo
	}
}

int	child_builtins(t_command *pl, t_shell *sh)
{
	if (pl->builtin == BI_ECHO)
		return (xy_echo(pl, sh));
	if (pl->builtin == BI_PWD)
		return (xy_pwd(pl, sh));
	if (pl->builtin == BI_ENV)
		return (xy_env(pl, sh));
	return (0);
}

int	path_exec(t_command *pl, t_shell *sh, char *cmd)
{
	if (access(cmd, F_OK) != 0)
		return (error_emit(&sh->err, PERR_NOT_FOUND, PNEAR_NONE));
	if (access(cmd, X_OK) != 0)
		return (error_emit(&sh->err, PERR_PERMISSION_DENIED, PNEAR_NONE));
	if (execve(cmd, pl->argv, sh->envp) == -1)
	{
		if (errno == EACCES)
			return (error_emit(&sh->err, PERR_PERMISSION_DENIED, PNEAR_NONE));
		if (errno == ENOENT)
			return (error_emit(&sh->err, PERR_NOT_FOUND, PNEAR_NONE));
		return (error_emit(&sh->err, PERR_OOM, PNEAR_NONE));
	}
	return (0);
	}

int exec_choice(t_command *pl, t_shell *sh)
{
	char	*cmd;

	if (!pl->argv[0])
		return (0);
	cmd = pl->argv[0];
	if (pl->type == CMD_BUILTIN) //ejecucion directa de lo builtins childs
		return (child_builtins(pl, sh));
	else if (cmd[0] == '/' || cmd[0] == '.') //caso ruta o ejecutable
		return (path_exec(pl, sh, cmd));
	else //comando seco
	{
		return (error_emit(&sh->err, PERR_NOT_FOUND, PNEAR_NONE));

	}
	return (127);
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
	apply_redirs(pl->redirs);
	exit(exec_choice(pl, sh));
}