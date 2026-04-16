/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/16 16:27:16 by jojeda-p         ###   ########.fr       */
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

static int	is_var_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

static int	has_quoted_char(char *qmask)
{
	int	i;

	i = 0;
	while (qmask && qmask[i])
	{
		if (qmask[i] == '1')
			return (1);
		i++;
	}
	return (0);
}

static int	match_env(char *env, char *word, char *qmask, int i)
{
	int	k;

	k = 0;
	while (env[k] && word[i] && env[k] == word[i])
	{
		k++;
		i++;
		if (env[k] == '='
			&& (!word[i] || !is_var_char(word[i])
				|| qmask[i] != qmask[i - 1]))
			return (1);
	}
	return (0);
}


static void	remove_word(char **word, char **qmask, int start)
{
	int	end;
	int	len;

	end = start + 1;
	while ((*word)[end] && is_var_char((*word)[end]))
		end++;
	end--;
	len = ft_strlen(*word);
	ft_memmove(*word + start, *word + end + 1, len - end);
	ft_memmove(*qmask + start, *qmask + end + 1, len - end);
}

static void expand_word(char **word, char **qmask, t_shell sh, int i)
{
	int j;
	int start_i;
	int found;

	j = 0;
	start_i = i;
	found = 0;
	while (sh.envp[j])
	{
		if (match_env(sh.envp[j], *word, *qmask, i))
		{
			get_expansion(sh.envp[j], word, start_i - 1, qmask);
			found = 1;
		}
		j++;
	}
	if (!found)
		remove_word(word, qmask, start_i - 1);
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

static void	question_fill_qmask(char *qmask, int i, int len)
{
	while (len > 0)
	{
		len--;
		qmask[i + len] = '0';
	}
}

int	qmasq_diference(char *qmask)
{
	char	c;
	int		i;

	if (!qmask || !qmask[0])
		return (0);
	c = qmask[0];
	i = 1;
	while (qmask[i])
	{
		if (qmask[i] != c)
			return (0);
		i++;
	}
	return (1);
}

static void	pid_replace(char **argv, char **qmask, int i)
{
	char	*new_argv;
	char	*new_qmask;
	char	*pid;
	int		old_len;
	int		pid_len;

	pid = ft_strdup("0");
	if (!pid)
		return ;
	old_len = ft_strlen(*argv);
	if (getpid() > 0)
	{
		free(pid);
		pid_len = question_len(getpid());
		pid = malloc(pid_len + 1);
		if (!pid)
			return ;
		question_fill(pid, getpid(), pid_len);
		pid[pid_len] = '\0';
	}
	else
		pid_len = 1;
	new_argv = malloc(old_len - 2 + pid_len + 1);
	new_qmask = malloc(old_len - 2 + pid_len + 1);
	if (!new_argv || !new_qmask)
		return (free(pid), free(new_argv), free(new_qmask), (void)0);
	ft_memcpy(new_argv, *argv, i);
	ft_memcpy(new_qmask, *qmask, i);
	ft_memcpy(new_argv + i, pid, pid_len);
	question_fill_qmask(new_qmask, i, pid_len);
	ft_strcpy(new_argv + i + pid_len, *argv + i + 2);
	ft_strcpy(new_qmask + i + pid_len, *qmask + i + 2);
	free(pid);
	free(*argv);
	free(*qmask);
	*argv = new_argv;
	*qmask = new_qmask;
}

static void	remove_empty_unquoted_arg(char **argv, char **qmask, int i)
{
	free(argv[i]);
	free(qmask[i]);
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		qmask[i] = qmask[i + 1];
		i++;
	}
	argv[i] = NULL;
	qmask[i] = NULL;
}

static int	question_is_valid(char *qmask, int i)
{
	if (!qmask || !qmask[i])
		return (0);
	if (qmask[i] == '1')
		return (0);
	return (1);
}

static void	question_replace(char **argv, char **qmask, int i, t_shell sh)
{
	char	*new_argv;
	char	*new_qmask;
	int		old_len;
	int		num_len;

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
	question_fill_qmask(new_qmask, i, num_len);
	ft_strcpy(new_argv + i + old_len, *argv + i + 2);
	ft_strcpy(new_qmask + i + old_len, *qmask + i + 2);
	free(*argv);
	free(*qmask);
	*argv = new_argv;
	*qmask = new_qmask;
}

static void	question_case(char **argv, char **qmask, t_shell sh)
{
	int	i;

	i = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i] == '$' && (*argv)[i + 1] == '?'
			&& question_is_valid(*qmask, i))
		{
			question_replace(argv, qmask, i, sh);
			i += question_len(sh.exit_status);
		}
		else
			i++;
	}
}

static void	expand_argv(char **argv, char **qmask, t_shell sh)
{
	int	i;
	int	j;
	int	had_quote;
	
	i = 0;
	while (argv[i])
	{
		had_quote = has_quoted_char(qmask[i]);
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == '$'
				&& argv[i][j + 1] == '?' && qmask[i][j] != '1')
			{
				question_case(&argv[i], &qmask[i], sh);
				j = -1;
			}
			else if (argv[i][j] == '$'
				&& argv[i][j + 1] == '$' && qmask[i][j] != '1')
			{
				pid_replace(&argv[i], &qmask[i], j);
				j = -1;
			}
			else if (argv[i][j] == '$'
				&& qmask[i][j] != '1')
			{
				if (!argv[i][j + 1])
				{
					j++;
					continue ;
				}
				expand_word(&argv[i], &qmask[i], sh, j + 1);
				j = -1;
			}
			j++;
		}
		if (!argv[i][0] && !had_quote)
		{
			remove_empty_unquoted_arg(argv, qmask, i);
			continue ;
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
