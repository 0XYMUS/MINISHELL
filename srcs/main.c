/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/04/23 17:45:34 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include "minishell.h"

volatile sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	t_shell	sh;

	(void)argc;
	(void)argv;
	if (shell_init(&sh, envp) != 0)
		return (1);
	shell_loop(&sh);
	env_free(sh.envp);
	clear_history();
	return (0);
}
