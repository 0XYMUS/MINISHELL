/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:27:52 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 10:53:23 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_write_loop(int fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (xy_streq(line, delimiter))
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1 || write(fd, "\n", 1) == -1)
		{
			free(line);
			return (-1);
		}
		free(line);
	}
	return (0);
}

static int	apply_heredoc_redir(const char *delimiter)
{
	char	tmp_name[] = "/tmp/minishell_heredoc_XXXXXX";
	int		fd;

	fd = mkstemp(tmp_name);
	if (fd < 0)
		return (-1);
	if (heredoc_write_loop(fd, delimiter) == -1)
		return (close(fd), unlink(tmp_name), -1);
	if (lseek(fd, 0, SEEK_SET) == -1)
		return (close(fd), unlink(tmp_name), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), unlink(tmp_name), -1);
	close(fd);
	unlink(tmp_name);
	return (0);
}

static int	apply_file_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == R_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == R_OUT)
		fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	if (redir->type == R_IN && dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), -1);
	if (redir->type != R_IN && dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), -1);
	close(fd);
	return (0);
}

int	apply_redirs(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == R_IN || redirs->type == R_OUT
			|| redirs->type == R_APPEND)
		{
			if (apply_file_redir(redirs) == -1)
				return (-1);
		}
		else if (redirs->type == R_HEREDOC)
		{
			if (apply_heredoc_redir(redirs->target) == -1)
				return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}

void	child_process(int prev_read, t_command *pl, int *pipefd, t_shell *sh)
{
	if (prev_read != -1)
		dup2(prev_read, STDIN_FILENO);
	if (pl->next)
		dup2(pipefd[1], STDOUT_FILENO);
	if (prev_read != -1)
		close(prev_read);
	if (pl->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	if (apply_redirs(pl->redirs) == -1)
		exit(1);
	exit(exec_choice(pl, sh));
}