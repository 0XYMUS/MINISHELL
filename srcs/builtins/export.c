/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:35:35 by julepere          #+#    #+#             */
/*   Updated: 2026/03/02 17:17:26 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_declare_x(char **envp)
{
	int		i;
	char	*sep;

	i = 0;
	while (envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (sep)
		{
			write(1, "declare -x ", 11);
			write(1, envp[i], sep - envp[i]);
			write(1, "=\"", 2);
			write(1, sep + 1, ft_strlen(sep + 1));
			write(1, "\"\n", 2);
		}
		i++;
	}
}

int	xy_export(t_command *cmd, t_shell *sh)
{
	char	*arg;

	if (!sh)
		return (0);
	if (!cmd->argv[1])
	{
		sh->exit_status = 0;
		print_declare_x(sh->envp);
	}
	else
	{
		arg = cmd->argv[1];
		sh->exit_status = export_set_var(arg, sh);
	}
	return (0);
}
