/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/12 16:29:01 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_expansion(char *env, char **word, int i, char **qmask)
{
	char	*expansion;
	char	*newq;
	int		len;
	int		len_q;

	len = expand_len(*word, env, i);
	expansion = malloc(sizeof(char) * len + 1);
	if (!expansion)
		return ;
	complete_expansion(env, *word, i, &expansion);
	expansion[len] = '\0';
	len_q = expansion_len(*word, i);
	newq = update_qmask_after_expansion(*qmask, i, len_q,len);
	if (!newq)
		return (free(expansion), (void)0);
	free((*word));
	free((*qmask));
	(*word) = expansion;
	*qmask = newq;
}

static void expand_word(char **word, char **qmask, t_shell sh, int i)
{
	int j;
	int k;
	int start_i;
	char	c;

	j = 0;
	c = (*qmask)[i - 1];
	start_i = i;
	while (sh.envp[j])
	{
		k = 0;
		while (sh.envp[j][k] && (*word)[i] && sh.envp[j][k] == (*word)[i])
		{
			k++;
			i++;
			if (sh.envp[j][k] == '=' && (!(*word)[i] || (*qmask)[i] != c))
				get_expansion(sh.envp[j], word, start_i - 1, qmask);
		}
		i = start_i;
		j++;
	}
}

static void	question_case(char **argv, char *qmask, t_shell sh)
{
	int		i;

	i = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i] == '$' && (*argv)[i + 1] == '?' && qmask[i] != '1')
		{
			if (sh.exit_status == 0)
			{
				(*argv)[i] = '0';
				str_move(argv, i + 1, -1);
			}
			else if (sh.exit_status == 127)
			{
				str_move(argv, i + 2, 1);
				(*argv)[i] = '1';
				(*argv)[i + 1] = '2';
				(*argv)[i + 2] = '7';
			}
		}
		i++;
	}
}

static void expand_argv(char **argv, char **qmask, t_shell sh)
{
	int i;
	int j;
	
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '$' && argv[i][j + 1] == '?' && qmask[i][j] != '1')
				question_case(&argv[i], qmask[i], sh);
			else if (argv[i][j] == '$' && qmask[i][j] != '1')
				{
					if (!argv[i][j + 1])
						return ;
					expand_word(&argv[i], &qmask[i], sh, j + 1);
				}
			j++;
		}
		i++;
	}
}

static void expand_redirs(t_redir *redirs, t_shell sh)
{
	int i;
	
	while (redirs)
	{
		i = 0;
		while (redirs->target[i])
		{
			if (redirs->target[i] == '$' && redirs->qmask[i] != '1')
				expand_word(&redirs->target, &redirs->qmask, sh, i);
			i++;
		}
		redirs = redirs->next;
	}
}

void    expand(t_pipeline *node, t_shell sh)
{
	while (node)
	{
		expand_argv(node->cmd->argv, node->cmd->qmask, sh);
		expand_redirs(node->cmd->redirs, sh);
		node = node->next;
	}
}