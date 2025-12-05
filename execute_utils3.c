/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:52:26 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:05:19 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_node(t_pipex *ps, t_ast *node)
{
	pid_t	pid;
	int		status;

	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (run_pipe_node(ps, node->left, node->right));
	set_signals_parent_wait();
	pid = rn_fork_and_exec(ps, node);
	status = rn_wait_pid(pid);
	set_signals_prompt();
	return (status);
}

void	free_splits(char **cmd_args)
{
	int	i;

	i = 0;
	while (cmd_args[i] != NULL)
	{
		free(cmd_args[i]);
		i++;
	}
	free(cmd_args);
}

int	line_is_empty(const char *line)
{
	return (!line || !*line);
}

int	validate_cmd_args_like_original(const char *line)
{
	char	**cmd_args;

	cmd_args = ft_split(line, ' ');
	if (!cmd_args || !cmd_args[0] || !*cmd_args[0])
	{
		free_splits(cmd_args);
		return (1);
	}
	free_splits(cmd_args);
	return (0);
}

void	free_token_list(t_token_list **list)
{
	t_token_list	*cur;
	t_token_list	*next;

	if (!list || !*list)
		return ;
	cur = *list;
	while (cur)
	{
		next = cur->next;
		free(cur->content);
		free(cur);
		cur = next;
	}
	*list = NULL;
}
