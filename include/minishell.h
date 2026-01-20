/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:53:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/20 13:39:04 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ************************************************************************** */
/*                                   LEXER                                    */
/* ************************************************************************** */

/* Token types produced by the tokenizer */
typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				space;
	struct s_token	*next;
}	t_token;

/* ************************************************************************** */
/*                                   PARSER                                   */
/* ************************************************************************** */

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	struct s_redir		*next;
}	t_redir;

typedef struct s_command
{
	char		**argv;		/* final argv for execve / builtins */
	int			**space;
	t_redir		*redirs;	/* redirections in order */
	int			is_builtin;
}	t_command;

typedef struct s_pipeline
{
	t_command			*cmd;
	struct s_pipeline	*next;
}	t_pipeline;

/* ************************************************************************** */
/*                                    SHELL                                   */
/* ************************************************************************** */

typedef struct s_shell
{
	char		*line;			/* current readline() buffer */
	char		**envp;			/* environment (temporary as char**) */
	int			exit_status;	/* $? */
	t_token		*tokens;		/* lexer output for current line */
	t_pipeline	*pipeline;		/* parser output for current line */
}	t_shell;

/* ************************************************************************** */
/*                                  TOKENIZER                                 */
/* ************************************************************************** */

int		is_space(char c);
int		word_len_quote(char *line, int i);
char	*word_dup(char *line, int i, int wordlen);
t_token	*tokenizer(char *line);
int		word_len(char *line, int i);

/* ************************************************************************** */
/*                                 TOKEN LIST                                 */
/* ************************************************************************** */

t_token	*token_new(t_token_type type, char *value, int space);
void	token_add_back(t_token **lst, t_token *new_tok);
void	token_free_one(t_token *tok);
void	token_free_all(t_token **lst);
void	token_debug_print(const t_token *lst);

#endif