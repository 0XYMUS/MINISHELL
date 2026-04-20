/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:31:13 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 13:55:26 by jojeda-p         ###   ########.fr       */
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

/*lee el heredoc y guarda su contenido en un archivo temporal*/
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

/*genera una ruta temporal unica para el heredoc con un indice*/
static void	build_heredoc_path(char *tmp_path, int file_index)
{
	int		digit_count;
	int		path_pos;
	char	reversed_digits[12];

	ft_memcpy(tmp_path, "/tmp/minishell_heredoc_", 23);
	digit_count = 0;
	if (file_index == 0)
		reversed_digits[digit_count++] = '0';
	while (file_index > 0)
	{
		reversed_digits[digit_count++] = (file_index % 10) + '0';
		file_index = file_index / 10;
	}
	path_pos = 0;
	while (digit_count > 0)
		tmp_path[23 + path_pos++] = reversed_digits[--digit_count];
	tmp_path[23 + path_pos] = '\0';
}

/*abre un archivo temporal heredoc evitando colisiones de nombre*/
static int	open_heredoc_tmp(char *tmp_path)
{
	static int	index;
	int			attempt;
	int			tmp_fd;

	attempt = 0;
	while (attempt < 1000)
	{
		build_heredoc_path(tmp_path, index++);
		tmp_fd = open(tmp_path, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (tmp_fd >= 0)
			return (tmp_fd);
		if (errno != EEXIST)
			return (-1);
		attempt++;
	}
	return (-1);
}

/*crea y prepara una redirección heredoc para stdin*/
int	apply_heredoc_redir(t_redir *redir, t_shell sh)
{
	char	tmp_path[64];
	int		write_fd;
	int		read_fd;

	write_fd = open_heredoc_tmp(tmp_path);
	if (write_fd < 0)
		return (-1);
	if (heredoc_write_loop(write_fd, redir->target, redir->expand, sh) == -1)
		return (close(write_fd), unlink(tmp_path), -1);
	close(write_fd);
	read_fd = open(tmp_path, O_RDONLY);
	if (read_fd < 0)
		return (unlink(tmp_path), -1);
	unlink(tmp_path);
	if (dup2(read_fd, STDIN_FILENO) == -1)
		return (close(read_fd), -1);
	close(read_fd);
	return (0);
}
