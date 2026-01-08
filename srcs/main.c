/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:21:05 by julepere          #+#    #+#             */
/*   Updated: 2026/01/08 17:10:11 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv)
{
	char	*line;

	(void)argc;
	(void)argv;

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