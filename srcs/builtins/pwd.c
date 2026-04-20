/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:22:52 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 15:49:08 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_pwd(t_command *cmd, t_shell *sh)
{
	char	*cwd;

	(void)cmd;
	if (!sh)
		return (0);
	sh->exit_status = 0;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		sh->exit_status = 1;
		return (0);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	free(cwd);
	return (0);
}
