/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:59:44 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/29 16:00:25 by jojeda-p         ###   ########.fr       */
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

static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->target);
		free(redir);
		redir = next;
	}
}

static void	free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	free_argv(cmd->argv);
	free_redirs(cmd->redirs);
	free(cmd);
}

void	pipeline_free_all(t_pipeline **lst)
{
	t_pipeline	*cur;
	t_pipeline	*next;

	if (!lst)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		free_command(cur->cmd);
		free(cur);
		cur = next;
	}
	*lst = NULL;
}