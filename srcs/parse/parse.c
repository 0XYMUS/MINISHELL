/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/27 15:05:09 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	while ((*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &node->cmd, n_red) == -1)
			return (NULL);
		if (add_word(token, &node->cmd, n_arg) == -1)
			return (NULL);
	}
	if (n_arg < 1)
		return (NULL);
	pipeline_add_back(lst, node);
	return (lst);
}

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
	if (is_redir((*token)->type))
	{
		if (!(*token)->next || (*token)->next->type != TOK_WORD)
			return (-1);
		if ((*token)->type == TOK_APPEND)
			(*cmd)->redirs[*n_red].type = R_APPEND;
		else if ((*token)->type == TOK_HEREDOC)
			(*cmd)->redirs[*n_red].type = R_HEREDOC;
		else if ((*token)->type == TOK_REDIR_IN)
			(*cmd)->redirs[*n_red].type = R_IN;
		else if ((*token)->type == TOK_REDIR_OUT)
			(*cmd)->redirs[*n_red].type = R_OUT;
		*token = (*token)->next;
		(*cmd)->redirs[*n_red].target = token;
		(*n_red)++;
		token = (*token)->next;
	}
	return (0);
}
t_pipeline	**parse (t_token **token)
{
	t_pipeline	**lst;

	while (token)
		lst = parse_simple_command(lst, token);
	return (lst);
}
