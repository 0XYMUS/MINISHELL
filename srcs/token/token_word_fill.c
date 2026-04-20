/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_word_fill.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jojeda-p <jojeda-p@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 17:30:00 by jojeda-p          #+#    #+#             */
/*   Updated: 2026/04/20 19:32:24 by jojeda-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*detecta comillas vacias y las elimina,
guarda el contexto que puede ser diferencial para el siguiente caracter*/
static int	handle_empty_quote_pair(char *line, t_word_ctx *ctx)
{
	if (ctx->q != 0)
		return (0);
	if ((line[ctx->read] == '\'' || line[ctx->read] == '"')
		&& ctx->read + 1 < ctx->end)
	{
		if (line[ctx->read + 1] == line[ctx->read])
		{
			ctx->pending_q = qmask_from_quote(line[ctx->read]);
			ctx->read += 2;
			return (1);
		}
	}
	return (0);
}

/*apica la logica de backslash y actualiza read y write*/
static int	handle_backslash_step(char *line, t_word_ctx *ctx, char **dst)
{
	int	bs[3];

	bs[0] = ctx->read;
	bs[1] = ctx->end;
	bs[2] = ctx->write;
	if (!handle_backslash(line, bs, ctx->q, dst))
		return (0);
	ctx->read = bs[0];
	ctx->write = bs[2];
	return (1);
}

/*aplica difrentes logicas vistas en funciones anteriores creando 
 word y qmask marcado con el contexto listo para parseo*/
void	fill_word_qmask(char *line, int *idxs, char **dst, int end)
{
	t_word_ctx	ctx;

	ctx.read = idxs[0];
	ctx.write = idxs[1];
	ctx.end = end;
	ctx.q = 0;
	ctx.pending_q = 0;
	while (ctx.read < ctx.end)
	{
		if (handle_empty_quote_pair(line, &ctx)
			|| handle_backslash_step(line, &ctx, dst))
			continue ;
		if (handle_quote(line, &ctx.read, &ctx.q))
			continue ;
		append_with_qmask(dst, &ctx.write, ctx.q, line[ctx.read]);
		if (ctx.pending_q && ctx.write > 0)
		{
			dst[1][ctx.write - 1] = ctx.pending_q;
			ctx.pending_q = 0;
		}
		ctx.read++;
	}
	idxs[0] = ctx.read;
	idxs[1] = ctx.write;
}
