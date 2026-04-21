/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:24:47 by julepere          #+#    #+#             */
/*   Updated: 2026/04/21 18:11:29 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*espera a los hijos y devueve el utlimo codgio de salida valido*/
int	wait_all_children(void)
{
	int	status;
	int	exit_code;
	int	printed_signal;

	exit_code = 0;
	printed_signal = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if ((WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
				&& !printed_signal)
			{
				if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit (core dumped)", 18);
				write(STDOUT_FILENO, "\n", 1);
				printed_signal = 1;
			}
			exit_code = 128 + WTERMSIG(status);
		}
	}
	return (exit_code);
}

/*ejecut los builtins en el proceso padre y redirecciona entradas y salidas*/
static int	run_single_parent_builtin(t_cmd *pl, t_shell *sh)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
		return (perror("minishell"), 1);
	if (apply_redirs(pl->redirs, sh) == -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		if (sh->exit_status == 130)
			return (130);
		return (1);
	}
	status = run_parent_builtin(pl, sh);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	sh->exit_status = status;
	return (status);
}

/*crea el pipe si hace falta, hace fork y conecta el comando con la pipeline*/
static int	fork_pipeline_command(t_cmd *pl, t_shell *sh, int *prev_read)
{
	pid_t	pid;
	int		pipefd[2];

	if (pl->next && pipe(pipefd) == -1)
		return (perror("minishell"), -1);
	pid = fork();
	if (pid < 0)
	{
		if (pl->next)
		{
			close(pipefd[0]);
			close(pipefd[1]);
		}
		return (perror("minishell"), -1);
	}
	if (pid == 0)
	{
		catch_signal_child();
		child_process(*prev_read, pl, pipefd, sh);
	}
	parent_process(prev_read, pl, pipefd);
	return (0);
}

/*coordina la ejecucion y lanza los procesos*/
int	execution(t_cmd *pl, t_shell *sh)
{
	int		prev_read;
	int		status;

	if (!pl)
		return (0);
	resolve_command_type(pl);
	prev_read = -1;
	if (!pl->next && is_parent_builtin(pl))
		return (run_single_parent_builtin(pl, sh));
	catch_signal_wait_parent();
	while (pl)
	{
		if (fork_pipeline_command(pl, sh, &prev_read) == -1)
			return (1);
		pl = pl->next;
	}
	status = wait_all_children();
	catch_signal_father();
	return (status);
}
