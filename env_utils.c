/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:24:19 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:53:55 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_env(t_pipex *ps)
{
	t_env_list	*curr;

	if (!ps || !ps->env_list)
		return (1);
	curr = ps->env_list;
	while (curr)
	{
		if (curr->value)
		{
			ft_putstr_fd(curr->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(curr->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		curr = curr->next;
	}
	return (0);
}

int	is_valid_identifier(const char *s)
{
	size_t	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	free_path_array(t_pipex *ps)
{
	int	i;

	if (!ps->path_array)
		return ;
	i = 0;
	while (ps->path_array[i])
	{
		free(ps->path_array[i]);
		i++;
	}
	free(ps->path_array);
	ps->path_array = NULL;
}

char	**ms_update_path_array_from_envlist(t_pipex *ps)
{
	char	*path_env;

	if (!ps)
		return (NULL);
	path_env = get_env_value(ps, "PATH");
	if (!path_env || !*path_env)
	{
		ps->no_path_flag = 1;
		free_path_array(ps);
		return (NULL);
	}
	ps->no_path_flag = 0;
	free_path_array(ps);
	ps->path_array = ft_split(path_env, ':');
	if (!ps->path_array)
	{
		write(2, "malloc error\n", 13);
		exit(1);
	}
	return (ps->path_array);
}
