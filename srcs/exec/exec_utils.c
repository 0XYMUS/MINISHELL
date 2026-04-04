/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 00:00:00 by julepere          #+#    #+#             */
/*   Updated: 2026/04/02 15:24:12 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_from_name(const char *name, t_builtin_cmd *builtin)
{
	if (xy_streq(name, "echo"))
		*builtin = BI_ECHO;
	else if (xy_streq(name, "cd"))
		*builtin = BI_CD;
	else if (xy_streq(name, "pwd"))
		*builtin = BI_PWD;
	else if (xy_streq(name, "export"))
		*builtin = BI_EXPORT;
	else if (xy_streq(name, "unset"))
		*builtin = BI_UNSET;
	else if (xy_streq(name, "env"))
		*builtin = BI_ENV;
	else if (xy_streq(name, "exit"))
		*builtin = BI_EXIT;
	else
		return (0);
	return (1);
}

void	resolve_command_type(t_command *pl)
{
	while (pl)
	{
		pl->type = CMD_EXTERNAL;
		pl->builtin = BI_NONE;
		if (!pl->argv || !pl->argv[0])
			pl->type = CMD_UNKNOWN;
		else if (builtin_from_name(pl->argv[0], &pl->builtin))
			pl->type = CMD_BUILTIN;
		pl = pl->next;
	}
}

static char	*substr_dup(const char *s, int start, int len)
{
	char	*dup;
	int		i;

	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[start + i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

static char	*join_path(const char *dir, const char *cmd)
{
	int		dir_len;
	int		cmd_len;
	char	*path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	path = malloc(dir_len + cmd_len + 2);
	if (!path)
		return (NULL);
	ft_memcpy(path, dir, dir_len);
	if (dir_len > 0)
		path[dir_len++] = '/';
	ft_memcpy(path + dir_len, cmd, cmd_len);
	path[dir_len + cmd_len] = '\0';
	return (path);
}

char	*find_exec_path(char **envp, const char *cmd)
{
	int		i;
	int		start;
	int		end;
	char	*dir;
	char	*path;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			start = 5;
			while (envp[i][start])
			{
				end = start;
				while (envp[i][end] && envp[i][end] != ':')
					end++;
				dir = substr_dup(envp[i], start, end - start);
				if (!dir)
					return (NULL);
				path = join_path(dir, cmd);
				free(dir);
				if (path && access(path, X_OK) == 0)
					return (path);
				free(path);
				if (envp[i][end] == ':')
					start = end + 1;
				else
					break ;
			}
		}
		i++;
	}
	return (NULL);
}
