/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 11:52:58 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 17:45:28 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include "minishell.h"

static void	handle_father_sigint(int sign)
{
	(void)sign;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	catch_signal_father(void)
{
	signal(SIGINT, handle_father_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	catch_signal_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static void	handle_wait_sigint(int sign)
{
	(void)sign;
	g_signal = SIGINT;
}

void	catch_signal_wait_parent(void)
{
	signal(SIGINT, handle_wait_sigint);
	signal(SIGQUIT, SIG_IGN);
}
