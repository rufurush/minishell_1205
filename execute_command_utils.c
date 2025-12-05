/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:24:36 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/29 11:12:23 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_cmd_args(char **cmd_args, t_pipex *ps)
{
	if (!cmd_args || !cmd_args[0] || !*cmd_args[0])
	{
		ps->last_status = 127;
		return (127);
	}
	return (0);
}

int	handle_no_cmd_path(t_pipex *ps, char **cmd_args)
{
	if (!ps || !cmd_args || !cmd_args[0])
		return (127);
	if (!cmd_args[0])
	{
		ps->last_status = 127;
		return (127);
	}
	if (!ps->err || !cmd_args[0])
	{
		ps->last_status = 127;
		return (127);
	}
	if (ps->err == ERR_ENOENT_PATH)
		ft_dprintf(2, cmd_args[0], "No such file or directory");
	else
		ft_dprintf(2, cmd_args[0], "command not found");
	ps->last_status = 127;
	return (127);
}

int	handle_directory_result(int directory, char **cmd_args, t_pipex *ps)
{
	if (directory > 0)
	{
		ft_dprintf(2, cmd_args[0], "Is a directory");
		ps->last_status = 126;
		return (126);
	}
	if (directory == -1)
	{
		ft_dprintf(2, cmd_args[0], "No such file or directory\n");
		ps->last_status = 127;
		return (127);
	}
	return (0);
}

int	check_exec_permission(char *cmd_path, char **cmd_args, t_pipex *ps)
{
	if (access(cmd_path, X_OK) == -1)
	{
		ft_dprintf(2, cmd_args[0], "Permission denied");
		ps->last_status = 126;
		return (126);
	}
	return (0);
}

int	handle_exec_error(char **cmd_args, t_pipex *ps)
{
	if (errno == ENOEXEC)
	{
		ft_dprintf(2, cmd_args[0], "Exec format error");
		ps->last_status = 126;
		return (126);
	}
	ft_dprintf(2, strerror(errno), cmd_args[0]);
	ps->last_status = 126;
	return (126);
}
