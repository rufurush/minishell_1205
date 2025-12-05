/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils5.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:01:28 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:06:17 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_pipe_node(t_pipex *ps, t_ast *left, t_ast *right)
{
	int		fds[2];
	pid_t	lp;
	pid_t	rp;
	int		last;

	if (pipe(fds) < 0)
		return (1);
	set_signals_parent_wait();
	lp = fork();
	if (lp == 0)
		ms_run_left_child(ps, left, fds);
	rp = fork();
	if (rp == 0)
		ms_run_right_child(ps, right, fds);
	close(fds[0]);
	close(fds[1]);
	last = ms_wait_pipe_children(lp, rp);
	set_signals_prompt();
	return (last);
}

int	post_read_signal_handle(char *line, t_pipex *ps, int *should_continue)
{
	*should_continue = 0;
	if (g_sigflags == SIGINT)
		ps->last_status = 130;
	else if (g_sigflags == SIGQUIT)
		ps->last_status = 131;
	if (g_sigflags == SIGINT || g_sigflags == SIGQUIT)
	{
		if (line[0] == '\0')
		{
			g_sigflags = 0;
			free(line);
			*should_continue = 1;
			return (1);
		}
		g_sigflags = 0;
	}
	return (0);
}
