/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/26 15:55:43 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_simple_command(t_token **token)
{
	t_command	cmd;
	int	n_red;
	int	n_arg;

	n_red = 0;
	n_arg = 0;
	if ((*token)->type == TOK_PIPE)
		return (-1);
	while ((*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &cmd, n_red) == -1)
			return (-1);
		if (add_word(token, &cmd, n_arg) == -1)
			return (-1);
	}
	if (n_arg < 1)
	return (-1);
}

static int	add_word(t_token **token, t_command **cmd, int *n_arg)
{
	if ((*token)->type == TOK_WORD)
	{
		(*cmd)->argv[*n_arg] = token;
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
t_pipeline	parse (t_token **token)
{
	t_pipeline	lst;

	while (token)
	{
		parse_simple_command(token);
	}
}

t_pipeline	pipeline_new()
{
	t_pipeline	*;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (type != TOK_WORD)
		token->value = NULL;
	else
		token->value = value;
	if (space == 1)
		token->space = 1;
	else
		token->space = 0;
	token->next = NULL;
	return (token);
}
