/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:24:47 by julepere          #+#    #+#             */
/*   Updated: 2026/04/27 11:41:03 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

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
static pid_t	fork_pipeline_command(t_cmd *pl, t_shell *sh, int *prev_read)
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
	return (pid);
}

/* comprueba heredocs, resuelve tipos y ejecuta el builtin del padre si toca */
static int	pre_exec_checks(t_cmd *pl, t_shell *sh)
{
	if (prepare_heredocs(pl, sh) == -1)
	{
		if (sh->exit_status == 130)
			return (130);
		return (1);
	}
	resolve_command_type(pl);
	if (!pl->next && is_parent_builtin(pl))
		return (run_single_parent_builtin(pl, sh));
	return (-1);
}

/* lanza la pipeline y recoge el estado final del ultimo proceso */
static int	launch_pipeline(t_cmd *pl, t_shell *sh)
{
	int		prev_read;
	pid_t	last_pid;
	pid_t	pid;

	prev_read = -1;
	last_pid = -1;
	catch_signal_wait_parent();
	while (pl)
	{
		pid = fork_pipeline_command(pl, sh, &prev_read);
		if (pid == -1)
			return (1);
		if (pl->next == NULL)
			last_pid = pid;
		pl = pl->next;
	}
	return (wait_all_children(last_pid));
}

/*coordina la ejecucion y lanza los procesos*/
int	execution(t_cmd *pl, t_shell *sh)
{
	int	status;

	if (!pl)
		return (0);
	status = pre_exec_checks(pl, sh);
	if (status != -1)
		return (status);
	status = launch_pipeline(pl, sh);
	catch_signal_father();
	return (status);
}
