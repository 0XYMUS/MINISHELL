/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_auxiliars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 16:28:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:53:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*encuentra donde termina la variable que empieza en i,
teniendo en cuenta caracteres y qmask*/
int	get_end(char *word, char *qmask, int i)
{
	int		end;
	char	ctx;

	end = i + 1;
	ctx = qmask[i];
	while (word[end] && is_var_char(word[end]) && qmask[end] == ctx)
		end++;
	return (end);
}

/*calcula la nueva longitud e word tras expansiom*/
int	expand_len(char *word, char *env, char *qmask, int i)
{
	int	j;
	int	len;
	int	end;

	j = 0;
	len = i;
	end = get_end(word, qmask, i);
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

/*actualiza tamno de qmask tras expansion*/
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

/*devuelve cuanto ocupa la expansion*/
int	expansion_len(char *word, char *qmask, int i)
{
	int	end;

	end = get_end(word, qmask, i);
	return (end - i);
}
