/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 12:49:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*si el token es una palabrala anade a la sestructura final*/
static int	add_word(t_token **token, t_command **cmd, int *n_arg,
			t_error *err)
{
	if ((*token)->type == TOK_WORD)
	{
		(*cmd)->argv[*n_arg] = ft_strdup((*token)->value);
		if (!(*cmd)->argv[*n_arg])
			return (ft_fail(NULL, err));
		(*cmd)->qmask[*n_arg] = qmask_dup((*token)->qmask);
		if ((*token)->qmask && !(*cmd)->qmask[*n_arg])
			return (ft_fail(NULL, err));
		(*n_arg)++;
		*token = (*token)->next;
	}
	return (0);
}

/*si el token es un redir lo valida y lo anade a la estructura final*/
static int	add_reddir(t_token **token, t_command **cmd, t_error *err)
{
	t_redir	*redir;

	if (is_redir((*token)->type))
	{
		if (!(*token)->next)
			return (error_fail(err, PERR_REDIR_NO_TARGET, PNEAR_NEWLINE));
		if ((*token)->type == TOK_HEREDOC
			&& (*token)->next->type != TOK_DELIMITER)
			return (error_fail(err, PERR_REDIR_NO_TARGET,
					near_from_token((*token)->next->type)));
		if ((*token)->type != TOK_HEREDOC && (*token)->next->type != TOK_WORD)
			return (error_fail(err, PERR_REDIR_NO_TARGET,
					near_from_token((*token)->next->type)));
		redir = redir_new((*token)->type, (*token)->next->value,
				(*token)->next->qmask);
		if (!redir)
			return (ft_fail(NULL, err));
		if ((*token)->type == TOK_HEREDOC
			&& qmask_has_quote((*token)->next->qmask))
			redir->expand = 0;
		redir_add_back(&(*cmd)->redirs, redir);
		*token = (*token)->next->next;
	}
	return (0);
}

/*inicializa los nodos a partir de la tokenizacion y crea la estructura final*/
int	init_command(t_command **node, t_token **token, t_error *err)
{
	int	n_argv;

	*node = pipeline_new();
	if (!(*node))
		return (ft_fail(NULL, err));
	n_argv = argv_len(*token);
	if (n_argv < 0)
		return (pipeline_free_all(node), -1);
	(*node)->argv = malloc(sizeof (char *) * (n_argv + 1));
	if (!(*node)->argv)
		return (ft_fail(node, err));
	(*node)->qmask = malloc(sizeof(char *) * (n_argv + 1));
	if (!(*node)->qmask)
		return (ft_fail(node, err));
	while (n_argv >= 0)
	{
		(*node)->argv[n_argv] = NULL;
		(*node)->qmask[n_argv] = NULL;
		n_argv--;
	}
	return (0);
}

/*construye los comandos hasta la siguinte pipe*/
int	parse_simple_command(t_command **lst, t_token **token, t_error *err)
{
	t_command	*node;
	int			n_argv;

	if (init_command(&node, token, err) == -1)
		return (-1);
	n_argv = 0;
	while (*token && (*token)->type != TOK_PIPE)
	{
		if (add_reddir(token, &node, err) == -1)
			return (pipeline_free_all(&node), -1);
		if (*token && add_word(token, &node, &n_argv, err) == -1)
			return (pipeline_free_all(&node), -1);
	}
	set_command_type(node);
	pipeline_add_back(lst, node);
	return (0);
}

/*funcion rincipal que convierte tokens a estructura final paseada*/
t_command	*parse(t_token **token, t_error *err)
{
	t_command	*lst;

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
