/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_buildin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:59:48 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:59:32 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	export_identifier_error(char *arg, char *key, char *val,
		int *status)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	*status = 1;
	free(key);
	free(val);
}

static void	process_one_export_arg(t_pipex *ps, char *arg, int *status)
{
	char	*key;
	char	*val;

	key = NULL;
	val = NULL;
	if (!split_key_value(arg, &key, &val))
	{
		ft_putstr_fd("minishell: export: allocation failed\n", STDERR_FILENO);
		*status = 1;
		return ;
	}
	if (!is_valid_identifier(key))
	{
		export_identifier_error(arg, key, val, status);
		return ;
	}
	if (update_env_value(ps, key, val) != 0)
		*status = 1;
	free(key);
	free(val);
}

static int	export_args(t_pipex *ps, t_ast *node_list)
{
	size_t	i;
	int		status;

	status = 0;
	i = 1;
	while (node_list->argv[i])
	{
		process_one_export_arg(ps, node_list->argv[i], &status);
		i++;
	}
	return (status);
}

int	exec_builtin_export(t_pipex *ps, t_ast *node_list)
{
	if (!ps || !node_list || !node_list->argv)
		return (1);
	if (!node_list->argv[1])
	{
		print_env_list_simple(ps->env_list);
		return (0);
	}
	return (export_args(ps, node_list));
}
