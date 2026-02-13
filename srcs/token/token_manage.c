/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_manage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 00:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/13 17:28:51 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * token_type_str():
 * Helper SOLO para debug. Convierte el enum del token a una string legible.
 * No se usa para lógica de parseo.
 */
static const char	*token_type_str(t_token_type t)
{
	if (t == TOK_WORD)
		return ("WORD");
	if (t == TOK_PIPE)
		return ("PIPE");
	if (t == TOK_REDIR_IN)
		return ("REDIR_IN");
	if (t == TOK_REDIR_OUT)
		return ("REDIR_OUT");
	if (t == TOK_APPEND)
		return ("APPEND");
	if (t == TOK_HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

/*
 * token_new():
 * Crea un nuevo token y lo deja aislado (next = NULL).
 * IMPORTANTE (ownership): este token "posee" el puntero 'value'.
 * Es decir: cuando liberes el token, se hará free(value).
 *
 * Recomendación: pásale un string ya duplicado (malloc/ft_strdup),
 * no un puntero a memoria estática o a un buffer que luego reutilices.
 */
t_token	*token_new(t_token_type type, char *value, char *qmask, int space)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (type != TOK_WORD)
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

/*
 * token_add_back():
 * Añade 'new_tok' al final de la lista enlazada de tokens.
 * - Si la lista está vacía (*lst == NULL), lo pone como primer nodo.
 * - No copia el token, solo enlaza punteros.
 */
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

/*
 * token_free_one():
 * Libera 1 token: primero libera tok->value y luego el token.
 * (Por eso es importante que value sea memoria dinámica).
 */
void	token_free_one(t_token *tok)
{
	if (!tok)
		return ;
	free(tok->value);
	free(tok->qmask);
	free(tok);
}

/*
 * token_free_all():
 * Libera una lista entera de tokens y deja *lst en NULL.
 */
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

/*
 * token_debug_print():
 * Imprime la lista de tokens en una forma fácil de leer, p.ej:
 *   [WORD:'echo'] -> [WORD:'hola'] -> [PIPE] -> [WORD:'wc'] -> [WORD:'-l']
 * Útil para validar el lexer antes de escribir el parser.
 */
void	token_debug_print(const t_token *lst)
{
	while (lst)
	{
		if (lst->type == TOK_WORD)
			printf("[%s:%s]", token_type_str(lst->type), lst->value);
		else
			printf("[%s]", token_type_str(lst->type));
		if (lst->next)
			printf(" -> ");
		lst = lst->next;
	}
	printf("\n");
}
