/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 16:42:57 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:09:26 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipex	init_pipex(char **envp)
{
	t_pipex	ps;
	int		i;

	ft_memset(&ps, 0, sizeof(ps));
	ps.infile_fd = -1;
	ps.outfile_fd = -1;
	ps.last_status = 0;
	ps.status = 0;
	ps.env_list = NULL;
	ps.path_array = NULL;
	ps.no_path_flag = 0;
	if (envp)
	{
		i = 0;
		while (envp[i])
		{
			ps.env_list = env_push_front(ps.env_list, envp[i]);
			i++;
		}
	}
	ms_update_path_array_from_envlist(&ps);
	return (ps);
}

static void	free_path_array(t_pipex *ps)
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

static void	free_env_list(t_pipex *ps)
{
	t_env_list	*cur;
	t_env_list	*next;

	cur = ps->env_list;
	while (cur)
	{
		next = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = next;
	}
	ps->env_list = NULL;
}

void	free_pipex(t_pipex *ps)
{
	if (!ps)
		return ;
	free_path_array(ps);
	free_env_list(ps);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	ps;
	int		ret;

	ps = init_pipex(envp);
	using_history();
	ps.path_array = ms_update_path_array_from_envlist(&ps);
	rl_catch_signals = 0;
	while (1)
	{
		ret = execute(argc, argv, &ps);
		if (ret < 0)
			return (rl_clear_history(), free_pipex(&ps), 1);
		if (ret > 0)
			break ;
	}
	rl_clear_history();
	free_pipex(&ps);
	return (ps.last_status & 0xff);
}
