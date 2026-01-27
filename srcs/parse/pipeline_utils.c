/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:35:10 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/27 13:35:34 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	command_init(t_command *cmd)
{
	if (!cmd)
		return ;
	cmd->argv = NULL;
	cmd->space = NULL;
	cmd->redirs = NULL;
	cmd->builtin = BI_NONE;
}

t_pipeline	*pipeline_new(void)
{
	t_pipeline	*node;

	node = (t_pipeline *)malloc(sizeof(t_pipeline));
	if (!node)
		return (NULL);
	command_init(&node->cmd);
	node->next = NULL;
	return (node);
}

void	pipeline_add_back(t_pipeline **lst, t_pipeline *new_node)
{
	t_pipeline	*cur;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}
