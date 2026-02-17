/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/17 15:44:35 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* NOTE: En rutas OOM forzamos errno=ENOMEM para que perror() sea determinista */

static char	*qmask_dup(const char *src)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static int	qmask_has_quote(const char *qmask)
{
	if (!qmask)
		return (0);
	while (*qmask)
	{
		if (*qmask != '0')
			return (1);
		qmask++;
	}
	return (0);
}

static int	add_word(t_token **token, t_command **cmd, int *n_arg,
			t_error *err)
{
	if ((*token)->type == TOK_WORD)
	{
		(*cmd)->argv[*n_arg] = ft_strdup((*token)->value);
		if (!(*cmd)->argv[*n_arg])
		{
			errno = ENOMEM;
			error_set(err, PERR_OOM, PNEAR_NONE);
			return (-1);
		}
		(*cmd)->qmask[*n_arg] = qmask_dup((*token)->qmask);
		if ((*token)->qmask && !(*cmd)->qmask[*n_arg])
		{
			errno = ENOMEM;
			error_set(err, PERR_OOM, PNEAR_NONE);
			return (-1);
		}
		(*n_arg)++;
		*token = (*token)->next;
	}
	return (0);
}

static int	add_reddir(t_token **token, t_command **cmd, t_error *err)
{
	t_redir	*redir;

	if (is_redir((*token)->type))
	{
		if (!(*token)->next)
			return (error_set(err, PERR_REDIR_NO_TARGET, PNEAR_NEWLINE), -1);
		if ((*token)->next->type != TOK_WORD)
			return (error_set(err, PERR_REDIR_NO_TARGET, near_from_token((*token)->next->type)), -1);
		redir = redir_new((*token)->type, (*token)->next->value,
			(*token)->next->qmask);
		if (!redir)
		{
			errno = ENOMEM;
			error_set(err, PERR_OOM, PNEAR_NONE);
			return (-1);
		}
		if ((*token)->type == TOK_HEREDOC
			&& qmask_has_quote((*token)->next->qmask))
			redir->expand = 0;
		redir_add_back(&(*cmd)->redirs, redir);
		*token = (*token)->next->next;
	}
	return (0);
}

int	init_command(t_pipeline **node, t_token **token, t_error *err)
{
	int	n_argv;

	*node = pipeline_new();
	if (!(*node))
	{
		errno = ENOMEM;
		error_set(err, PERR_OOM, PNEAR_NONE);
		return (-1);
	}
	n_argv = argv_len(*token);
	if (n_argv < 0)
		return (pipeline_free_all(node), -1);
	(*node)->cmd->argv = malloc(sizeof (char *) * (n_argv + 1));
	if (!(*node)->cmd->argv)
	{
		errno = ENOMEM;
		error_set(err, PERR_OOM, PNEAR_NONE);
		return (pipeline_free_all(node), -1);
	}
	(*node)->cmd->qmask = malloc(sizeof(char *) * (n_argv + 1));
	if (!(*node)->cmd->qmask)
	{
		errno = ENOMEM;
		error_set(err, PERR_OOM, PNEAR_NONE);
		return (pipeline_free_all(node), -1);
	}
	while (n_argv >= 0)
	{
		(*node)->cmd->argv[n_argv] = NULL;
		(*node)->cmd->qmask[n_argv] = NULL;
		n_argv--;
	}
	return (0);
}

/* int	correct_command(char *argv, t_pipeline **node, t_error *err)
{
	is_builtin(argv, node);
	if ((*node)->cmd->cmd_info.type == CMD_BUILTIN)
		return (0);
	if (is_external(argv, node, err) == -1)
		return (-1);
	return (0);
} */

/* int	check_command(t_pipeline **node, t_error *err)
{
	if (!(*node)->cmd->argv[0] && (*node)->cmd->redirs)
		return (0);
	if (!(*node)->cmd->argv[0] && !(*node)->cmd->redirs)
	{
		errno = ENOMEM;
		error_set(err, PERR_NONE, PNEAR_NONE);
		return (-1);
	}
	if (correct_command((*node)->cmd->argv[0], node, err) == -1)
	{
		errno = ENOMEM;
		error_set(err, PERR_UNEXPECTED_TOKEN, PNEAR_WORD);
		return (-1);
	}
	return (0);
} */

int	parse_simple_command(t_pipeline **lst, t_token **token, t_error *err)
{
	t_pipeline	*node;
	int	n_argv;

	if (init_command(&node, token, err) == -1)
		return (-1);
	n_argv = 0;
	while (*token && (*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &node->cmd, err) == -1)
			return (pipeline_free_all(&node), -1);
		if (*token && add_word(token, &node->cmd, &n_argv, err) == -1)
			return (pipeline_free_all(&node), -1);
	}
	/* if (check_command(&node, err) == -1)
		return (pipeline_free_all(&node), -1); */
	pipeline_add_back(lst, node);
	return (0);
}

t_pipeline	*parse(t_token **token, t_error *err)
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
