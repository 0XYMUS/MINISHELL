/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/02/22 00:21:51 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*lst;
	t_pipeline	*pl;
	t_shell		sh;
	t_error		err;

	(void)argc;
	(void)argv;
	lst = NULL;

	if (shell_init(&sh, envp) != 0)
		return (1);
	while(1)
	{
		line = readline(
		"\001\x1b[1;97;106m\002 minishell "
		"\001\x1b[49;1;96m\002▓▒░ "
		"\001\x1b[0m\002"
	);

		if (line == NULL)
		{
			// Ctrl+D (EOF)
			printf("exit\n");
			break;
		}
		if (line[0] == '\0')
		{
			// Enter sin escribir nada
			free(line);
			continue;
		}
		add_history(line);
		lst = tokenizer(line);
		exec_from_tokens_tmp(lst, &sh);
		token_debug_print(lst); //(para probar tokenizer)
		pl = parse(&lst, &err);
		expand(pl, sh);
		if (!pl)
		{
			sh.exit_status = error_status(&err);
			error_print(&err);
		}
		else
		{
			pipeline_debug_print(pl); // para probar parseo
			pipeline_free_all(&pl);
		}
		token_free_all(&lst);
		free(line);
	}
	token_free_all(&lst);
	return (0);
}