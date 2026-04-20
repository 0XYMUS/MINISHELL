/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_build.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 11:40:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 19:32:24 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fill_word_qmask(char *line, int *idxs, char **dst, int end);

/*calcula la longitud del proximo token, da error si hay comillas abiertas*/
int	word_len(char *line, int i)
{
	int		i_initial;
	char	q;

	i_initial = i;
	q = 0;
	while (line[i])
	{
		if (!q && (is_space(line[i]) || is_key(line[i])))
			break ;
		if (word_len_step(line, &i, &q))
			continue ;
		i++;
	}
	if (q != 0)
	{
		errno = EINVAL;
		return (-1);
	}
	return (i - i_initial);
}

/*reserva memoria para word y qmask*/
static int	alloc_word_qmask(int wordlen, char **word, char **qmask)
{
	*word = malloc(sizeof(char) * wordlen + 1);
	if (!*word)
		return (0);
	*qmask = malloc(sizeof(char) * wordlen + 1);
	if (!*qmask)
	{
		free(*word);
		return (0);
	}
	return (1);
}

/*duplica la palabra parseada y crea su qmask*/
char	*word_dup(char *line, int i, int wordlen, char **qmask_out)
{
	char	*word;
	char	*qmask;
	int		idxs[2];
	char	*dst[2];
	int		end;

	end = i + wordlen;
	if (!alloc_word_qmask(wordlen, &word, &qmask))
		return (NULL);
	idxs[0] = i;
	idxs[1] = 0;
	dst[0] = word;
	dst[1] = qmask;
	fill_word_qmask(line, idxs, dst, end);
	word[idxs[1]] = '\0';
	qmask[idxs[1]] = '\0';
	if (qmask_out)
		*qmask_out = qmask;
	else
		free(qmask);
	return (word);
}
