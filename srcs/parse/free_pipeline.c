/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:59:44 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/25 21:27:40 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_qmask(char **qmask)
{
	int	i;

	if (!qmask)
		return ;
	i = 0;
	while (qmask[i])
	{
		free(qmask[i]);
		i++;
	}
	free(qmask);
}

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

static void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	free_qmask(cmd->qmask);
	free_redirs(cmd->redirs);
	free(cmd);
}

void	pipeline_free_all(t_command **lst)
{
	t_command	*cur;
	t_command	*next;

	if (!lst)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		free_command(cur);
		cur = next;
	}
	*lst = NULL;
}