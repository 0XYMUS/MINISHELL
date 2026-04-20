/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:31:13 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 20:08:59 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*redirecciona stdin y stdout y usa tty para leer el heredoc visible*/
static int	setup_heredoc_terminal(int *saved_in, int *saved_out,
	int *terminal_fd)
{
	*terminal_fd = open("/dev/tty", O_RDWR);
	if (*terminal_fd == -1)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in == -1 || *saved_out == -1)
	{
		close(*terminal_fd);
		return (-1);
	}
	if (dup2(*terminal_fd, STDIN_FILENO) == -1
		|| dup2(*terminal_fd, STDOUT_FILENO) == -1)
	{
		close(*terminal_fd);
		close(*saved_in);
		close(*saved_out);
		return (-1);
	}
	return (1);
}

/*restaura stdin/stdout originales tras leer el heredoc*/
static void	restore_heredoc_terminal(int saved_in, int saved_out,
	int terminal_fd)
{
	if (terminal_fd == -1)
		return ;
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	close(terminal_fd);
}

/*expande una linea de heredoc como texto normal cuando corresponde*/
static int	heredoc_expand_line(char **line, t_shell sh)
{
	char	*qmask;
	int		i;

	qmask = ft_strdup(*line);
	if (!qmask)
		return (-1);
	i = 0;
	while (qmask[i])
		qmask[i++] = '0';
	question_case(line, &qmask, sh);
	handle_pid_expansion(line, &qmask, 0);
	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$' && (*line)[i + 1] && is_var_char((*line)[i + 1]))
			(expand_word(line, &qmask, sh, i + 1), i = 0);
		else
			i++;
	}
	free(qmask);
	return (0);
}

/*lee el heredoc y guarda su contenido en un pipe*/
int	heredoc_write_loop(int temp_fd, const char *delimiter,
	int expand, t_shell sh)
{
	char	*input_line;

	while (1)
	{
		input_line = readline("> ");
		if (!input_line || xy_streq(input_line, delimiter))
			return (free(input_line), 0);
		if (expand && heredoc_expand_line(&input_line, sh) == -1)
			return (free(input_line), -1);
		if (write(temp_fd, input_line, ft_strlen(input_line)) == -1
			|| write(temp_fd, "\n", 1) == -1)
			return (free(input_line), -1);
		free(input_line);
	}
	return (0);
}

/*crea y prepara una redirección heredoc para stdin*/
int	apply_heredoc_redir(t_redir *redir, t_shell sh)
{
	int	pipefd[2];
	int	saved_in;
	int	saved_out;
	int	terminal_fd;
	int	terminal_status;

	if (pipe(pipefd) == -1)
		return (-1);
	saved_in = -1;
	saved_out = -1;
	terminal_fd = -1;
	terminal_status = setup_heredoc_terminal(&saved_in, &saved_out,
		&terminal_fd);
	if (terminal_status == -1)
		return (close(pipefd[1]), close(pipefd[0]), -1);
	if (heredoc_write_loop(pipefd[1], redir->target, redir->expand, sh) == -1)
	{
		restore_heredoc_terminal(saved_in, saved_out, terminal_fd);
		return (close(pipefd[1]), close(pipefd[0]), -1);
	}
	restore_heredoc_terminal(saved_in, saved_out, terminal_fd);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (close(pipefd[0]), -1);
	close(pipefd[0]);
	return (0);
}
