/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/01/21 13:58:36 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_token	*lst;
	t_shell	sh;

	
	(void)argc;
	(void)argv;
	(void)envp;
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
		token_debug_print(lst);
		/*test         PRUEBA DE COMANDOS		 test*/
		/*test*/exec_from_tokens_tmp(lst, &sh);
		/*test*/token_free_all(&lst);
	
		free(line);
	}
	token_free_all(&lst);
	return (0);
}