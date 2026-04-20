/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 00:00:00 by julepere          #+#    #+#             */
/*   Updated: 2026/04/20 17:20:26 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*clasifica cada comando como builtin, external o desconocido*/
void	resolve_command_type(t_cmd *pl)
{
	while (pl)
	{
		pl->type = CMD_EXTERNAL;
		pl->builtin = BI_NONE;
		if (!pl->argv || !pl->argv[0])
			pl->type = CMD_UNKNOWN;
		else if (xy_streq(pl->argv[0], "echo"))
			pl->builtin = BI_ECHO;
		else if (xy_streq(pl->argv[0], "cd"))
			pl->builtin = BI_CD;
		else if (xy_streq(pl->argv[0], "pwd"))
			pl->builtin = BI_PWD;
		else if (xy_streq(pl->argv[0], "export"))
			pl->builtin = BI_EXPORT;
		else if (xy_streq(pl->argv[0], "unset"))
			pl->builtin = BI_UNSET;
		else if (xy_streq(pl->argv[0], "env"))
			pl->builtin = BI_ENV;
		else if (xy_streq(pl->argv[0], "exit"))
			pl->builtin = BI_EXIT;
		if (pl->builtin != BI_NONE)
			pl->type = CMD_BUILTIN;
		pl = pl->next;
	}
}

/*duplica una subcadena con memoria nueva*/
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

/*une un segmento de PATH con un comando*/
static char	*path_join(const char *dir, const char *cmd)
{
	char	*path;
	int		dir_len;
	int		cmd_len;

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

/*busca el ejecutable dentro de una entrada PATH concreta*/
static char	*search_path_entry(const char *entry, const char *cmd)
{
	int		start;
	int		end;
	char	*dir;
	char	*path;

	start = 5;
	while (entry[start])
	{
		end = start;
		while (entry[end] && entry[end] != ':')
			end++;
		dir = substr_dup(entry, start, end - start);
		if (!dir)
			return (NULL);
		path = path_join(dir, cmd);
		free(dir);
		if (path && access(path, X_OK) == 0)
			return (path);
		free(path);
		if (entry[end] == ':')
			start = end + 1;
		else
			break ;
	}
	return (NULL);
}

/*busca la ruta ejecutable del comando en PATH*/
char	*find_exec_path(char **envp, const char *cmd)
{
	while (envp && *envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (search_path_entry(*envp, cmd));
		envp++;
	}
	return (NULL);
}
