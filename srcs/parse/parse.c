/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/27 17:30:35 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_word(t_token **token, t_command **cmd, int *n_arg)
{
	if ((*token)->type == TOK_WORD)
	{
		(*cmd)->argv[*n_arg] = (*token)->value;
		(*n_arg)++;
		*token = (*token)->next;
	}
	return (0);
}

static int	add_reddir(t_token **token, t_command **cmd, int  *n_red)
{
	t_redir	*redir;

	if (is_redir((*token)->type))
	{
		if (!(*token)->next || (*token)->next->type != TOK_WORD)
			return (-1);
		redir = redir_new((*token)->type, (*token)->next->value);
		redir_add_back(&(*cmd)->redirs, redir);
		(*n_red)++;
		*token = (*token)->next->next;
	}
	return (0);
}

t_pipeline	**parse_simple_command(t_pipeline **lst, t_token **token)
{
	t_pipeline	*node;
	int	n_red;
	int	n_arg;

	n_red = 0;
	n_arg = 0;
	if ((*token)->type == TOK_PIPE)
		return (NULL);
	node = pipeline_new();
	if (!node)
		return (NULL);
	while (*token && (*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &node->cmd, &n_red) == -1)
			return (NULL);
		if (add_word(token, &node->cmd, &n_arg) == -1)
			return (NULL);
	}
	if (n_arg < 1)
		return (NULL);
	pipeline_add_back(lst, node);
	return (lst);
}

t_pipeline	**parse (t_token **token)
{
	t_pipeline	**lst;

	while (token)
		lst = parse_simple_command(lst, token);
	return (lst);
}
