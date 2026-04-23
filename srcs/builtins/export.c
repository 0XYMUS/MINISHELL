/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 12:35:35 by julepere          #+#    #+#             */
/*   Updated: 2026/04/23 17:09:39 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

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

static int	export_identifier_error(char *name)
{
	write(2, "minishell: export: `", 20);
	write(2, name, ft_strlen(name));
	write(2, "': not a valid identifier\n", 26);
	return (1);
}

static int	valid_export_name(char *name)
{
	int	i;

	if (!name || name[0] == '\0')
		return (0);
	if (!((name[0] >= 'A' && name[0] <= 'Z')
			|| (name[0] >= 'a' && name[0] <= 'z') || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z')
				|| (name[i] >= 'a' && name[i] <= 'z')
				|| (name[i] >= '0' && name[i] <= '9') || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	xy_export(t_cmd *cmd, t_shell *sh)
{
	int	i;
	int	status;

	if (!sh)
		return (0);
	if (!cmd->argv[1])
	{
		sh->exit_status = 0;
		print_declare_x(sh->envp);
		return (0);
	}
	status = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!valid_export_name(cmd->argv[i]))
			status = export_identifier_error(cmd->argv[i]);
		else
			export_set_var(cmd->argv[i], sh);
		i++;
	}
	sh->exit_status = status;
	return (status);
}
