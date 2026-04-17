/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 16:12:32 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*hace el calculo de cuantos digitos tendra la expansion de $?*/
int	question_len(int n)
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

/*escribe el numero que significa $? en el nuevo argv*/
void	question_fill(char *new_argv, int n, int len)
{
	while (len > 0)
	{
		len--;
		new_argv[len] = (n % 10) + '0';
		n /= 10;
	}
}

/*rellena la nueva qmask con 0 donde esta la expansion de $?*/
void	question_fill_qmask(char *qmask, int i, int len)
{
	while (len > 0)
	{
		qmask[i] = '0';
		i++;
		len--;
	}
}

/*reemplaza la expansion $? por su numero correspondiente*/
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
	question_fill_qmask(new_qmask, i, num_len);
	old_len = num_len;
	ft_strcpy(new_argv + i + old_len, *argv + i + 2);
	ft_strcpy(new_qmask + i + old_len, *qmask + i + 2);
	free(*argv);
	free(*qmask);
	*argv = new_argv;
	*qmask = new_qmask;
}

/*aplica las expansiones de $? validas que encuentre en argv*/
void	question_case(char **argv, char **qmask, t_shell sh)
{
	int	i;

	i = 0;
	while ((*argv)[i])
	{
		if ((*argv)[i] == '$' && (*argv)[i + 1] == '?'
			&& (*qmask) && (*qmask)[i] && (*qmask)[i] != '1'
			&& same_qmask_context(*qmask, i))
		{
			question_replace(argv, qmask, i, sh);
			i += question_len(sh.exit_status);
		}
		else
			i++;
	}
}
