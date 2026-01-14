/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/01/14 14:02:25 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;

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
		printf("Input recibido: %s\n", line);
		free(line);
	}
	return (0);
}