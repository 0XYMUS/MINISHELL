/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 14:54:42 by julepere          #+#    #+#             */
/*   Updated: 2026/02/22 00:19:17 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cuenta cuantas posiciones tiene el env
int	env_count(char **envp)
{
	int i;

	i = 0;
	if (!envp)
		return (0);
	while(envp[i])
		i++;
	return (i);
}

char	**env_dup(char **envp)
{
	int		n;
	int		i;
	char	**dup;

	n = env_count(envp);
	dup = malloc(sizeof(char *) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = ft_strdup(envp[i]);
		if (!dup[i])
		{
			env_free(dup);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

void	env_free(char **envp)
{
	int	i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
