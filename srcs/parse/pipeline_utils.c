/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:35:10 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 12:52:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*reserva e inicializa memoria para la estructura final*/
t_command	*pipeline_new(void)
{
	t_command	*node;

	node = (t_command *)malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	*node = (t_command){0};
	node->argv = NULL;
	node->qmask = NULL;
	node->redirs = NULL;
	node->type = CMD_UNKNOWN;
	node->builtin = BI_NONE;
	node->next = NULL;
	return (node);
}

/*anade comando al fina de la lista*/
void	pipeline_add_back(t_command **lst, t_command *new_node)
{
	t_command	*cur;

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

/*reserva e inicializa las redirecciones de la estructura final*/
t_redir	*redir_new(t_token_type type, char *target, char *qmask)
{
	t_redir	*node;

	node = (t_redir *)calloc(1, sizeof(t_redir));
	if (!node)
		return (NULL);
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

/*anade las redirecciones al final de la lista*/
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
