/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:24:47 by julepere          #+#    #+#             */
/*   Updated: 2026/02/04 12:03:32 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

/*
TEMPORAL Y A PROPÓSITO "CUTRE":
- Convierte tokens -> cmd->argv cogiendo SOLO TOK_WORD, sin redirs/pipes.
- Detecta builtin por string SOLO para "echo".
- Ejecuta echo.
- Sin comprobaciones “serias” más allá de evitar segfaults básicos.
*/

/* -------- mini helpers (porque no tienes ft_strdup/ft_strcmp) -------- */

static int	xy_streq(const char *a, const char *b)
{
	int	i;

	if (!a || !b)
		return (0);
	i = 0;
	while (a[i] && b[i] && a[i] == b[i])
		i++;
	return (a[i] == '\0' && b[i] == '\0');
}

static char	*xy_strdup(const char *s)
{
	size_t	i;
	size_t	len;
	char	*dup;

	if (!s)
		return (NULL);
	len = xy_strlen(s);
	dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static void	xy_free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

/* -------- 1) converter: tokens -> command -------- */

int	token_to_command_tmp(t_token *toks, t_command *cmd)
{
	int		i;
	int		words;
	t_token	*t;

	/* count TOK_WORD (sí, hay que contar para malloc) */
	words = 0;
	t = toks;
	while (t)
	{
		if (t->type == TOK_WORD)
			words++;
		t = t->next;
	}

	cmd->argv = (char **)malloc(sizeof(char *) * (words + 1));
	if (!cmd->argv)
		return (-1);

	i = 0;
	while (toks)
	{
		if (toks->type == TOK_WORD)
			cmd->argv[i++] = xy_strdup(toks->value);
		toks = toks->next;
	}
	cmd->argv[i] = NULL;

	cmd->space = NULL;
	cmd->redirs = NULL;
	cmd->cmd_info.type = CMD_UNKNOWN;
	cmd->cmd_info.builtin = BI_NONE;
	return (0);
}

/* -------- 2) identificar tipo de comando -------- */

static void	identify_command_type(t_command *cmd)
{
    if (!cmd->argv || !cmd->argv[0])
    {
        cmd->cmd_info.type = CMD_UNKNOWN;
        cmd->cmd_info.builtin = BI_NONE;
        return ;
    }
    if (xy_streq(cmd->argv[0], "echo"))
        cmd->cmd_info.builtin = BI_ECHO;
    else if (xy_streq(cmd->argv[0], "cd"))
        cmd->cmd_info.builtin = BI_CD;
    else if (xy_streq(cmd->argv[0], "pwd"))
        cmd->cmd_info.builtin = BI_PWD;
    else if (xy_streq(cmd->argv[0], "export"))
        cmd->cmd_info.builtin = BI_EXPORT;
    else if (xy_streq(cmd->argv[0], "unset"))
        cmd->cmd_info.builtin = BI_UNSET;
    else if (xy_streq(cmd->argv[0], "env"))
        cmd->cmd_info.builtin = BI_ENV;
    else if (xy_streq(cmd->argv[0], "exit"))
        cmd->cmd_info.builtin = BI_EXIT;
    else
        cmd->cmd_info.builtin = BI_NONE;
    if (cmd->cmd_info.builtin != BI_NONE)
        cmd->cmd_info.type = CMD_BUILTIN;
    else
        cmd->cmd_info.type = CMD_EXTERNAL;
}

/* -------- 3) dispatcher de builtins -------- */

static int	exec_builtin(t_command *cmd, t_shell *sh)
{
    if (cmd->cmd_info.builtin == BI_ECHO)
        return (xy_echo(cmd, sh));
    if (cmd->cmd_info.builtin == BI_PWD)
        return (xy_pwd(cmd, sh));
    if (cmd->cmd_info.builtin == BI_ENV)
        return (xy_env(cmd, sh));
    /* TODO: resto de builtins */
    return (1);
}

/* -------- 4) ejecutor principal -------- */

int	cmd_type_and_exec_tmp(t_command *cmd, t_shell *sh)
{
    identify_command_type(cmd);
    if (cmd->cmd_info.type == CMD_BUILTIN)
        return (exec_builtin(cmd, sh));
    /* CMD_EXTERNAL: por ahora no hace nada */
    return (0);
}

/* -------- 5) wrapper: de tokens a ejecución -------- */

int	exec_from_tokens_tmp(t_token *toks, t_shell *sh)
{
    t_command	cmd;
    int			ret;

    if (token_to_command_tmp(toks, &cmd) == -1)
        return (-1);
    ret = cmd_type_and_exec_tmp(&cmd, sh);
    xy_free_argv(cmd.argv);
    return (ret);
}