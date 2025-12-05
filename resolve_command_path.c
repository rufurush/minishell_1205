/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:27:58 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:03:04 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*resolve_check_basic(char *cmd, t_pipex *ps)
{
	if (!cmd || !*cmd)
	{
		ps->err = ERR_NOT_FOUND;
		return (NULL);
	}
	if ((cmd[0] == '.' && cmd[1] == '\0') || (cmd[0] == '.' && cmd[1] == '.'
			&& cmd[2] == '\0'))
	{
		ps->err = ERR_NOT_FOUND;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

static char	*resolve_get_pathstr(t_pipex *ps)
{
	char	*pathstr;

	pathstr = ms_getenv_dup(ps, "PATH");
	if (!pathstr || !*pathstr)
	{
		if (pathstr)
			free(pathstr);
		ps->err = ERR_ENOENT_PATH;
		return (NULL);
	}
	return (pathstr);
}

static char	*resolve_search_in_path(char *pathstr, char *cmd, t_pipex *ps)
{
	char	*full;

	full = find_in_path_string(pathstr, cmd);
	free(pathstr);
	if (full)
		return (full);
	ps->err = ERR_NOT_FOUND;
	return (NULL);
}

char	*resolve_command_path(char *cmd, t_pipex *ps)
{
	char	*res;
	char	*pathstr;

	ps->err = ERR_NONE;
	res = resolve_check_basic(cmd, ps);
	if (res || ps->err != ERR_NONE)
		return (res);
	pathstr = resolve_get_pathstr(ps);
	if (!pathstr)
		return (NULL);
	return (resolve_search_in_path(pathstr, cmd, ps));
}
