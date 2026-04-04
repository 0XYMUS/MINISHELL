/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julepere <julepere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:24:47 by julepere          #+#    #+#             */
/*   Updated: 2026/04/02 15:23:32 by julepere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

int	run_parent_builtin(t_command *pl, t_shell *sh)
{
	if (pl->builtin == BI_CD)
		return (xy_cd(pl, sh));
	if (pl->builtin == BI_EXPORT)
		return (xy_export(pl, sh));
	if (pl->builtin == BI_UNSET)
		return (xy_unset(pl, sh));
	if (pl->builtin == BI_EXIT)
		return (xy_exit(pl, sh));
	return (0);
}

int	is_parent_builtin(t_command *pl)
{
	if (pl->builtin == BI_CD || pl->builtin == BI_EXIT
		|| pl->builtin == BI_EXPORT || pl->builtin == BI_UNSET)
		return (1);
	return (0);
}


void	parent_process(int *prev_read, t_command *pl, int *pipefd)
{
	if (*prev_read != -1)
		close((*prev_read));
	if (pl->next)
	{
		close(pipefd[1]);
		*prev_read = pipefd[0];
	}
	else
		*prev_read = -1;
}

int	wait_all_children(void)
{
    int	status;
    int	exit_code;

    exit_code = 0;
    while (waitpid(-1, &status, 0) > 0)  // Espera a CUALQUIER hijo (-1)
    {
        // Comprueba si terminó "normalmente" (sin signal)
        if ((status & 0xFF) == 0)
        {
            // Extrae el código de salida (está en bits 8-15)
            exit_code = (status >> 8) & 0xFF;
        }
        // Si fue killeado por signal, no hacemos nada
        // (el padre sigue esperando otros hijos si los hay)
    }
    return (exit_code);  // Retorna el código del último hijo
}

int	execution(t_command *pl, t_shell *sh)
{
	pid_t	pid; //retorno de fork()
	int		pipefd[2]; //pipe del comando actual
	int		prev_read; //fd de lectura del pipe anterior
	int		saved_stdin;
	int		saved_stdout;

	if (!pl)
		return (0);
	resolve_command_type(pl);
	prev_read = -1;
	if (!pl->next && is_parent_builtin(pl))
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1 || saved_stdout == -1)
			return (perror("minishell"), 1);
		if (apply_redirs(pl->redirs) == -1)
			return (close(saved_stdin), close(saved_stdout), 1);
		sh->exit_status = run_parent_builtin(pl, sh);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (sh->exit_status);
	}
	while (pl)
	{
		if (pl->next && pipe(pipefd) == -1)//checkeo de si hay siguiente comando
			return (perror("minishell"), 1);
		pid = fork();
		if (pid < 0)
			return (perror("minishell"), 1);
		if (pid == 0)//comiendo de proceso hijo
		{
			child_process(prev_read, pl, pipefd, sh);
		}
		else//comienzo de proceso padre
		{
			parent_process(&prev_read, pl, pipefd);
		}
		pl = pl->next;
	}
	return (wait_all_children());
}
/*Tabla mental de FDs
Caso 1: comando único sin pipes
Proceso		stdin		stdout			Qué hace
Padre		terminal	terminal	Si es cd/export/unset/exit, ejecuta directamente
Hijo		terminal	terminal	Si decides lanzar otros comandos normales
Caso 2: primer comando de un pipeline
Ejemplo: ls | grep x | wc

Proceso		prev_read	pipefd[0]				pipefd[1]				Acción
Hijo 1		-1			read del pipe nuevo		write del pipe nuevo	dup2(pipefd[1], STDOUT_FILENO)
Padre		-1			read del pipe nuevo		write del pipe nuevo	cierra pipefd[1] y guarda prev_read = pipefd[0]
Caso 3: comando intermedio
Proceso		prev_read				pipefd[0]				pipefd[1]				Acción
Hijo i		read del pipe anterior	read del pipe nuevo		write del pipe nuevo	dup2(prev_read, STDIN_FILENO) y dup2(pipefd[1], STDOUT_FILENO)
Padre		read del pipe anterior	read del pipe nuevo		write del pipe nuevo	cierra prev_read, cierra pipefd[1], guarda prev_read = pipefd[0]
Caso 4: último comando
Proceso		prev_read						pipefd					Acción
Hijo 		último read del pipe anterior	no hay pipe nuevo		dup2(prev_read, STDIN_FILENO)
Padre		read del pipe anterior			no hay pipe nuevo		cierra prev_read, deja prev_read = -1
*/