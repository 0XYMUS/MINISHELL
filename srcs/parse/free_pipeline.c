/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:59:44 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*libera o argv o qmask*/
static void	free_argv_or_qmask(char **argv_or_qmask)
{
	int	i;

	if (!argv_or_qmask)
		return ;
	i = 0;
	while (argv_or_qmask[i])
	{
		free(argv_or_qmask[i]);
		i++;
	}
	free(argv_or_qmask);
}

/*libera la lista de las redirecciones*/
static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->target);
		free(redir->qmask);
		free(redir);
		redir = next;
	}
}

/*libera toda la lista de comandos de pipeline*/
void	pipeline_free_all(t_cmd **lst)
{
	t_cmd	*cur;
	t_cmd	*next;

	if (!lst)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		free_argv_or_qmask(cur->argv);
		free_argv_or_qmask(cur->qmask);
		free_redirs(cur->redirs);
		free(cur);
		cur = next;
	}
	*lst = NULL;
}
