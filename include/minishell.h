/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:53:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/01/21 13:52:04 by julepere         ###   ########.fr       */
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

typedef enum e_builtin
{
    BI_NONE,
    BI_ECHO,
    BI_CD,
    BI_PWD,
    BI_EXPORT,
    BI_UNSET,
    BI_ENV,
    BI_EXIT
} t_builtin;

typedef struct s_command
{
	char		**argv;			/* final argv for execve / builtins */
	int			**space;
	t_redir		*redirs;		/* redirections in order */
	t_builtin	builtin;
}	t_command;

typedef struct s_pipeline
{
	t_command			cmd;
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
/*                                    UTILS                                   */
/* ************************************************************************** */
/* utils.c */
size_t	xy_strlen(const char *s);
/* token_utils.c */
int		is_space(char c);
char	*word_dup(char *line, int i, int wordlen);
int		word_len(char *line, int i);

/* ************************************************************************** */
/*                                  TOKENIZER                                 */
/* ************************************************************************** */
/* tokenizaion.c */
t_token	*tokenizer(char *line);

/* token_manage.c */
t_token	*token_new(t_token_type type, char *value, int space);
void	token_add_back(t_token **lst, t_token *new_tok);
void	token_free_one(t_token *tok);
void	token_free_all(t_token **lst);
void	token_debug_print(const t_token *lst);

/* ************************************************************************** */
/*                                   PARSER                                   */
/* ************************************************************************** */


/* ************************************************************************** */
/*                                 BUILT-INS                                  */
/* ************************************************************************** */
/* echo.c */
int		xy_echo(t_command *cmd, t_shell *sh);

/* ************************************************************************** */
/*                                   EXEC                                     */
/* ************************************************************************** */
/* exec.c */
int	token_to_command_tmp(t_token *toks, t_command *cmd);
int	cmd_type_and_exec_tmp(t_command *cmd, t_shell *sh);
int	exec_from_tokens_tmp(t_token *toks, t_shell *sh);
#endif