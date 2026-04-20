/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_char_class.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 16:01:46 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 19:32:24 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v'
		|| c == '\f');
}

int	is_dquote_escapable(char c)
{
	return (c == '\\' || c == '"' || c == '$' || c == '`' || c == '\n');
}

int	is_key(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char	qmask_from_quote(char c)
{
	if (c == '\'')
		return ('1');
	if (c == '"')
		return ('2');
	return ('0');
}
