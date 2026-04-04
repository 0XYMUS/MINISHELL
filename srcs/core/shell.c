/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 02:02:59 by julepere          #+#    #+#             */
/*   Updated: 2026/04/04 19:14:33 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(char *line, t_shell *sh)
{
	t_command	*pl;

	sh->tokens = tokenizer(line);
	/* token_debug_print(sh->tokens); */
	pl = parse(&sh->tokens, &sh->err);
	if (!pl)
	{
		sh->exit_status = error_status(&sh->err);
		error_print(&sh->err);
	}
	else
	{
		expand(pl, *sh);
		sh->exit_status = execution(pl, sh);
		/* pipeline_debug_print(pl); */
		pipeline_free_all(&pl);
	}
	token_free_all(&sh->tokens);
	sh->tokens = NULL;
}

void	shell_loop(t_shell *sh)
{
	char	*line;

	while (1)
	{
		line = readline(
			"\001\x1b[1;97;106m\002 minishell "
			"\001\x1b[49;1;96m\002▓▒░ "
			"\001\x1b[0m\002"
		);
		if (line == NULL)
		{
			printf("exit\n");
			break;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			process_line(line, sh);
		}
		free(line);
	}
}