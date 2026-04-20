/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_escape_quote.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:02:33 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 19:32:24 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*anade un caracter en word y su marca en qmask segun el contexto*/
void	append_with_qmask(char **dst, int *j, char q, char c)
{
	dst[0][*j] = c;
	dst[1][*j] = qmask_from_quote(q);
	(*j)++;
}

/*marca $ como protegido para que no se expanda al venir tras \*/
static void	append_protected_char(char **dst, int *j, char q, char c)
{
	dst[0][*j] = c;
	if (c == '$')
		dst[1][*j] = '1';
	else
		dst[1][*j] = qmask_from_quote(q);
	(*j)++;
}

/*procesa \ dependiendo del cararcter que le siga y contexto de comillas*/
int	handle_backslash(char *line, int *idxs, char q, char **dst)
{
	if (line[idxs[0]] != '\\' || q == '\'' || idxs[0] + 1 >= idxs[1])
		return (0);
	if (q == 0)
	{
		if (line[idxs[0] + 1] == '$')
			append_protected_char(dst, &idxs[2], q, line[idxs[0] + 1]);
		else
			append_with_qmask(dst, &idxs[2], q, line[idxs[0] + 1]);
		idxs[0] += 2;
		return (1);
	}
	if (q == '"' && is_dquote_escapable(line[idxs[0] + 1]))
	{
		if (line[idxs[0] + 1] != '\n')
			append_protected_char(dst, &idxs[2], q, line[idxs[0] + 1]);
		idxs[0] += 2;
		return (1);
	}
	append_with_qmask(dst, &idxs[2], q, line[idxs[0]]);
	idxs[0]++;
	return (1);
}

/*abre y cierra el contexto de las comillas cuando toca*/
int	handle_quote(char *line, int *i, char *q)
{
	if (line[*i] != '\'' && line[*i] != '"')
		return (0);
	if (*q == 0)
	{
		*q = line[*i];
		(*i)++;
	}
	else if (*q == line[*i])
	{
		*q = 0;
		(*i)++;
	}
	else
		return (0);
	return (1);
}

/*avanza el indce teneiendo en cuenta las reglas de las comillas y \\*/
int	word_len_step(char *line, int *i, char *q)
{
	if (line[*i] == '\\' && *q != '\'' && line[*i + 1])
	{
		if (*q == 0)
			*i += 2;
		else if (*q == '"' && is_dquote_escapable(line[*i + 1]))
			*i += 2;
		else
			(*i)++;
		return (1);
	}
	if (is_quote(line[*i]))
	{
		if (*q == 0)
			*q = line[*i];
		else if (*q == line[*i])
			*q = 0;
		(*i)++;
		return (1);
	}
	return (0);
}
