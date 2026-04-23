/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qmask.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:10:22 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:32:59 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

/*hace una copia de la cadena qmask*/
char	*qmask_dup(const char *src)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!src)
		return (NULL);
	len = ft_strlen(src);
	dup = malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/*determina si qmask tiene comillas*/
int	qmask_has_quote(const char *qmask)
{
	if (!qmask)
		return (0);
	while (*qmask)
	{
		if (*qmask != '0')
			return (1);
		qmask++;
	}
	return (0);
}
