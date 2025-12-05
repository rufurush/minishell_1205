/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:41:26 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:00:44 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_token_type type)
{
	return (type == INPUT || type == OUTPUT || type == APPEND);
}

void	parse_command_syntax_err(t_token_type type)
{
	if (type == AND)
	{
		ft_putstr_fd("minishell$ syntax error near unexpected token '&&'\n", 2);
		return ;
	}
	else
	{
		ft_putstr_fd("minishell$: syntax err\n", 2);
		return ;
	}
}

int	cmd_push_arg_node(t_ast *cmd, const char *str)
{
	t_arg	*new;
	t_arg	*tmp;

	new = malloc(sizeof(t_arg));
	if (!new)
		return (0);
	new->value = ft_strdup(str);
	if (!new->value)
	{
		free(new);
		return (0);
	}
	new->next = NULL;
	if (!cmd->arg_list)
		cmd->arg_list = new;
	else
	{
		tmp = cmd->arg_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}

t_redir	*make_heredoc_node(const char *delim, int quoted)
{
	t_redir	*r;

	r = malloc(sizeof(*r));
	if (!r)
		return (NULL);
	r->kind = HEREDOC;
	r->filename = NULL;
	r->delim = ft_strdup(delim);
	r->quoted = quoted;
	r->next = NULL;
	return (r);
}

void	append_redir(t_ast *cmd, t_redir *node)
{
	t_redir	*p;

	if (!cmd->redirs)
	{
		cmd->redirs = node;
		return ;
	}
	p = cmd->redirs;
	while (p->next)
		p = p->next;
	p->next = node;
}
