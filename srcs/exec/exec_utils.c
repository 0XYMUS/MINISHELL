/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 00:00:00 by julepere          #+#    #+#             */
/*   Updated: 2026/04/23 16:41:21 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

/*clasifica cada comando en la pipeline como builtin, external o desconocido*/
void	resolve_command_type(t_cmd *pl)
{
	while (pl)
	{
		pl->type = CMD_EXTERNAL;
		if (!pl->argv || !pl->argv[0])
			pl->type = CMD_UNKNOWN;
		else
		{
			pl->builtin = get_builtin_type(pl->argv[0]);
			if (pl->builtin != BI_NONE)
				pl->type = CMD_BUILTIN;
		}
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
