/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 12:22:52 by julepere          #+#    #+#             */
/*   Updated: 2026/02/17 15:10:38 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int xy_n_flag(const char *s)
{
    size_t i;

    if (!s || s[0] != '-' || s[1] != 'n')
        return 0;

    i = 2;
    while (s[i] == 'n')
        i++;

    return (s[i] == '\0');
}

int	xy_echo(t_command *cmd, t_shell *sh)
{
	int i;
	int n_flag;

	i = 1;
	n_flag = 0;
	if(!sh)
		return(0);
	sh->exit_status = 0;
	if(!cmd || !cmd->argv || !cmd->argv[0])		
		return (0);

	while(cmd->argv[i] && xy_n_flag(cmd->argv[i]))
	{
		n_flag = 1;
		i++;
	}
	while(cmd->argv[i])
	{
		write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
		if(cmd->argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
   		write(1, "\n", 1);
	return(0);
}