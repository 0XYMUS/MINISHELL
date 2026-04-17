/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_aux.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 13:40:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 13:41:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*busca comillas sin cerrar*/
char	find_unclosed_quote(const char *line, int i)
{
	char	q;

	q = 0;
	while (line[i])
	{
		if (line[i] == '\\' && q != '\'' && line[i + 1])
		{
			i += 2;
			continue ;
		}
		if (line[i] == '\'' || line[i] == '"')
		{
			if (q == 0)
				q = line[i];
			else if (q == line[i])
				q = 0;
		}
		i++;
	}
	return (q);
}

/*imprime error de sintaxis al detectar comillas abiertas*/
void	print_unclosed_quote(char q)
{
	if (!q)
		q = '\'';
	fprintf(stderr,
		"minishell: unexpected EOF while looking for matching `%c'\n", q);
	fprintf(stderr,
		"minishell: syntax error: unexpected end of file\n");
}

/*extrae una palabra y crea el token a la lista*/
int	token_add_word(char *line, int *i, t_token **lst)
{
	int		wordlen;
	char	*word;
	char	*qmask;
	t_token	*token;

	wordlen = word_len(line, *i);
	if (wordlen < 0)
	{
		if (errno == EINVAL)
			print_unclosed_quote(find_unclosed_quote(line, *i));
		return (-1);
	}
	qmask = NULL;
	word = word_dup(line, *i, wordlen, &qmask);
	if (!word)
		return (-1);
	token = token_new(TOK_WORD, word, qmask, is_space(line[*i + wordlen]));
	if (!token)
		return (free(word), free(qmask), -1);
	token_add_back(lst, token);
	*i += wordlen;
	return (0);
}

/*maca la utlima word como delimite si hay heredoc*/
void	token_mark_delimiter(t_token *lst)
{
	t_token	*prev;
	t_token	*last;

	if (!lst)
		return ;
	prev = NULL;
	last = lst;
	while (last->next)
	{
		prev = last;
		last = last->next;
	}
	if (last->type == TOK_WORD && prev && prev->type == TOK_HEREDOC)
		last->type = TOK_DELIMITER;
}
