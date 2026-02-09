/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:31:36 by julepere          #+#    #+#             */
/*   Updated: 2026/02/09 18:53:36 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_exit(t_command *cmd, t_shell *sh)
{
	(void)cmd;

	printf("exit\n");
	exit(sh->exit_status);
}