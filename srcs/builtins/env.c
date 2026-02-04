/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 13:21:47 by julepere          #+#    #+#             */
/*   Updated: 2026/02/04 12:15:39 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	xy_env(t_command *cmd, t_shell *sh)
{
    int		i;
    char	**envp;

    (void)cmd;
    if (!sh)
        return (0);
    sh->exit_status = 0;
    envp = sh->envp;
    if (!envp)
        return (0);
    i = 0;
    while (envp[i])
    {
        write(1, envp[i], xy_strlen(envp[i]));
        write(1, "\n", 1);
        i++;
    }
    return (0);
}