/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:38:58 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:26:17 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ms_getenv_dup(t_pipex *ps, const char *key)
{
	t_env_list	*cur;
	size_t		key_len;

	if (!ps || !key || !*key)
		return (ft_strdup(""));
	cur = ps->env_list;
	key_len = ft_strlen(key);
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(cur->key)) == 0
			&& ft_strlen(cur->key) == key_len)
		{
			if (cur->value)
				return (ft_strdup(cur->value));
			else
				return (ft_strdup(""));
		}
		cur = cur->next;
	}
	return (ft_strdup(""));
}

char	*expand_one(const char *arg, t_pipex *ps)
{
	t_expand_ctx	ctx;

	ctx.arg = arg;
	ctx.ps = ps;
	ctx.out = ft_strdup("");
	if (!ctx.out)
		return (NULL);
	ctx.i = 0;
	ctx.in_squote = 0;
	ctx.in_dquote = 0;
	ctx.should_break = 0;
	if (!expand_one_loop(&ctx))
		return (NULL);
	return (ctx.out);
}

int	expand_one_loop(t_expand_ctx *ctx)
{
	int	res;

	ctx->i = 0;
	ctx->in_squote = 0;
	ctx->in_dquote = 0;
	while (ctx->arg[ctx->i] != '\0')
	{
		res = expand_one_step(ctx);
		if (res == 0)
			return (0);
		if (res == 2)
			break ;
	}
	return (1);
}

int	try_backslash_in_dquote(t_expand_ctx *ctx)
{
	char	next;

	if (!ctx->in_dquote || ctx->in_squote)
		return (-1);
	if (ctx->arg[ctx->i] != '\\')
		return (-1);
	next = ctx->arg[ctx->i + 1];
	if (next != '"' && next != '$' && next != '\\')
		return (-1);
	if (!sb_append(&ctx->out, &next, 1))
		return (free(ctx->out), 0);
	ctx->i += 2;
	return (1);
}

int	try_quote_or_dollar(t_expand_ctx *ctx)
{
	int	res;

	if (handle_quote_state(ctx->arg[ctx->i], &ctx->in_squote,
			&ctx->in_dquote, &ctx->i))
		return (1);
	if (ctx->in_squote || ctx->arg[ctx->i] != '$')
		return (-1);
	res = handle_dollar(ctx);
	if (!res)
		return (0);
	if (ctx->should_break)
		return (2);
	return (1);
}
