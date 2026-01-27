/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:59:05 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/26 13:02:22 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_redir(t_token_type type)
{
    return (type == TOK_APPEND || type == TOK_HEREDOC || type == TOK_REDIR_IN
         || type == TOK_REDIR_OUT);
}