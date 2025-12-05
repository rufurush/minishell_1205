/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:22:04 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:06:44 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_arg_list(t_ast *node, t_pipex *ps)
{
	t_arg	*arg;
	char	*new_val;

	arg = node->arg_list;
	while (arg)
	{
		new_val = expand_one(arg->value, ps);
		if (!new_val)
			return (FAILURE);
		free(arg->value);
		arg->value = new_val;
		arg = arg->next;
	}
	return (SUCCESS);
}

static void	free_node_argv(t_ast *node)
{
	size_t	i;

	if (!node->argv)
		return ;
	i = 0;
	while (node->argv[i])
	{
		free(node->argv[i]);
		i++;
	}
	free(node->argv);
	node->argv = NULL;
}

int	expand_command(t_ast *node, t_pipex *ps)
{
	if (expand_arg_list(node, ps) == FAILURE)
		return (FAILURE);
	free_node_argv(node);
	node->argv = list_to_argv(node->arg_list);
	if (!node->argv)
		return (FAILURE);
	return (SUCCESS);
}

int	handle_quote_state(char c, int *in_squote, int *in_dquote, size_t *i)
{
	if (c == '\'' && !*in_dquote)
	{
		*in_squote = !*in_squote;
		(*i)++;
		return (1);
	}
	if (c == '"' && !*in_squote)
	{
		*in_dquote = !*in_dquote;
		(*i)++;
		return (1);
	}
	return (0);
}

int	handle_status_var(t_pipex *ps, char **out, size_t *i)
{
	char	*s;
	int		status;

	if (ps)
		status = ps->last_status;
	else
		status = 0;
	s = itoa_status(status);
	if (!s || !sb_append(out, s, ft_strlen(s)))
	{
		free(s);
		free(*out);
		return (0);
	}
	free(s);
	(*i)++;
	return (1);
}
