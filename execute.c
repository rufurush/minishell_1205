/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:26:14 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 12:59:48 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_parsed_line(char *line, t_pipex *ps)
{
	t_ast	*node_list;
	int		handled;
	int		build_res;

	add_history(line);
	build_res = build_nodes_from_line(line, &node_list, ps);
	if (build_res < 0)
		return (free(line), -1);
	if (build_res > 0)
		return (free(line), 0);
	if (try_parent_builtin_then_cleanup(node_list, ps, line, &handled) < 0)
		return (-1);
	if (handled)
		return (0);
	if (validate_cmd_args_like_original(line))
		return (free(line), 0);
	ps->last_status = run_pipeline_and_cleanup(node_list, ps);
	return (0);
}

int	handle_signals_and_empty(char *line, t_pipex *ps)
{
	int	next;

	if (post_read_signal_handle(line, ps, &next))
	{
		if (next)
			return (1);
		return (2);
	}
	if (line_is_empty(line))
		return (free(line), 1);
	return (0);
}

int	process_line(char *line, t_pipex *ps)
{
	int	res;

	res = handle_signals_and_empty(line, ps);
	if (res == 1)
		return (0);
	if (res == 2)
		return (1);
	return (process_parsed_line(line, ps));
}

int	run_line(t_pipex *ps)
{
	char	*line;
	int		ret;

	setup_signal_parents();
	line = ms_readline();
	if (!line)
	{
		if (isatty(STDIN_FILENO))
			printf("exit\n");
		return (1);
	}
	if (!*line || is_all_space(line))
		return (free(line), 0);
	ret = process_line(line, ps);
	if (ret != 0)
		return (ret);
	return (0);
}

int	execute(int argc, char **argv, t_pipex *ps)
{
	(void)argc;
	(void)argv;
	return (run_line(ps));
}
