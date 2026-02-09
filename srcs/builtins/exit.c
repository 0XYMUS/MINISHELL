/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:31:36 by julepere          #+#    #+#             */
/*   Updated: 2026/02/09 21:24:43 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_exit(t_command *cmd, t_shell *sh)
{
	(void)cmd;
	if (!sh)
		return (0);
	write(1, "exit\n", 5);
	exit(sh->exit_status);
}