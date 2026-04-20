/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_token_type type)
{
	return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
		|| type == TOK_REDIR_OUT);
}

static t_builtin_cmd	get_builtin_type(char *cmd)
{
	if (!cmd)
		return (BI_NONE);
	if (xy_streq(cmd, "echo"))
		return (BI_ECHO);
	if (xy_streq(cmd, "cd"))
		return (BI_CD);
	if (xy_streq(cmd, "pwd"))
		return (BI_PWD);
	if (xy_streq(cmd, "export"))
		return (BI_EXPORT);
	if (xy_streq(cmd, "unset"))
		return (BI_UNSET);
	if (xy_streq(cmd, "env"))
		return (BI_ENV);
	if (xy_streq(cmd, "exit"))
		return (BI_EXIT);
	return (BI_NONE);
}

void	set_cmd_type(t_cmd *node)
{
	if (!node || !node->argv || !node->argv[0])
	{
		node->type = CMD_UNKNOWN;
		node->builtin = BI_NONE;
		return ;
	}
	node->builtin = get_builtin_type(node->argv[0]);
	if (node->builtin != BI_NONE)
		node->type = CMD_BUILTIN;
	else
		node->type = CMD_EXTERNAL;
}

int	argv_len(t_token *token)
{
	int	n_argv;

	n_argv = 0;
	while (token && token->type != TOK_PIPE)
	{
		if (is_redir(token->type))
		{
			if (!token->next)
				return (-1);
			if (token->type == TOK_HEREDOC
				&& token->next->type != TOK_DELIMITER)
				return (-1);
			if (token->type != TOK_HEREDOC && token->next->type != TOK_WORD)
				return (-1);
			token = token->next->next;
		}
		else if (token->type == TOK_WORD)
		{
			token = token->next;
			n_argv++;
		}
		else
			token = token->next;
	}
	return (n_argv);
}
