/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:35:10 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/29 16:00:22 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipeline	*pipeline_new(void)
{
	t_pipeline	*node;

	node = (t_pipeline *)malloc(sizeof(t_pipeline));
	if (!node)
		return (NULL);
	node->cmd = malloc(sizeof(t_command));
	if (!node->cmd)
		return (NULL);
	node->cmd->argv = NULL;
	node->cmd->space = NULL;
	node->cmd->redirs = NULL;
	node->cmd->builtin = BI_NONE;
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
t_redir	*redir_new(t_token_type type, char *target)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	if (type == TOK_APPEND)
		node->type = R_APPEND;
	else if (type == TOK_HEREDOC)
		node->type = R_HEREDOC;
	else if (type == TOK_REDIR_IN)
		node->type = R_IN;
	else if (type == TOK_REDIR_OUT)
		node->type = R_OUT;
	node->target = ft_strdup(target);
	if (!node->target)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

void	redir_add_back(t_redir **lst, t_redir *new_node)
{
	t_redir	*cur;

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
