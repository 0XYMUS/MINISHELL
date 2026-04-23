/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:51:42 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static char	*pid_value(int *pid_len)
{
	*pid_len = 1;
	return (ft_strdup("0"));
}

/*reserva memoria para el nuevo argv y qmask tras la expansion*/
static int	alloc_pid_replace(char **new_argv, char **new_qmask,
		int old_len, int pid_len)
{
	*new_argv = malloc(old_len - 2 + pid_len + 1);
	*new_qmask = malloc(old_len - 2 + pid_len + 1);
	if (!*new_argv || !*new_qmask)
	{
		free(*new_argv);
		free(*new_qmask);
		return (0);
	}
	return (1);
}

/*reempleza $$ por el valor dl pid haciendo los cambios de memoria necesaios*/
static void	pid_replace(char **argv, char **qmask, int i)
{
	char	*new_argv;
	char	*new_qmask;
	char	*pid;
	int		old_len;
	int		pid_len;

	pid = pid_value(&pid_len);
	if (!pid)
		return ;
	old_len = ft_strlen(*argv);
	if (!alloc_pid_replace(&new_argv, &new_qmask, old_len, pid_len))
		return (free(pid), (void)0);
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

/*condicion cuando se encuentra $$*/
void	handle_pid_expansion(char **argv, char **qmask, int j)
{
	if ((*argv)[j] != '$' || (*argv)[j + 1] != '$')
		return ;
	pid_replace(argv, qmask, j);
}
