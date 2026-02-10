/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/10 16:57:52 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  expand_len(char *word, char *env, int i)
{
	int	j;
	int	len;
	
	j = 0;
	len = 0;
	while (word[len] && word[len] != '$')
		len++;
	while (env[j] && env[j] != '=')
		j++;
	while (env[j + 1])
	{
		j++;
		len++;
	}
	while (word[i])
	{
		i++;
		len++;
	}
	return (len - 1);
}

static void complete_expansion(char *env, char *word, int i, char **expansion)
{
	int	j;
	int	k;
	
	j = 0;
	while (word[j] && word[j] != '$')
	{
		if (word[j] != '$')
			(*expansion)[j] = word[j];
		j++;
	}
	k = 0;
	while (env[k] && env[k] != '=')
		k++;
	while (env[k + 1])
	{
		(*expansion)[j] = env[k + 1];
		j++;
		k++;
	}
	while (word[i])
	{
		(*expansion)[j] = word[i];
		i++;
		j++;
	}
}

static char *get_expansion(char *env, char *word, int i)
{
	char	*expansion;
	int		len;


	len = expand_len(word, env, i);
	expansion = malloc(sizeof(char) * len + 1);
	if (!expansion)
		return (NULL);
	complete_expansion(env, word, i, &expansion);
	expansion[len] = '\0';
	return (expansion);
}

static void env_find_word(char **word, char *qmask, t_shell sh, int i)
{
	int j;
	int k;
	int start_i;
	char    *expansion;
	char	c;

	j = 0;
	c = qmask[i - 1];
	start_i = i;
	while (sh.envp[j])
	{
		k = 0;
		while (sh.envp[j][k] && (*word)[i] && sh.envp[j][k] == (*word)[i])
		{
			k++;
			i++;
			if (sh.envp[j][k] == '=' && (!(*word)[i] || qmask[i] != c))
			{
				expansion = get_expansion(sh.envp[j], *word, i);
				free((*word));
				(*word) = expansion;
			}
		}
		i = start_i;
		j++;
	}
}

static void expand_word(char **word, char *qmask, t_shell sh, int i)
{
	i++;
	if (!(*word)[i])
		return ;
	env_find_word(word, qmask, sh, i);
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
			if (argv[i][j] == '$' && argv[i][j + 1] == '?')
				special_case;
			else if (argv[i][j] == '$' && qmask[i][j] != '1')
				expand_word(&argv[i], qmask[i], sh, j);
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
				expand_word(&redirs->target, redirs->qmask, sh, i);
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