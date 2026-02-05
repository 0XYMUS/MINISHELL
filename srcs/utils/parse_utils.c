/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/05 15:02:28 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redir(t_token_type type)
{
    return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
         || type == TOK_REDIR_OUT);
}

static void	is_builtin_2(char *argv, t_pipeline **node)
{
	if (ft_strncmp(argv, 'unset', 5) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_ECHO;
	}
	else if (ft_strncmp(argv, 'env', 3) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_ECHO;
	}
	else if (ft_strncmp(argv, 'exit', 4) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_ECHO;
	}
}

void	is_builtin(char *argv, t_pipeline **node)
{
	if (ft_strncmp(argv, 'echo', 4) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_ECHO;
	}
	else if (ft_strncmp(argv, 'cd', 2) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_CD;
	}
	else if (ft_strncmp(argv, 'pwd', 3) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_PWD;
	}
	else if (ft_strncmp(argv, 'export', 6) == 0)
	{
		(*node)->cmd->cmd_info.builtin = CMD_BUILTIN;
		(*node)->cmd->cmd_info.type = BI_EXPORT;
	}
	else
		is_builtin2(argv, node);
}

is_external(char *argv, t_pipeline)
{
	
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (!s1[i] && !s2[i])
		return (0);
	else
		return (-1);
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
