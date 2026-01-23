/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 16:09:49 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/23 11:21:15 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    first_word(char *word, t_token_type type)
{
    if (type == TOK_PIPE)
        return (-1);
    if (type == TOK_REDIR_IN || type == TOK_REDIR_OUT || type == TOK_APPEND
    || type == TOK_HEREDOC)
        
}

void    parse(t_token *lst, t_pipeline *lst2)
{
    while(lst)
    {
        first_word(lst->value);
    }
}