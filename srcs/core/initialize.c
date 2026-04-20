/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:08:19 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 15:58:05 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shell_init(t_shell *sh, char **envp)
{
	if (!sh)
		return (1);
	sh->line = NULL;
	sh->tokens = NULL;
	sh->pipeline = NULL;
	sh->exit_status = 0;
	error_init(&sh->err);
	sh->envp = env_dup(envp);
	if (!sh->envp)
		return (1);
	return (0);
}
