/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:09:19 by julepere          #+#    #+#             */
/*   Updated: 2026/02/09 14:33:43 by julepere         ###   ########.fr       */
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
        write(1, "minishell: cd: No existe el archivo o el directorio\n", 52);
        sh->exit_status = 1;
        return (1);
    }
    write(1, "se ha cambiado a ", 17);
    write(1, path, xy_strlen(path));
    write(1, "\n", 1);
    return (0);
}