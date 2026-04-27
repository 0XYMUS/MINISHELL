/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:31:36 by julepere          #+#    #+#             */
/*   Updated: 2026/04/27 10:58:32 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <readline/history.h>

static void	exit_cleanup(t_cmd *cmd, t_shell *sh)
{
	(void)cmd;
	if (!sh)
		return ;
	if (sh->tokens)
		token_free_all(&sh->tokens);
	if (sh->pipeline)
		pipeline_free_all(&sh->pipeline);
	env_free(sh->envp);
	clear_history();
}

static int	parse_digits_mod256(const char *arg, int i, int *n)
{
	*n = 0;
	if (!is_number(arg[i]))
		return (0);
	while (arg[i])
	{
		if (!is_number(arg[i]))
			return (0);
		*n = (*n * 10 + (arg[i] - '0')) % 256;
		i++;
	}
	return (1);
}

static int	parse_arg(const char *arg, int *status)
{
	int	i;
	int	sign;
	int	n;

	i = 0;
	if (!arg || !arg[0])
		return (0);
	sign = 1;
	if (arg[i] == '+' || arg[i] == '-')
	{
		if (arg[i] == '-')
			sign = -1;
		i++;
	}
	if (!parse_digits_mod256(arg, i, &n))
		return (0);
	if (sign == -1)
		n = (-n) % 256;
	*status = (unsigned char)n;
	return (1);
}

static int	handle_exit_arg(t_cmd *cmd, t_shell *sh, int *status)
{
	if (!cmd->argv[1])
		return (1);
	if (!parse_arg(cmd->argv[1], status))
	{
		write(2, "minishell: exit: ", 17);
		write_str_fd(cmd->argv[1], 2);
		write(2, ": numeric argument required\n", 28);
		sh->exit_status = 2;
		exit_cleanup(cmd, sh);
		exit(2);
	}
	return (1);
}

int	xy_exit(t_cmd *cmd, t_shell *sh)
{
	int	status;

	if (!sh)
		return (0);
	write(1, "exit\n", 5);
	if (!cmd || !cmd->argv)
		return (1);
	status = sh->exit_status;
	handle_exit_arg(cmd, sh, &status);
	if (cmd->argv[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		sh->exit_status = 1;
		return (1);
	}
	sh->exit_status = status;
	exit_cleanup(cmd, sh);
	exit(sh->exit_status);
}
