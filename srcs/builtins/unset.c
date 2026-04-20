/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 02:16:43 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	valid_unset_name(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!((name[0] >= 'A' && name[0] <= 'Z')
			|| (name[0] >= 'a' && name[0] <= 'z') || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z')
				|| (name[i] >= 'a' && name[i] <= 'z')
				|| (name[i] >= '0' && name[i] <= '9') || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	unset_identifier_error(char *name)
{
	write(2, "minishell: unset: `", 19);
	write(2, name, ft_strlen(name));
	write(2, "': not a valid identifier\n", 27);
	return (1);
}

static void	unset_one_var(t_shell *sh, char *name)
{
	remove_env_var(sh, name);
}

int	xy_unset(t_cmd *cmd, t_shell *sh)
{
	int	i;

	if (!sh || !cmd || !cmd->argv || !cmd->argv[1])
		return (0);
	sh->exit_status = 0;
	i = 1;
	while (cmd->argv[i])
	{
		if (!valid_unset_name(cmd->argv[i]))
			sh->exit_status = unset_identifier_error(cmd->argv[i]);
		else
			unset_one_var(sh, cmd->argv[i]);
		i++;
	}
	return (0);
}
