/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:09:19 by julepere          #+#    #+#             */
/*   Updated: 2026/02/06 12:48:03 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int xy_cd(t_command *cmd, t_shell *sh)
{
	char path;

	path = cmd->argv[1];
	if (chdir(path))
	{
		write(1, "se ha cambiado a ", 17);
		write(1, path, xy_strlen(path));
	
		sh->exit_status = 0;
		chdir(path);
	}
	else
	{
		write(1, "ruta no cambiada", 16);
		sh->exit_status = 1;
	}
	return (0);
}