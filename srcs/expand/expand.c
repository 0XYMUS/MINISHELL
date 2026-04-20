/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*funcion algo compleja:
realiza filtros y devuelve-
1 si ha realiado una expansion
0 si no ha podido aplicarla en esa posicion(debido a las reglas)
-1 si $ esta solo al final de la linea*/
static int	handle_dollar_expansion(char **arg, char **qmask, t_shell sh, int j)
{
	if ((*arg)[j] != '$' || (*qmask)[j] == '1'
		|| !same_qmask_context(*qmask, j))
		return (0);
	if ((*arg)[j + 1] == '?')
	{
		question_case(arg, qmask, sh);
		return (1);
	}
	if ((*arg)[j + 1] == '$')
	{
		handle_pid_expansion(arg, qmask, j);
		return (1);
	}
	if (!(*arg)[j + 1])
		return (-1);
	expand_word(arg, qmask, sh, j + 1);
	return (1);
}

/*recorre argv aplicando las epansiones*/
static void	expand_single_arg(char **arg, char **qmask, t_shell sh)
{
	int	j;
	int	status;

	j = 0;
	while ((*arg)[j])
	{
		status = handle_dollar_expansion(arg, qmask, sh, j);
		if (status == 1 || ((*arg)[j] == '$' && (*qmask)[j] == '0'
			&& (*arg)[j + 1] && is_var_char((*arg)[j + 1])
			&& !same_qmask_context(*qmask, j)))
		{
			if (status != 1)
				remove_dollar_only(arg, qmask, j);
			j = -1;
		}
		j++;
	}
}

/*expande el argv*/
static void	expand_argv(char **argv, char **qmask, t_shell sh)
{
	int	i;
	int	had_quote;

	i = 0;
	while (argv[i])
	{
		had_quote = has_quoted_char(qmask[i]);
		expand_single_arg(&argv[i], &qmask[i], sh);
		if (!argv[i][0] && !had_quote)
			remove_empty_unquoted_arg(argv, qmask, i);
		else
			i++;
	}
}

/*expande redirecciones*/
static void	expand_redirs(t_redir *redirs, t_shell sh)
{
	int	i;

	while (redirs)
	{
		i = 0;
		while (redirs->target[i])
		{
			if (redirs->target[i] == '$' && redirs->qmask[i] != '1')
				expand_word(&redirs->target, &redirs->qmask, sh, i + 1);
			i++;
		}
		redirs = redirs->next;
	}
}

/*funcion general de expansiones*/
void	expand(t_cmd *node, t_shell sh)
{
	while (node)
	{
		expand_argv(node->argv, node->qmask, sh);
		expand_redirs(node->redirs, sh);
		node = node->next;
	}
}
