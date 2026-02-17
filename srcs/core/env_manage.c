/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 14:54:42 by julepere          #+#    #+#             */
/*   Updated: 2026/02/17 14:55:00 by julepere         ###   ########.fr       */
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
