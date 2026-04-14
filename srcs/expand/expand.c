/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/14 15:51:36 by jojeda-p         ###   ########.fr       */
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

static int	question_len(int n)
{
	int	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static void	question_fill(char *dst, int n, int len)
{
	while (len > 0)
	{
		len--;
		dst[len] = (n % 10) + '0';
		n /= 10;
	}
}

static void	question_case(char **argv, char **qmask, t_shell sh)
{
	char	*new_argv;
	char	*new_qmask;
	int		old_len;
	int		num_len;
	int		i;

	i = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i] == '$' && (*argv)[i + 1] == '?' && (*qmask)[i] != '1')
		{
			old_len = ft_strlen(*argv);
			num_len = question_len(sh.exit_status);
			new_argv = malloc(old_len - 2 + num_len + 1);
			new_qmask = malloc(old_len - 2 + num_len + 1);
			if (!new_argv || !new_qmask)
				return (free(new_argv), free(new_qmask), (void)0);
			ft_memcpy(new_argv, *argv, i);
			ft_memcpy(new_qmask, *qmask, i);
			question_fill(new_argv + i, sh.exit_status, num_len);
			old_len = num_len;
			while (num_len > 0)
			{
				num_len--;
				new_qmask[i + num_len] = '0';
			}
			ft_strcpy(new_argv + i + old_len, *argv + i + 2);
			ft_strcpy(new_qmask + i + old_len, *qmask + i + 2);
			free(*argv);
			free(*qmask);
			*argv = new_argv;
			*qmask = new_qmask;
			i += old_len;
		}
		else
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
				question_case(&argv[i], &qmask[i], sh);
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
				expand_word(&redirs->target, &redirs->qmask, sh, i + 1);
			i++;
		}
		redirs = redirs->next;
	}
}

void    expand(t_command *node, t_shell sh)
{
	while (node)
	{
		expand_argv(node->argv, node->qmask, sh);
		expand_redirs(node->redirs, sh);
		node = node->next;
	}
}