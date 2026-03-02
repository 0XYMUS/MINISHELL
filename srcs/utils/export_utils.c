/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:56:48 by julepere          #+#    #+#             */
/*   Updated: 2026/03/02 23:45:50 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_env_var(char **envp, const char *name)
{
	int	i;
	int	len;
	int	name_len;

	if (!envp || !name)
		return (-1);
	len = ft_strlen(name);
	name_len = len;
	i = 0;
	while (i < len && name[i] != '=')
		i++;
	if (i < len)
		name_len = i;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0
			&& envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	export_set_var(char *arg, t_shell *sh)
{
	char	*eq_pos;

	if (!arg)
		return (1);
	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		return (export_with_value(arg, eq_pos, sh));
	return (0);
}

int	export_with_value(char *arg, char *eq_pos, t_shell *sh)
{
	int	idx;
	char	*new_var;

	(void)eq_pos;
	idx = find_env_var(sh->envp, arg);
	if (idx >= 0)
		return (update_env_var(sh, idx, arg));
	new_var = ft_strdup(arg);
	if (!new_var)
		return (1);
	if (add_env_var(sh, new_var))
	{
		free(new_var);
		return (1);
	}
	return (0);
}

int	update_env_var(t_shell *sh, int idx, char *arg)
{
	char	*new_var;

	new_var = ft_strdup(arg);
	if (!new_var)
		return (1);
	free(sh->envp[idx]);
	sh->envp[idx] = new_var;
	return (0);
}

int	add_env_var(t_shell *sh, char *new_var)
{
	int		count;
	char	**new_envp;
	int		i;

	count = env_count(sh->envp);
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (1);
	i = 0;
	while (i < count)
	{
		new_envp[i] = sh->envp[i];
		i++;
	}
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(sh->envp);
	sh->envp = new_envp;
	return (0);
}