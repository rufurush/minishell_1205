/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 17:22:32 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/01 19:15:07 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe_and_left(t_ast *node_list, t_pipex *ps, char *line,
		t_pipe_ctx *pctx)
{
	pctx->left = -1;
	if (pipe(pctx->fds) < 0)
		return (1);
	pctx->left = fork();
	if (pctx->left < 0)
	{
		close(pctx->fds[0]);
		close(pctx->fds[1]);
		return (1);
	}
	if (pctx->left == 0)
		exec_left_child(node_list, ps, line, pctx->fds);
	return (0);
}

int	fork_right_process(t_ast *node_list, t_pipex *ps, char *line,
		t_pipe_ctx *pctx)
{
	pctx->right = fork();
	if (pctx->right < 0)
	{
		close(pctx->fds[0]);
		close(pctx->fds[1]);
		waitpid(pctx->left, NULL, 0);
		return (1);
	}
	if (pctx->right == 0)
		exec_right_child(node_list, ps, line, pctx->fds);
	return (0);
}

int	exec_cmd_pipe_node(t_ast *node_list, t_pipex *ps, char *line)
{
	t_pipe_ctx	pctx;

	if (setup_pipe_and_left(node_list, ps, line, &pctx) != 0)
		return (1);
	if (fork_right_process(node_list, ps, line, &pctx) != 0)
		return (1);
	close(pctx.fds[0]);
	close(pctx.fds[1]);
	return (wait_pipe_children(pctx.left, pctx.right));
}

int	execute_ast(t_ast *node_list, t_pipex *ps, char *line)
{
	if (!node_list)
		return (1);
	if (node_list->type == NODE_CMD)
		return (exec_cmd_node(node_list, ps));
	if (node_list->type == NODE_PIPE)
		return (exec_cmd_pipe_node(node_list, ps, line));
	return (1);
}

// #include "minishell.h"

// int	setup_pipe_and_left(t_ast *node_list, t_pipex *ps, char *line,
// 		int fds[2], pid_t *left)
// {
// 	*left = -1;
// 	if (pipe(fds) < 0)
// 		return (1);
// 	*left = fork();
// 	if (*left < 0)
// 	{
// 		close(fds[0]);
// 		close(fds[1]);
// 		return (1);
// 	}
// 	if (*left == 0)
// 		exec_left_child(node_list, ps, line, fds);
// 	return (0);
// }

// int	fork_right_process(t_ast *node_list, t_pipex *ps, char *line,
// 		int fds[2], pid_t left, pid_t *right)
// {
// 	*right = fork();
// 	if (*right < 0)
// 	{
// 		close(fds[0]);
// 		close(fds[1]);
// 		waitpid(left, NULL, 0);
// 		return (1);
// 	}
// 	if (*right == 0)
// 		exec_right_child(node_list, ps, line, fds);
// 	return (0);
// }

// int	exec_cmd_pipe_node(t_ast *node_list, t_pipex *ps, char *line)
// {
// 	int		fds[2];
// 	pid_t	left;
// 	pid_t	right;

// 	if (setup_pipe_and_left(node_list, ps, line, fds, &left) != 0)
// 		return (1);
// 	if (fork_right_process(node_list, ps, line, fds, left, &right) != 0)
// 		return (1);
// 	close(fds[0]);
// 	close(fds[1]);
// 	return (wait_pipe_children(left, right));
// }

// int	execute_ast(t_ast *node_list, t_pipex *ps, char *line)
// {
// 	if (!node_list)
// 		return (1);
// 	if (node_list->type == NODE_CMD)
// 		return (exec_cmd_node(node_list, ps));
// 	if (node_list->type == NODE_PIPE)
// 		return (exec_cmd_pipe_node(node_list, ps, line));
// 	return (1);
// }
