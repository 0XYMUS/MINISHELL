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