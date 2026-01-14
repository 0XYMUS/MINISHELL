/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 15:58:11 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/09 13:25:33 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	tokenizer(char *line)
{
	int	i;
	
	i = 0;
	while (line[i])
	{
		while (!is_space(line[i]))
			
		i++;
	}
}