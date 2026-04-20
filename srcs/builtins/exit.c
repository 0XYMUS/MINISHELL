/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:31:36 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 15:42:30 by jojeda-p         ###   ########.fr       */
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
