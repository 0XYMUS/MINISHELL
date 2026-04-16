 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 16:53:12 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/03/02 17:03:34 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   LIBS                                     */
/* ══════════════════════════════════════════════════════════════════════════ */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <sys/wait.h>
# include <errno.h>
#include <sys/stat.h>

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   ENUMS                                    */
/* ══════════════════════════════════════════════════════════════════════════ */

typedef enum e_token_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_APPEND,
	TOK_HEREDOC,
	TOK_DELIMITER
}	t_token_type;

typedef enum e_redir_type
{
	R_IN,
	R_OUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

typedef enum e_builtin_cmd
{
	BI_NONE,
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT
} t_builtin_cmd;

typedef enum e_cmd_type
{
	CMD_BUILTIN,
	CMD_EXTERNAL,
	CMD_UNKNOWN
}	t_cmd_type;

typedef enum e_errcode
{
	PERR_NONE,
	PERR_PIPE_START,
	PERR_PIPE_END,
	PERR_PIPE_DOUBLE,
	PERR_REDIR_NO_TARGET,
	PERR_UNEXPECTED_TOKEN,
	PERR_NOT_FOUND,
	PERR_PERMISSION_DENIED,
	PERR_IS_DIRECTORY,
	PERR_EXEC_FORMAT,
	PERR_OOM
}	t_errcode;

typedef enum e_near
{
	PNEAR_NONE,
	PNEAR_NEWLINE,
	PNEAR_PIPE,
	PNEAR_REDIR_IN,
	PNEAR_REDIR_OUT,
	PNEAR_APPEND,
	PNEAR_HEREDOC,
	PNEAR_WORD
}	t_near;

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   STRUCTS                                  */
/* ══════════════════════════════════════════════════════════════════════════ */

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	char			*qmask;
	int				space;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	char				*qmask;
	int					expand;
	struct s_redir		*next;
}	t_redir;

typedef struct s_command
{
	char				**argv;  // User input
	char				**qmask; // Comillas
	t_redir				*redirs; 
	t_cmd_type			type; 	 //command types (ENUM)
	t_builtin_cmd		builtin; // Builtin type
	struct s_command	*next;   //next command in line
}	t_command;

typedef struct s_error
{
	t_errcode			code;
	t_near				near;
	const char			*subject;
}t_error;

/*-------------------------------- [  shell  ] -------------------------------*/

typedef struct s_shell
{
	char		*line;			/* current readline() buffer */
	char		**envp;			/* environment (temporary as char**) */
	int			exit_status;	/* $? */
	t_token		*tokens;		/* lexer output for current line */
	t_command	*pipeline;		/* parser output for current line */
	t_error		err;
}	t_shell;

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                  FUNCTIONS                                 */
/* ══════════════════════════════════════════════════════════════════════════ */

/*-------------------------------- [  core  ] --------------------------------*/
/* env_manage.c */
int		env_count(char **envp);
void	env_free(char **envp);
char	**env_dup(char **envp);

/* initialize.c */
int		shell_init(t_shell *sh, char **envp);

/* shell.c */
void	shell_loop(t_shell *sh);

/*-------------------------------- [  utils  ] -------------------------------*/

/* utils.c */
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);

/* utils_1.c */
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	ft_strcpy(char *dest, const char *src);
void	str_move(char **str, int i, int move);
int		ft_strncmp(const char *s1, const char *s2, size_t size);

/* parse_utils.c */
int			argv_len(t_token *token);
int			xy_streq(const char *a, const char *b);
void		set_command_type(t_command *node);
/* void		is_builtin(char *argv, t_command **node); */
/* int			is_external(char *argv, t_command **node, t_error *err); */


/* token_utils.c */
int		is_space(char c);
int		is_redir(t_token_type type);
char	*word_dup(char *line, int i, int wordlen, char **qmask_out);
int		word_len(char *line, int i);

/* expand_utils.c */
size_t	ft_strlen(const char *s);

/* export_utils.c */
int		find_env_var(char **envp, const char *name);
int		export_set_var(char *arg, t_shell *sh);
int		export_with_value(char *arg, char *eq_pos, t_shell *sh);
int		update_env_var(t_shell *sh, int idx, char *arg);
int		add_env_var(t_shell *sh, char *new_var);

/* unset_utils.c*/
int	remove_env_var(t_shell *sh, char *new_var);

/* exec_utils.c */
void	resolve_command_type(t_command *pl);
char	*find_exec_path(char **envp, const char *cmd);

/*------------------------------ [  tokenizer  ] -----------------------------*/

/* tokenizaion.c */
t_token	*tokenizer(char *line);

/* token_manage.c */
t_token	*token_new(t_token_type type, char *value, char *qmask, int space);
void	token_add_back(t_token **lst, t_token *new_tok);
void	token_free_one(t_token *tok);
void	token_free_all(t_token **lst);
void	token_debug_print(const t_token *lst);

/*-------------------------------- [  parser  ] ------------------------------*/

/* parse.c */
t_command	*parse(t_token **token, t_error *err);
int			parse_simple_command(t_command **lst, t_token **token, t_error *err);

/* pipeline_utils.c */
t_command	*pipeline_new(void);
void		pipeline_add_back(t_command **lst, t_command *new_node);
void		pipeline_debug_print(const t_command *lst);
t_redir		*redir_new(t_token_type type, char *target, char *qmask);
void		redir_add_back(t_redir **lst, t_redir *new_node);

/* free_pipeline.c */
void		pipeline_free_all(t_command **lst);

/* validate_syntax.c */
t_near		near_from_token(t_token_type token);
int			validate_syntax(t_token *t, t_error *err);

/* parse_error.c */
void		error_init(t_error *err);
void		error_set(t_error *err, t_errcode code, t_near near);
void	error_set_subject(t_error *err, t_errcode code, t_near near,
		const char *subject);
void		error_print(const t_error *err);
int			error_status(const t_error *err);
int			error_emit(t_error *err, t_errcode code, t_near near);
int		error_emit_subject(t_error *err, t_errcode code, t_near near,
		const char *subject);
int			error_fail(t_error *err, t_errcode code, t_near near);
int		error_fail_subject(t_error *err, t_errcode code, t_near near,
		const char *subject);

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   EXPAND                                   */
/* ══════════════════════════════════════════════════════════════════════════ */

/* expand.c */
void    expand(t_command *node, t_shell sh);

/* expand_auxiliars.c*/
int		get_end(char *word, char *qmask, int i);
int		expand_len(char *word, char *env, char *qmask, int i);
char	*update_qmask_after_expansion(char *qmask, int s, int len_q, int len);
int		expansion_len(char *word, char *qmask, int i);

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   ERRORS                                   */
/* ══════════════════════════════════════════════════════════════════════════ */

/* errors.c */

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                 BUILT-INS                                  */
/* ══════════════════════════════════════════════════════════════════════════ */

/* cd.c */
int		xy_cd(t_command *cmd, t_shell *sh);

/* echo.c */
int		xy_echo(t_command *cmd, t_shell *sh);

/* env.c */
int		xy_env(t_command *cmd, t_shell *sh);

/* exit.c */
int		xy_exit(t_command *cmd, t_shell *sh);

/* export.c */
int		xy_export(t_command *cmd, t_shell *sh);

/* pwd.c */
int		xy_pwd(t_command *cmd, t_shell *sh);

/* unset.c */
int		xy_unset(t_command *cmd, t_shell *sh);

/* ══════════════════════════════════════════════════════════════════════════ */
/*                                   EXEC                                     */
/* ══════════════════════════════════════════════════════════════════════════ */

/* exec.c */
int		execution(t_command *pl, t_shell *sh);
int		run_parent_builtin(t_command *pl, t_shell *sh);
int		is_parent_builtin(t_command *pl);
void	parent_process(int *prev_read, t_command *pl, int *pipefd);
int		wait_all_children(void);

/* child_process.c */
int	apply_redirs(t_redir *redirs);
int		exec_choice(t_command *pl, t_shell *sh);
int		execute_external(t_command *pl, t_shell *sh);
void	child_process(int prev_read, t_command *pl, int *pipefd, t_shell *sh);


#endif