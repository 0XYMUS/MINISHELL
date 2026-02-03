/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/02/03 16:45:47 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*lst;
	t_pipeline	*pl;
	t_shell	sh;
	t_parse_error	err;

	
	(void)argc;
	(void)argv;
	(void)envp;
	(void)sh;
	lst = NULL;
	sh.exit_status = 0;
	while(1)
	{
		line = readline("minishell$ ");
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
		// token_debug_print(lst); //(para probar tokenizer)
		pl = parse(&lst, &err);
		if (!pl)
		{
			sh.exit_status = parse_error_status(&err);
			parse_error_print(&err);
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