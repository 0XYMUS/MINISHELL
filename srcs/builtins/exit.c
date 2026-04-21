/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:31:36 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_cleanup(t_cmd *cmd, t_shell *sh)
{
	if (cmd)
		pipeline_free_all(&cmd);
	if (sh->tokens)
		token_free_all(&sh->tokens);
	if (sh->pipeline)
		pipeline_free_all(&sh->pipeline);
	env_free(sh->envp);
	clear_history();
}

int	xy_exit(t_cmd *cmd, t_shell *sh)
{
	if (!sh)
		return (0);
	write(1, "exit\n", 5);
	exit_cleanup(cmd, sh);
	exit(sh->exit_status);
}
