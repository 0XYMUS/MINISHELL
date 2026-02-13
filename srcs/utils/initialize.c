/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:08:19 by julepere          #+#    #+#             */
/*   Updated: 2026/02/13 16:45:09 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_init(t_shell *sh)
{
	if (!sh)
		return (1);
	sh->line = NULL;
	sh->envp = NULL;
	sh->exit_status = 0;
	sh->tokens = NULL;
	sh->pipeline = NULL;
	error_init(&sh->err);
	return (0);
}