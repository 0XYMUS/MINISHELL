/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/10 11:41:20 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/10 13:57:28 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int  expand_len(char *word, char *qmask, int i)
{
    char    c;
    
    c = qmask[i];
    while (word[i] && qmask[i] == c)
        i++;
    return (i);
}

static char  *env_find_word(char *word, t_shell *sh, int i)
{
    int j;
    int k;
    int start_i;
    char    *expansion;

    j = 0;
    while (sh->envp[j])
    {
        k = 0;
        start_i = i;
        while (sh->envp[j][k] && word[i] && sh->envp[j][k] == word[i])
        {
            if (sh->envp[j][k] = '=' && !word[i])
            {
                expansion = get_expansion
            }
            k++;
            i++;
        }
        j++
    }
}

static void expand_word(char *word, char *qmask, t_shell *sh, int i)
{
    int word_len;

    i++;
    if (!word[i])
        return ;
    word_len = expand_len(word, qmask, i);
    if (word_len == i)
        return ;
    while (word[i] && )
}

static void expand_argv(char **argv, char **qmask, t_shell *sh)
{
    int i;
    int j;
    
    i = 0;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] == '$' && qmask[i][j] != '1')
                expand_word(argv[i][j], qmask[i][j], sh, j);
            j++;
        }
        i++;
    }
}

static void expand_redirs(t_redir *redirs, t_shell *sh)
{
    int i;
    
    i = 0;
    while (redirs && redirs->target[i])
    {
        if (redirs->target[i] == '$' && redirs->qmask[i] != '1')
            expand_word(redirs->target[i], redirs->qmask[i], sh, i);
        i++;
    }
}

void    expand(t_pipeline **node, t_shell *sh)
{
    while ((*node))
    {
        expand_argv((*node)->cmd->argv, (*node)->cmd->qmask, sh);
        expand_redirs((*node)->cmd->redirs, sh);
        (*node) = (*node)->next;
    }
}