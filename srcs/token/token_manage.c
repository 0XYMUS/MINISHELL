/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/23 16:23:03 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "minishell.h"

/*Crea un nuevo token*/
t_token	*token_new(t_token_type type, char *value, char *qmask, int space)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (type != TOK_WORD && type != TOK_DELIMITER)
	{
		token->value = NULL;
		token->qmask = NULL;
	}
	else
	{
		token->value = value;
		token->qmask = qmask;
	}
	if (space == 1)
		token->space = 1;
	else
		token->space = 0;
	token->next = NULL;
	return (token);
}

/* Añade un token al final de la lista*/
void	token_add_back(t_token **lst, t_token *new_tok)
{
	t_token	*cur;

	if (!lst || !new_tok)
		return ;
	if (!*lst)
	{
		*lst = new_tok;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_tok;
}

/*Libera 1 token*/
void	token_free_one(t_token *tok)
{
	if (!tok)
		return ;
	free(tok->value);
	free(tok->qmask);
	free(tok);
}

/* Libera la lista de tokens*/
void	token_free_all(t_token **lst)
{
	t_token	*cur;
	t_token	*next;

	if (!lst)
		return ;
	cur = *lst;
	while (cur)
	{
		next = cur->next;
		token_free_one(cur);
		cur = next;
	}
	*lst = NULL;
}
