/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 13:35:10 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/02/09 15:35:18 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_redir(const t_redir *r)
{
	while (r)
	{
		if (r->type == R_IN)
			printf("    redir: < %s\n", r->target);
		else if (r->type == R_OUT)
			printf("    redir: > %s\n", r->target);
		else if (r->type == R_APPEND)
			printf("    redir: >> %s\n", r->target);
		else if (r->type == R_HEREDOC)
			printf("    redir: << %s\n", r->target);
		r = r->next;
	}
}

static void	print_argv(char **argv)
{
	int	i;

	if (!argv)
	{
		printf("    argv: (null)\n");
		return ;
	}
	i = 0;
	printf("    argv:");
	while (argv[i])
	{
		printf(" [%s]", argv[i]);
		i++;
	}
	printf("\n");
}

t_pipeline	*pipeline_new(void)
{
	t_pipeline	*node;

	node = (t_pipeline *)malloc(sizeof(t_pipeline));
	if (!node)
		return (NULL);
	node->cmd = malloc(sizeof(t_command));
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	/* Ensure all fields are initialized even if t_command grows */
	*node->cmd = (t_command){0};
	node->cmd->argv = NULL;
	node->cmd->space = NULL;
	node->cmd->redirs = NULL;
	node->cmd->cmd_info.type = CMD_UNKNOWN;
	node->cmd->cmd_info.builtin = BI_NONE;
	node->next = NULL;
	return (node);
}

void	pipeline_add_back(t_pipeline **lst, t_pipeline *new_node)
{
	t_pipeline	*cur;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}
t_redir	*redir_new(t_token_type type, char *target)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	if (type == TOK_APPEND)
		node->type = R_APPEND;
	else if (type == TOK_HEREDOC)
		node->type = R_HEREDOC;
	else if (type == TOK_REDIR_IN)
		node->type = R_IN;
	else if (type == TOK_REDIR_OUT)
		node->type = R_OUT;
	node->target = ft_strdup(target);
	if (!node->target)
	{
		free(node);
		return (NULL);
	}
	node->expand = 1;
	node->next = NULL;
	return (node);
}

void	redir_add_back(t_redir **lst, t_redir *new_node)
{
	t_redir	*cur;

	if (!lst || !new_node)
		return ;
	if (!*lst)
	{
		*lst = new_node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new_node;
}

void	pipeline_debug_print(const t_pipeline *lst)
{
	int	i;

	i = 0;
	while (lst)
	{
		printf("cmd[%d]:\n", i);
		if (!lst->cmd)
			printf("    (null cmd)\n");
		else
		{
			print_argv(lst->cmd->argv);
			print_redir(lst->cmd->redirs);
		}
		lst = lst->next;
		i++;
	}
}
