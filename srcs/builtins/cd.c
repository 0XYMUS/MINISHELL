/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:09:19 by julepere          #+#    #+#             */
/*   Updated: 2026/04/23 16:36:00 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	xy_cd(t_cmd *cmd, t_shell *sh)
{
	const char	*path;

	if (!sh)
		return (0);
	sh->exit_status = 0;
	if (!cmd || !cmd->argv || !cmd->argv[1])
		return (1);
	if (cmd->argv[2])
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		sh->exit_status = 1;
		return (1);
	}
	path = cmd->argv[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}
