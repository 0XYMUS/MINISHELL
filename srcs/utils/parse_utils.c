/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/03 12:17:14 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redir(t_token_type type)
{
    return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
         || type == TOK_REDIR_OUT);
}


char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*dup;

	if (!s)
		return (NULL);
	len = xy_strlen(s);
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	argv_len(t_token *token)
{
	int	n_argv;

	n_argv = 0;
	while (token && token->type != TOK_PIPE)
	{
		if (is_redir(token->type))
		{
			if (!token->next || token->next->type != TOK_WORD)
				return (-1);
			token = token->next->next;
		}
		else if (token->type == TOK_WORD)
		{
			token = token->next;
			n_argv++;
		}
		else
			token = token->next;
	}
	return (n_argv);
}
