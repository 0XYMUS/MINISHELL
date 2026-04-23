/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:09:19 by julepere          #+#    #+#             */
/*   Updated: 2026/04/23 12:57:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_cd(t_cmd *cmd, t_shell *sh)
{
	const char	*path;

	if (!sh)
		return (0);
	sh->exit_status = 0;
	if (!cmd || !cmd->argv || !cmd->argv[1])
		return (1);
	path = cmd->argv[1];
	if (chdir(path) != 0)
	{
		write(1, "minishell: cd: No such file or directory\n", 41);
		sh->exit_status = 1;
		return (1);
	}
	return (0);
}
