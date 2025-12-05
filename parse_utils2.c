/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:49:50 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:01:09 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_free_a(t_ast *n)
{
	int	i;

	if (!n)
		return ;
	ast_free_a(n->left);
	ast_free_a(n->right);
	if (n->argv)
	{
		i = 0;
		while (n->argv[i])
		{
			free(n->argv[i]);
			i++;
		}
		free(n->argv);
	}
	free_arg_list_a(n->arg_list);
	free_redirs_a(n->redirs);
	free(n);
}

t_ast	*parse_command(t_token_list **cur)
{
	t_ast	*cmd;
	int		seen;
	int		ret;

	seen = 0;
	cmd = ast_new_cmd();
	if (!cmd)
		return (NULL);
	ret = parse_command_loop(cur, cmd, &seen);
	if (ret == 2)
		return (NULL);
	if (ret == 1 || !seen)
	{
		ast_free_a(cmd);
		return (NULL);
	}
	cmd->argv = list_to_argv(cmd->arg_list);
	return (cmd);
}

t_ast	*ast_new_pipe(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->argv = NULL;
	node->arg_list = NULL;
	node->redirs = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

void	consume(t_token_list **cur, t_token_type expected)
{
	if (*cur == NULL)
		return ;
	if ((*cur)->type == expected)
		*cur = (*cur)->next;
	else
		ft_putstr_fd("minishell$: parse error: unexpected token\n", 2);
}
