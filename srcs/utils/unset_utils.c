/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 02:31:06 by julepere          #+#    #+#             */
/*   Updated: 2026/03/17 00:00:00 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_env_var(t_shell *sh, char *rm_var)
{
	int	idx;

	if (!sh || !sh->envp || !rm_var)
		return (0);
	idx = find_env_var(sh->envp, rm_var);
	if (idx < 0)
		return (0);
	free(sh->envp[idx]);
	while (sh->envp[idx + 1])
	{
		sh->envp[idx] = sh->envp[idx + 1];
		idx++;
	}
	sh->envp[idx] = NULL;
	return (0);
}
