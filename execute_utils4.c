/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils4.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:54:32 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/27 15:36:17 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_simple_in_this_process(t_pipex *ps, t_ast *cmd)
{
	int		r;
	int		st;
	char	*full;
	char	**envp;

	r = apply_redirs(ps, cmd->redirs);
	if (r == HEREDOC_ABORT)
		return (130);
	if (r != 0)
		return (1);
	if (!cmd->argv || !cmd->argv[0] || !*cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0]) == SUCCESS)
		return (exec_builtin(ps, cmd));
	full = resolve_command_path(cmd->argv[0], ps);
	envp = get_envp_as_string(ps);
	st = execute_command(full, cmd->argv, envp, ps);
	free(full);
	free_envp(envp);
	return (st);
}

int	ms_pipe_status_from_wait(int st)
{
	int	last;
	int	sig;

	last = 0;
	if (WIFEXITED(st))
		last = WEXITSTATUS(st);
	else if (WIFSIGNALED(st))
	{
		sig = WTERMSIG(st);
		last = 128 + sig;
		if (sig == SIGINT)
			write(2, "\n", 1);
		else if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
	}
	return (last);
}

int	ms_wait_pipe_children(pid_t lp, pid_t rp)
{
	int	st;
	int	last;

	st = 0;
	last = 0;
	waitpid(lp, NULL, 0);
	if (waitpid(rp, &st, 0) > 0)
		last = ms_pipe_status_from_wait(st);
	return (last);
}

void	ms_run_left_child(t_pipex *ps, t_ast *left, int fds[2])
{
	int	st;

	set_signals_child();
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
	if (left->type == NODE_PIPE)
		st = run_pipe_node(ps, left->left, left->right);
	else
		st = run_simple_in_this_process(ps, left);
	exit(st);
}

void	ms_run_right_child(t_pipex *ps, t_ast *right, int fds[2])
{
	int	st;

	set_signals_child();
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	close(fds[1]);
	if (right->type == NODE_PIPE)
		st = run_pipe_node(ps, right->left, right->right);
	else
		st = run_simple_in_this_process(ps, right);
	exit(st);
}
