/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/06 12:40:39 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redir(t_token_type type)
{
    return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
         || type == TOK_REDIR_OUT);
}

static void	is_builtin2(char *argv, t_pipeline **node)
{
	if (xy_streq(argv, "unset"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_UNSET;
	}
	else if (xy_streq(argv, "env"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_ENV;
	}
	else if (xy_streq(argv, "exit"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_EXIT;
	}
}

void	is_builtin(char *argv, t_pipeline **node)
{
	if (xy_streq(argv, "echo"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_ECHO;
	}
	else if (xy_streq(argv, "cd"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_CD;
	}
	else if (xy_streq(argv, "pwd"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_PWD;
	}
	else if (xy_streq(argv, "export"))
	{
		(*node)->cmd->cmd_info.type = CMD_BUILTIN;
		(*node)->cmd->cmd_info.builtin = BI_EXPORT;
	}
	else
		is_builtin2(argv, node);
}

int	is_external(char *argv, t_pipeline **node, t_error *err)
{
	if ((argv[0] == '/' || (argv[0] == '.' && argv[1] == '/')) && *node)
	{
		if (access(argv,F_OK) == 0)
		{
			if (access(argv, X_OK) == 0)
				return (0);
			else
				return(parse_error_set(err, PERR_PERMISSION_DENIED, PNEAR_NONE), -1);
		}
		else
			return(parse_error_set(err, PERR_NOT_FOUND, PNEAR_NONE), -1);
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	if (!s)
		return (NULL);
	len = xy_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

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
