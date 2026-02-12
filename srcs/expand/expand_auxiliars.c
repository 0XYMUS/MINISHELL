/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_auxiliars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:28:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/12 16:30:19 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_end(char *word, int i)
{
	int	end;

	end = i + 1;
	while (word[end] && word[end] != ' ' && word[end] != '$')
		end++;
	return (end);
}

int  expand_len(char *word, char *env, int i)
{
	int	j;
	int	len;
	int	end;

	j = 0;
	len = i;
	end = get_end(word, i);
	while (env[j] && env[j] != '=')
		j++;
	while (env[j + 1])
	{
		j++;
		len++;
	}
	while (word[end])
	{
		end++;
		len++;
	}
	return (len);
}

void complete_expansion(char *env, char *word, int i, char **expansion)
{
	int	j;
	int	k;
	int	end;

	j = 0;
	while (word[j] && word[j] != '$')
	{
		(*expansion)[j] = word[j];
		j++;
	}
	k = 0;
	while (env[k] && env[k] != '=')
		k++;
	while (env[k + 1])
		(*expansion)[j++] = env[++k];
	end = get_end(word, i);
	while (word[end])
		(*expansion)[j++] = word[end++];
}

char	*update_qmask_after_expansion(char *qmask, int s, int len_q, int len)
{
	int		oldt;
	int		newt;
	int		i;
	char	*new;

	oldt = ft_strlen(qmask);
	newt = oldt - len_q + len;
	new = malloc(newt + 1);
	if (!new)
		return (NULL);
	i = -1;
	while (++i < s)
		new[i] = qmask[i];
	while (i < s + len)
		new[i++] = '0';
	while (i < newt)
	{
		new[i] = qmask[i - len + len_q];
		i++;
	}
	new[newt] = '\0';
	return (new);
}

int	expansion_len(char *word, int i)
{
	int	end;

	end = get_end(word, i);
	return (end - i);
}
