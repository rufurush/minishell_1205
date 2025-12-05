/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_buildin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:34:55 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:11:07 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*cd_get_target(t_pipex *ps, t_ast *node_list)
{
	char	*target;
	char	*argv;

	argv = node_list->argv[1];
	if (!argv || ft_strncmp(argv, "~", 1) == 0)
	{
		target = get_env_value(ps, "HOME");
		if (!target)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), NULL);
	}
	else if (ft_strncmp(argv, "-", 1) == 0)
	{
		target = get_env_value(ps, "OLDPWD");
		if (!target)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), NULL);
	}
	else
		target = argv;
	return (target);
}

static int	cd_chdir_error(char *target, char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(target, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(oldpwd);
	return (1);
}

static void	cd_update_env(t_pipex *ps, char *oldpwd, char *newpwd)
{
	if (oldpwd)
		update_env_value(ps, "OLDPWD", oldpwd);
	if (newpwd)
		update_env_value(ps, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
}

int	exec_builtin_cd(t_pipex *ps, t_ast *node_list)
{
	char	*target;
	char	*oldpwd;
	char	*newpwd;

	if (node_list->argv[1] && node_list->argv[2])
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	target = cd_get_target(ps, node_list);
	if (!target)
		return (1);
	oldpwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
		return (cd_chdir_error(target, oldpwd));
	newpwd = getcwd(NULL, 0);
	cd_update_env(ps, oldpwd, newpwd);
	return (0);
}
