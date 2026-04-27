/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:27:52 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/27 11:41:03 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>

/*cierra los fd del padre y prepara el pipe para el siguiente comando*/
void	parent_process(int *prev_read, t_cmd *pl, int *pipefd)
{
	if (*prev_read != -1)
		close((*prev_read));
	if (pl->next)
	{
		close(pipefd[1]);
		*prev_read = pipefd[0];
	}
	else
		*prev_read = -1;
}

/*abre y conecta una redirección de archivo con stdin o stdout*/
static int	apply_file_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == R_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == R_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	if (redir->type == R_IN && dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	if (redir->type != R_IN && dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

/*usa un heredoc ya preparado o lo genera al vuelo para conectar stdin*/
static int	apply_heredoc_input(t_redir *redir, int stdin_backup, t_shell *sh)
{
	if (redir->heredoc_fd != -1)
	{
		if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
			return (-1);
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
		return (0);
	}
	if (dup2(stdin_backup, STDIN_FILENO) == -1)
		return (-1);
	if (apply_heredoc_redir(redir, sh) == -1)
		return (-1);
	return (0);
}

/*aplica todas las redirecciones asociadas al comando*/
int	apply_redirs(t_redir *redirs, t_shell *sh)
{
	int	stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	if (stdin_backup == -1)
		return (-1);
	while (redirs)
	{
		if (redirs->type == R_IN || redirs->type == R_OUT
			|| redirs->type == R_APPEND)
		{
			if (apply_file_redir(redirs) == -1)
				return (close(stdin_backup), -1);
		}
		else if (redirs->type == R_HEREDOC)
		{
			if (apply_heredoc_input(redirs, stdin_backup, sh) == -1)
				return (close(stdin_backup), -1);
		}
		redirs = redirs->next;
	}
	close(stdin_backup);
	return (0);
}

/*prepara los descriptores del hijo y ejecuta el comando*/
void	child_process(int prev_read, t_cmd *pl, int *pipefd, t_shell *sh)
{
	if (prev_read != -1)
		dup2(prev_read, STDIN_FILENO);
	if (pl->next)
		dup2(pipefd[1], STDOUT_FILENO);
	if (prev_read != -1)
		close(prev_read);
	if (pl->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (apply_redirs(pl->redirs, sh) == -1)
	{
		if (sh->exit_status == 130)
			child_cleanup_and_exit(sh, 130);
		child_cleanup_and_exit(sh, 1);
	}
	child_cleanup_and_exit(sh, exec_choice(pl, sh));
}
