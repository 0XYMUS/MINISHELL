/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:31:13 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/22 16:47:54 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (g_signal == SIGINT)
			return (free(input_line), -1);
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

static int	heredoc_child_write(int temp_fd, t_redir *redir, t_shell sh)
{
	g_signal = 0;
	catch_signal_heredoc();
	signal(SIGQUIT, SIG_IGN);
	if (heredoc_write_loop(temp_fd, redir->target, redir->expand, sh) == -1)
		exit(130);
	exit(0);
}

/*crea y prepara una redirección heredoc para stdin*/
int	apply_heredoc_redir(t_redir *redir, t_shell *sh)
{
	int		pipefd[2];
	int		tty[3];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (-1);
	tty[0] = -1;
	tty[1] = -1;
	tty[2] = -1;
	g_signal = 0;
	if (setup_heredoc_terminal(&tty[1], &tty[2], &tty[0]) == -1)
		return (close(pipefd[1]), close(pipefd[0]), -1);
	pid = fork();
	if (pid < 0)
		return (restore_heredoc_terminal(tty[1], tty[2], tty[0]),
			close(pipefd[1]), close(pipefd[0]), -1);
	if (pid == 0)
		(close(pipefd[0]), heredoc_child_write(pipefd[1], redir, *sh));
	close(pipefd[1]);
	return (wait_heredoc_child(pid, sh, pipefd[0], tty));
}
