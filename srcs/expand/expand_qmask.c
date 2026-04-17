/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_qmask.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:20:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 16:20:29 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*comprueba si hay caracteres entre comillas simples*/
int	has_quoted_char(char *qmask)
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

/*comprueba qe un caracter y el siguiente pertenezcan al mismo entrecomillado*/
int	same_qmask_context(char *qmask, int i)
{
	if (!qmask || !qmask[i] || !qmask[i + 1])
		return (0);
	return (qmask[i] == qmask[i + 1]);
}

/*comprueba que un caracter pueda formar parte de una variable*/
int	is_var_char(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	if (c >= '0' && c <= '9')
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

/*comprueba que una qmask tenga el mismo valor en toda su extension*/
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
