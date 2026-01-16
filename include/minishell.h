/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:53:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/16 11:34:23 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// Estructura temporal de tokenizacion
typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC
} 	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
} 	t_token;

// Estructura final
typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char		**argv;    // argumentos finales del comando
	t_redir		*redirs;  // TODAS las redirecciones en orden
	int			is_builtin;
}	t_command;

typedef struct s_pipeline
{
	t_command			*cmd;
	struct s_pipeline	*next;
}	t_pipeline;

//funciones
int		is_space(char c);
int		str_len_quote(char *str, int i, char q);
int		str_len_space(char *str, int i, char q);
char	*word_dup(char *line, int i, int wordlen, char q);
t_token	*tokenizer(char *line);

//funciones para listas
t_token	*token_new(t_token_type type, char *value);
void	token_add_back(t_token **lst, t_token *new_tok);
void	token_free_one(t_token *tok);
void	token_free_all(t_token **lst);
void	token_debug_print(const t_token *lst);