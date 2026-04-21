/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 02:02:59 by julepere          #+#    #+#             */
/*   Updated: 2026/04/21 17:50:33 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(char *line, t_shell *sh)
{
	t_cmd	*pl;
	t_token	*tok_cursor;

	sh->tokens = tokenizer(line);
	tok_cursor = sh->tokens;
	pl = parse(&tok_cursor, &sh->err);
	if (!pl)
	{
		sh->exit_status = error_status(&sh->err);
		error_print(&sh->err);
	}
	else
	{
		expand(pl, *sh);
		sh->exit_status = execution(pl, sh);
		pipeline_free_all(&pl);
	}
	token_free_all(&sh->tokens);
	sh->tokens = NULL;
}

void	shell_loop(t_shell *sh)
{
	char	*line;

	g_signal = 0;
	while (1)
	{
		catch_signal_father();
		line = readline(
				"\001\x1b[1;97;106m\002 minishell "
				"\001\x1b[49;1;96m\002▓▒░ "
				"\001\x1b[0m\002");
		if (line == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			process_line(line, sh);
		}
		if (g_signal == SIGINT)
		{
			sh->exit_status = 130;
			g_signal = 0;
		}
		free(line);
	}
}
