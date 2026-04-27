/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 16:20:03 by julepere          #+#    #+#             */
/*   Updated: 2026/04/27 11:41:03 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*restaura stdin/stdout originales tras leer el heredoc*/
void	restore_heredoc_terminal(int saved_in, int saved_out,
	int terminal_fd)
{
	if (terminal_fd == -1)
		return ;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	close(terminal_fd);
}

/*redirecciona stdin y stdout y usa tty para leer el heredoc visible*/
int	setup_heredoc_terminal(int *saved_in, int *saved_out,
	int *terminal_fd)
{
	*terminal_fd = open("/dev/tty", O_RDWR);
	if (*terminal_fd == -1)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in == -1 || *saved_out == -1)
	{
		close(*terminal_fd);
		return (-1);
	}
	if (dup2(*terminal_fd, STDIN_FILENO) == -1
		|| dup2(*terminal_fd, STDOUT_FILENO) == -1)
	{
		close(*terminal_fd);
		close(*saved_in);
		close(*saved_out);
		return (-1);
	}
	return (1);
}

int	wait_heredoc_child(pid_t pid, t_shell *sh, int pfd0, int *tty)
{
	int	status;

	catch_signal_wait_parent();
	waitpid(pid, &status, 0);
	restore_heredoc_terminal(tty[1], tty[2], tty[0]);
	catch_signal_father();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (sh->exit_status = 130, g_signal = 0, close(pfd0), -1);
	g_signal = 0;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (sh->exit_status = 130, close(pfd0), -1);
	if (dup2(pfd0, STDIN_FILENO) == -1)
		return (close(pfd0), -1);
	return (close(pfd0), 0);
}

/*genera un heredoc completo y guarda su lectura para reutilizarla luego*/
static int	prepare_single_heredoc(t_redir *redir, t_shell *sh)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (-1);
	if (redir->heredoc_fd != -1)
	{
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	if (apply_heredoc_redir(redir, sh) == -1)
		return (close(saved_stdin), -1);
	redir->heredoc_fd = dup(STDIN_FILENO);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (close(saved_stdin), close(redir->heredoc_fd), -1);
	close(saved_stdin);
	if (redir->heredoc_fd == -1)
		return (-1);
	return (0);
}

/*prepara todos los heredoc en orden antes de crear hijos de la pipeline*/
int	prepare_heredocs(t_cmd *pl, t_shell *sh)
{
	t_redir	*redir;

	while (pl)
	{
		redir = pl->redirs;
		while (redir)
		{
			if (redir->type == R_HEREDOC)
			{
				if (prepare_single_heredoc(redir, sh) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		pl = pl->next;
	}
	return (0);
}
