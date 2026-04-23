/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:35:10 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:32:59 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*reserva e inicializa memoria para la estructura final*/
t_cmd	*pipeline_new(void)
{
	t_cmd	*node;

	node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	*node = (t_cmd){0};
	node->argv = NULL;
	node->qmask = NULL;
	node->redirs = NULL;
	node->type = CMD_UNKNOWN;
	node->builtin = BI_NONE;
	node->next = NULL;
	return (node);
}

/*anade comando al fina de la lista*/
void	pipeline_add_back(t_cmd **lst, t_cmd *new_node)
{
	t_cmd	*cur;

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

/*majedo de error y liberacion de memoria en las redirecciones*/
static t_redir	*redir_fail(t_redir *node)
{
	if (node)
	{
		free(node->target);
		free(node->qmask);
		free(node);
	}
	return (NULL);
}

static void	init_node(t_redir *node)
{
	node->type = R_IN;
	node->target = NULL;
	node->qmask = NULL;
	node->expand = 0;
	node->next = NULL;
}

/*reserva e inicializa las redirecciones de la estructura final*/
t_redir	*redir_new(t_token_type type, char *target, char *qmask)
{
	t_redir	*node;

	node = (t_redir *)malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	init_node(node);
	if (type == TOK_APPEND)
		node->type = R_APPEND;
	else if (type == TOK_HEREDOC)
		node->type = R_HEREDOC;
	else if (type == TOK_REDIR_IN)
		node->type = R_IN;
	else
		node->type = R_OUT;
	node->target = ft_strdup(target);
	if (!node->target)
		return (redir_fail(node));
	if (qmask)
	{
		node->qmask = ft_strdup(qmask);
		if (!node->qmask)
			return (redir_fail(node));
	}
	node->expand = 1;
	return (node);
}
