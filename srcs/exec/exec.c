/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:24:47 by julepere          #+#    #+#             */
/*   Updated: 2026/01/21 14:00:34 by julepere         ###   ########.fr       */
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
	cmd->builtin = BI_NONE;
	return (0);
}

/* -------- 2) detector + executor -------- */

int	cmd_type_and_exec_tmp(t_command *cmd, t_shell *sh)
{
	(void)sh;
	/* “sin comprobar nada realmente” => asumimos argv[0] existe */
	if (cmd->argv && cmd->argv[0] && xy_streq(cmd->argv[0], "echo"))
	{
		cmd->builtin = BI_ECHO;
		xy_echo(cmd, sh);
		return (1);
	}
	cmd->builtin = BI_NONE;
	return (0);
}

/* -------- 3) wrapper: de tokens a ejecución -------- */

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