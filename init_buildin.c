/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:50:44 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 18:00:42 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	streq(const char *s, const char *lit)
{
	size_t	n;

	n = ft_strlen(lit);
	return (s && ft_strncmp(s, lit, n) == 0 && s[n] == '\0');
}

int	exec_builtin(t_pipex *ps, t_ast *node_list)
{
	if (!node_list->argv || !node_list->argv[0])
		return (0);
	else if (streq(node_list->argv[0], "echo"))
		ps->last_status = exec_builtin_echo(node_list);
	else if (streq(node_list->argv[0], "cd"))
		ps->last_status = exec_builtin_cd(ps, node_list);
	else if (streq(node_list->argv[0], "pwd"))
		ps->last_status = exec_builtin_pwd(ps);
	else if (streq(node_list->argv[0], "unset"))
		ps->last_status = exec_builtin_unset(ps, node_list);
	else if (streq(node_list->argv[0], "export"))
		ps->last_status = exec_builtin_export(ps, node_list);
	else if (streq(node_list->argv[0], "env"))
		ps->last_status = exec_builtin_env(ps);
	else if (streq(node_list->argv[0], "exit"))
		ps->last_status = exec_builtin_exit(ps, node_list);
	return (ps->last_status);
}

int	is_builtin(char *argv)
{
	if (!argv)
		return (FAILURE);
	if (streq(argv, "echo"))
		return (SUCCESS);
	if (streq(argv, "cd"))
		return (SUCCESS);
	if (streq(argv, "pwd"))
		return (SUCCESS);
	if (streq(argv, "export"))
		return (SUCCESS);
	if (streq(argv, "unset"))
		return (SUCCESS);
	if (streq(argv, "env"))
		return (SUCCESS);
	if (streq(argv, "exit"))
		return (SUCCESS);
	return (FAILURE);
}

int	is_parent_builtin(t_ast *node_list)
{
	if (node_list->type != NODE_CMD)
		return (FAILURE);
	if (!node_list->argv || !node_list->argv[0])
		return (FAILURE);
	if (node_list->redirs != NULL)
		return (FAILURE);
	return (is_builtin(node_list->argv[0]));
}
