/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 09:17:53 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:59:14 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_start(int c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_var_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

int	sb_append(char **dst, const char *piece, size_t len)
{
	size_t	a;
	char	*nbuf;

	if (*dst)
		a = ft_strlen(*dst);
	else
		a = 0;
	nbuf = (char *)malloc(a + len + 1);
	if (!nbuf)
		return (0);
	if (*dst)
	{
		ft_memcpy(nbuf, *dst, a);
		free(*dst);
	}
	if (piece && len)
		ft_memcpy(nbuf + a, piece, len);
	nbuf[a + len] = '\0';
	*dst = nbuf;
	return (1);
}

char	*itoa_status(int st)
{
	return (ft_itoa(st));
}

int	expand_command_line(t_ast *node, t_pipex *ps)
{
	if (!node)
		return (SUCCESS);
	if (node->type == NODE_CMD)
		return (expand_command(node, ps));
	if (node->type == NODE_PIPE)
	{
		if (expand_command_line(node->left, ps) == FAILURE)
			return (FAILURE);
		if (expand_command_line(node->right, ps) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
