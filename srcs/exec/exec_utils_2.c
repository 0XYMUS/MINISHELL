/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:13:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:41:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <readline/history.h>

/*ejecuta el builtin que debe correr en el proceso padre*/
int	run_parent_builtin(t_cmd *pl, t_shell *sh)
{
	if (pl->builtin == BI_CD)
		return (xy_cd(pl, sh));
	if (pl->builtin == BI_EXPORT)
		return (xy_export(pl, sh));
	if (pl->builtin == BI_UNSET)
		return (xy_unset(pl, sh));
	if (pl->builtin == BI_EXIT)
		return (xy_exit(pl, sh));
	return (0);
}

/*comprueba si el builtin debe ejecutarse en el padre*/
int	is_parent_builtin(t_cmd *pl)
{
	if (pl->builtin == BI_CD || pl->builtin == BI_EXIT
		|| pl->builtin == BI_EXPORT || pl->builtin == BI_UNSET)
		return (1);
	return (0);
}

/*libera estado heredado en el hijo y termina sin pasar por atexit*/
void	child_cleanup_and_exit(t_shell *sh, int status)
{
	if (!sh)
		_exit(status);
	if (sh->tokens)
		token_free_all(&sh->tokens);
	if (sh->pipeline)
		pipeline_free_all(&sh->pipeline);
	env_free(sh->envp);
	clear_history();
	_exit(status);
}
