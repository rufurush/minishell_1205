/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:42:04 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/26 14:46:05 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*make_redir_node(t_token_type kind, const char *filename,
		int quoted)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->kind = kind;
	node->filename = ft_strdup(filename);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	node->quoted = quoted;
	node->next = NULL;
	return (node);
}

int	parse_redirection(t_ast *cmd, t_token_type kind, t_token_list *content)
{
	t_redir	*node;
	int		quoted;
	char	*filename;

	if (!(kind == INPUT || kind == OUTPUT || kind == APPEND || kind == HEREDOC))
		return (0);
	if (!content || content->type != WORD)
		return (0);
	quoted = 0;
	filename = strip_all_quotes(content->content);
	if (!filename)
		return (0);
	node = make_redir_node(kind, filename, quoted);
	free(filename);
	if (!node)
		return (0);
	append_redir(cmd, node);
	return (1);
}

t_ast	*ast_new_cmd(void)
{
	t_ast	*node;

	node = (malloc(sizeof(t_ast)));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->argv = NULL;
	node->arg_list = NULL;
	node->redirs = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_redirs_a(t_redir *r)
{
	t_redir	*n;

	while (r)
	{
		n = r->next;
		free(r->filename);
		free(r);
		r = n;
	}
}
