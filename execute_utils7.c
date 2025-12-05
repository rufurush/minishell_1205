/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils7.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:14:01 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/27 15:36:49 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ========== exec_cmd_pipe_node の分割 ========== */

void	exec_left_child(t_ast *node_list, t_pipex *ps, char *line,
		int fds[2])
{
	int	st;

	close(fds[0]);
	if (dup2(fds[1], 1) == -1)
		exit(1);
	close(fds[1]);
	st = execute_ast(node_list->left, ps, line);
	exit(st);
}

void	exec_right_child(t_ast *node_list, t_pipex *ps, char *line,
		int fds[2])
{
	int	st;

	close(fds[1]);
	if (dup2(fds[0], 0) < 0)
		exit(1);
	close(fds[0]);
	st = execute_ast(node_list->right, ps, line);
	exit(st);
}

int	wait_pipe_children(pid_t left, pid_t right)
{
	int	st_left;
	int	st_right;

	st_left = 0;
	st_right = 0;
	waitpid(left, &st_left, 0);
	waitpid(right, &st_right, 0);
	return (get_exit_status(st_right));
}

int	is_all_space(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
