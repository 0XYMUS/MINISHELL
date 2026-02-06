/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:09:19 by julepere          #+#    #+#             */
/*   Updated: 2026/02/06 14:41:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_cd(t_command *cmd, t_shell *sh)
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
        chdir(path);
		write(1, "se ha cambiado a ", 17);
		write(1, path, xy_strlen(path));
        sh->exit_status = 1;
        return (1);
    }
	getenv("HOME");
    return (0);
}