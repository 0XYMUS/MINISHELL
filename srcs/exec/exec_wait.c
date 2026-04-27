/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:47:44 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/27 11:12:07 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>

/*convierte el estado de waitpid al codigo de salida de shell*/
static int	status_to_exit_code(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

/*imprime el mensaje de señal solo una vez por pipeline*/
static void	print_signal_message(int status, int *printed_signal)
{
	if (!WIFSIGNALED(status))
		return ;
	if ((WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT)
		&& !*printed_signal)
	{
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)", 18);
		write(1, "\n", 1);
		*printed_signal = 1;
	}
}

/*espera al ultimo hijo para fijar $? y reapila el resto*/
int	wait_all_children(pid_t last_pid)
{
	int		status;
	int		exit_code;
	int		printed_signal;
	pid_t	waited_pid;

	exit_code = 0;
	printed_signal = 0;
	if (last_pid > 0)
	{
		waited_pid = waitpid(last_pid, &status, 0);
		if (waited_pid == last_pid)
			exit_code = status_to_exit_code(status);
		print_signal_message(status, &printed_signal);
	}
	waited_pid = waitpid(-1, &status, 0);
	while (waited_pid > 0)
	{
		print_signal_message(status, &printed_signal);
		waited_pid = waitpid(-1, &status, 0);
	}
	return (exit_code);
}
