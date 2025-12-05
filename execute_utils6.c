/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils6.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:12:28 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/27 15:36:28 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(int stR)
{
	if (WIFEXITED(stR))
		return (WEXITSTATUS(stR));
	else if (WIFSIGNALED(stR))
		return (128 + WTERMSIG(stR));
	else
		return (1);
}

void	free_arg_list_a(t_arg *a)
{
	t_arg	*n;

	while (a)
	{
		n = a->next;
		free(a->value);
		free(a);
		a = n;
	}
}

static void	exec_cmd_child(t_ast *node_list, t_pipex *ps)
{
	char	*full_path;
	char	**envp;
	int		execute;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirs(ps, node_list->redirs) < 0)
		exit(1);
	full_path = resolve_command_path(node_list->argv[0], ps);
	envp = get_envp_as_string(ps);
	execute = execute_command(full_path, node_list->argv, envp, ps);
	if (full_path)
		free(full_path);
	if (envp)
		free_envp(envp);
	exit(execute);
}

static int	wait_cmd_child(pid_t pid, t_pipex *ps)
{
	int	st;
	int	code;

	st = 0;
	waitpid(pid, &st, 0);
	code = get_exit_status(st);
	if (ps)
		ps->last_status = code;
	return (code);
}

int	exec_cmd_node(t_ast *node_list, t_pipex *ps)
{
	pid_t	pid;

	if (!node_list)
		return (0);
	pid = fork();
	if (pid < 0)
	{
		free_arg_list_a(node_list->arg_list);
		return (0);
	}
	if (pid == 0)
		exec_cmd_child(node_list, ps);
	return (wait_cmd_child(pid, ps));
}
