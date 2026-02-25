/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/25 21:27:40 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redir(t_token_type type)
{
    return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
         || type == TOK_REDIR_OUT);
}

/* static void	is_builtin2(char *argv, t_command **node)
{
	if (xy_streq(argv, "unset"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_UNSET;
	}
	else if (xy_streq(argv, "env"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_ENV;
	}
	else if (xy_streq(argv, "exit"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_EXIT;
	}
} */

/* void	is_builtin(char *argv, t_command **node)
{
	if (xy_streq(argv, "echo"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_ECHO;
	}
	else if (xy_streq(argv, "cd"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_CD;
	}
	else if (xy_streq(argv, "pwd"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_PWD;
	}
	else if (xy_streq(argv, "export"))
	{
		(*node)->type = CMD_BUILTIN;
		(*node)->builtin = BI_EXPORT;
	}
	else
		is_builtin2(argv, node);
} */

/* int	is_external(char *argv, t_command **node, t_error *err)
{
	if (argv[0] == '/' || (argv[0] == '.' && argv[1] == '/')
	 || (argv[0] == '.' && argv[1] == '.' && argv[2] == '/'))
	{
		if (access(argv,F_OK) == 0)
		{
			if (access(argv, X_OK) == 0)
			{
				(*node)->type = CMD_EXTERNAL;
				return (0);
			}
				else
				return(error_set(err, PERR_PERMISSION_DENIED, PNEAR_NONE), -1);
		}
		else
			return(error_set(err, PERR_NOT_FOUND, PNEAR_NONE), -1);
	}
	(*node)->type = CMD_UNKNOWN;
	return (0);
} */

int	argv_len(t_token *token)
{
	int	n_argv;

	n_argv = 0;
	while (token && token->type != TOK_PIPE)
	{
		if (is_redir(token->type))
		{
			if (!token->next || token->next->type != TOK_WORD)
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
