/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/03 17:06:22 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>

/* NOTE: En rutas OOM forzamos errno=ENOMEM para que perror() sea determinista */

static int	add_word(t_token **token, t_command **cmd, int *n_arg,
			t_parse_error *err)
{
	if ((*token)->type == TOK_WORD)
	{
		(*cmd)->argv[*n_arg] = ft_strdup((*token)->value);
		if (!(*cmd)->argv[*n_arg])
		{
			errno = ENOMEM;
			parse_error_set(err, PERR_OOM, PNEAR_NONE);
			return (-1);
		}
		(*n_arg)++;
		*token = (*token)->next;
	}
	return (0);
}

static int	add_reddir(t_token **token, t_command **cmd, t_parse_error *err)
{
	t_redir	*redir;

	if (is_redir((*token)->type))
	{
		if (!(*token)->next || (*token)->next->type != TOK_WORD)
			return (-1);
		redir = redir_new((*token)->type, (*token)->next->value);
		if (!redir)
		{
			errno = ENOMEM;
			parse_error_set(err, PERR_OOM, PNEAR_NONE);
			return (-1);
		}
		redir_add_back(&(*cmd)->redirs, redir);
		*token = (*token)->next->next;
	}
	return (0);
}

int	parse_simple_command(t_pipeline **lst, t_token **token, t_parse_error *err)
{
	t_pipeline	*node;
	int	n_argv;

	node = pipeline_new();
	if (!node)
	{
		errno = ENOMEM;
		parse_error_set(err, PERR_OOM, PNEAR_NONE);
		return (-1);
	}
	n_argv = argv_len(*token);
	if (n_argv < 0)
		return (pipeline_free_all(&node), -1);
	node->cmd->argv = malloc(sizeof (char *) * (n_argv + 1));
	if (!node->cmd->argv)
	{
		errno = ENOMEM;
		parse_error_set(err, PERR_OOM, PNEAR_NONE);
		return (pipeline_free_all(&node), -1);
	}
	node->cmd->argv[n_argv] = NULL;
	n_argv = 0;
	while (*token && (*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &node->cmd, err) == -1)
			return (pipeline_free_all(&node), -1);
		if (*token && add_word(token, &node->cmd, &n_argv, err) == -1)
			return (pipeline_free_all(&node), -1);
	}
	pipeline_add_back(lst, node);
	return (0);
}

t_pipeline	*parse(t_token **token, t_parse_error *err)
{
	t_pipeline	*lst;

	if (validate_syntax(*token, err) == -1)
		return (NULL);
	lst = NULL;
	while (*token)
	{
		if (parse_simple_command(&lst, token, err) == -1)
			return (pipeline_free_all(&lst), NULL);
		if (*token && (*token)->type == TOK_PIPE)
			*token = (*token)->next;
	}
	return (lst);
}
