/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:20:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 16:38:37 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*borra el crarcter $ en  word y qmask*/
void	remove_dollar_only(char **word, char **qmask, int i)
{
	int	len;

	len = ft_strlen(*word);
	ft_memmove(*word + i, *word + i + 1, len - i);
	ft_memmove(*qmask + i, *qmask + i + 1, len - i);
}

/*elimina una palabra concreta de wor y qmask*/
void	remove_word(char **word, char **qmask, int start)
{
	int		end;
	int		len;
	char	ctx;

	end = start + 1;
	ctx = (*qmask)[end];
	while ((*word)[end] && is_var_char((*word)[end]) && (*qmask)[end] == ctx)
		end++;
	end--;
	len = ft_strlen(*word);
	ft_memmove(*word + start, *word + end + 1, len - end);
	ft_memmove(*qmask + start, *qmask + end + 1, len - end);
}

/*borra un argumento que se queda vacio tras expandir*/
void	remove_empty_unquoted_arg(char **argv, char **qmask, int i)
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
