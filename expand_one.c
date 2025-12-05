/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:31:06 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:28:00 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_env_var(t_expand_ctx *ctx)
{
	char	*key;
	char	*val;
	size_t	start;

	start = ctx->i;
	while (ctx->arg[ctx->i] && is_var_char((unsigned char)ctx->arg[ctx->i]))
		ctx->i++;
	key = ft_substr(ctx->arg, start, ctx->i - start);
	if (!key)
		return (free(ctx->out), 0);
	val = ms_getenv_dup(ctx->ps, key);
	free(key);
	if (!val || !sb_append(&ctx->out, val, ft_strlen(val)))
		return (free(val), free(ctx->out), 0);
	free(val);
	return (1);
}

static int	handle_status_var(t_expand_ctx *ctx)
{
	char	*status_str;
	int		ok;

	status_str = ft_itoa(ctx->ps->last_status);
	if (!status_str)
		return (free(ctx->out), 0);
	ok = sb_append(&ctx->out, status_str, ft_strlen(status_str));
	free(status_str);
	if (!ok)
	{
		free(ctx->out);
		return (0);
	}
	ctx->i++;
	return (1);
}

int	handle_dollar(t_expand_ctx *ctx)
{
	ctx->should_break = 0;
	ctx->i++;
	if (ctx->arg[ctx->i] == '\0')
	{
		if (!sb_append(&ctx->out, "$", 1))
			return (free(ctx->out), 0);
		ctx->should_break = 1;
		return (1);
	}
	if (ctx->arg[ctx->i] == '?')
		return (handle_status_var(ctx));
	if (is_env_start((unsigned char)ctx->arg[ctx->i]))
		return (handle_env_var(ctx));
	if (!sb_append(&ctx->out, "$", 1))
		return (free(ctx->out), 0);
	return (1);
}

static int	try_backslash_outside_quotes(t_expand_ctx *ctx)
{
	char	next;

	if (ctx->in_squote || ctx->in_dquote)
		return (-1);
	if (ctx->arg[ctx->i] != '\\')
		return (-1);
	next = ctx->arg[ctx->i + 1];
	if (next != '\0')
	{
		if (!sb_append(&ctx->out, &next, 1))
			return (free(ctx->out), 0);
		ctx->i += 2;
		return (1);
	}
	ctx->i++;
	return (1);
}

int	expand_one_step(t_expand_ctx *ctx)
{
	int	res;

	res = try_backslash_outside_quotes(ctx);
	if (res != -1)
		return (res);
	res = try_backslash_in_dquote(ctx);
	if (res != -1)
		return (res);
	res = try_quote_or_dollar(ctx);
	if (res != -1)
		return (res);
	if (!sb_append(&ctx->out, &ctx->arg[ctx->i], 1))
		return (free(ctx->out), 0);
	ctx->i++;
	return (1);
}

// static int	expand_one_step(t_expand_ctx *ctx)
// {
// 	int		res;
// 	char	c;
// 	char	next;

// 	c = ctx->arg[ctx->i];
// 	if (!ctx->in_squote && !ctx->in_dquote && c == '\\')
// 	{
// 		next = ctx->arg[ctx->i + 1];
// 		if (next != '\0')
// 		{
// 			if (!sb_append(&ctx->out, &next, 1))
// 				return (free(ctx->out), 0);
// 			ctx->i += 2;
// 			return (1);
// 		}
// 		ctx->i++;
// 		return (1);
// 	}
// 	if (ctx->in_dquote && !ctx->in_squote && c == '\\')
// 	{
// 		next = ctx->arg[ctx->i + 1];
// 		if (next == '"' || next == '$' || next == '\\')
// 		{
// 			if (!sb_append(&ctx->out, &next, 1))
// 				return (free(ctx->out), 0);
// 			ctx->i += 2;
// 			return (1);
// 		}
// 	}
// 	if (handle_quote_state(ctx->arg[ctx->i], &ctx->in_squote, &ctx->in_dquote,
// 			&ctx->i))
// 		return (1);
// 	if (!ctx->in_squote && ctx->arg[ctx->i] == '$')
// 	{
// 		res = handle_dollar(ctx);
// 		if (!res)
// 			return (0);
// 		if (ctx->should_break)
// 			return (2);
// 		return (1);
// 	}
// 	if (!sb_append(&ctx->out, &ctx->arg[ctx->i], 1))
// 		return (free(ctx->out), 0);
// 	ctx->i++;
// 	return (1);
// }