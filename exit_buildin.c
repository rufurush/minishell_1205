/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:19:26 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/26 14:06:29 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_numeric_error(t_pipex *ps, char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	free_pipex(ps);
	exit(2);
}

static void	validate_exit_arg(t_pipex *ps, char *arg)
{
	int	i;

	if (arg[0] == '\0')
		exit_numeric_error(ps, arg);
	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]) && !(i == 0 && (arg[i] == '+'
					|| arg[i] == '-')))
			exit_numeric_error(ps, arg);
		i++;
	}
}

static int	handle_too_many_args(t_pipex *ps, t_ast *node_list)
{
	if (node_list->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		ps->last_status = 1;
		return (1);
	}
	return (0);
}

int	exec_builtin_exit(t_pipex *ps, t_ast *node_list)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!node_list->argv[1])
		exit(ps->last_status);
	validate_exit_arg(ps, node_list->argv[1]);
	if (handle_too_many_args(ps, node_list))
		return (1);
	exit_code = ft_atoi(node_list->argv[1]);
	free_pipex(ps);
	exit(exit_code % 256);
}
