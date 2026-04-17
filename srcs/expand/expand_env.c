/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 14:00:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/17 16:15:15 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*a la hora expandir, priemro copia el texto antes de la expansion,
despues cambia el $ por el ontenido de nev y rellena con el resto del texto*/
static void	fill_expansion(char *env, char *word, char *expansion, int *pos)
{
	int	j;
	int	k;

	j = 0;
	while (j < pos[0])
	{
		expansion[j] = word[j];
		j++;
	}
	k = 0;
	while (env[k] && env[k] != '=')
		k++;
	while (env[k + 1])
		expansion[j++] = env[++k];
	while (word[pos[1]])
		expansion[j++] = word[pos[1]++];
}

/*recalcula los tamanos e los word y qmask tras expandir, y ejecuta expansion*/
static void	get_expansion(char *env, char **word, int i, char **qmask)
{
	char	*expansion;
	char	*newq;
	int		len;
	int		len_q;
	int		pos[2];

	len = expand_len(*word, env, *qmask, i);
	expansion = malloc(sizeof(char) * len + 1);
	if (!expansion)
		return ;
	pos[0] = i;
	pos[1] = get_end(*word, *qmask, i);
	fill_expansion(env, *word, expansion, pos);
	expansion[len] = '\0';
	len_q = expansion_len(*word, *qmask, i);
	newq = update_qmask_after_expansion(*qmask, i, len_q, len);
	if (!newq)
		return (free(expansion), (void)0);
	free((*word));
	free((*qmask));
	(*word) = expansion;
	*qmask = newq;
}

/*comprueba si una variable de env coincide con la expansion*/
static int	match_env(char *env, char *word, char *qmask, int i)
{
	int		k;
	char	ctx;

	k = 0;
	ctx = qmask[i];
	while (env[k] && word[i] && env[k] == word[i] && qmask[i] == ctx)
	{
		k++;
		i++;
		if (env[k] == '='
			&& (!word[i] || !is_var_char(word[i])
				|| qmask[i] != ctx))
			return (1);
	}
	return (0);
}

/*recorre env y aplica las expansiones cuando hay match, sino la elimina*/
void	expand_word(char **word, char **qmask, t_shell sh, int i)
{
	int	j;
	int	start_i;
	int	found;

	j = 0;
	start_i = i;
	found = 0;
	while (sh.envp[j])
	{
		if (match_env(sh.envp[j], *word, *qmask, i))
		{
			get_expansion(sh.envp[j], word, start_i - 1, qmask);
			found = 1;
		}
		j++;
	}
	if (!found)
		remove_word(word, qmask, start_i - 1);
}
