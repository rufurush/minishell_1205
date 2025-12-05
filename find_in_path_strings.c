/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_in_path_strings.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:30:52 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/13 20:13:52 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_executable_regular(const char *full)
{
	struct stat	st;

	if (access(full, X_OK) != 0)
		return (0);
	if (stat(full, &st) != 0)
		return (0);
	if (!S_ISREG(st.st_mode))
		return (0);
	return (1);
}

static char	*join_dir_cmd(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!full)
		return (NULL);
	return (full);
}

static char	*search_in_dirs(char **dirs, const char *cmd)
{
	size_t	i;
	char	*full;

	i = 0;
	full = NULL;
	while (dirs[i])
	{
		full = join_dir_cmd(dirs[i], cmd);
		if (!full)
		{
			i++;
			continue ;
		}
		if (is_executable_regular(full))
			return (full);
		free(full);
		full = NULL;
		i++;
	}
	return (NULL);
}

char	*find_in_path_string(const char *pathstr, const char *cmd)
{
	char	**dirs;
	char	*full;

	dirs = NULL;
	full = NULL;
	if (!pathstr || !*pathstr)
		return (NULL);
	dirs = ft_split(pathstr, ':');
	if (!dirs)
		return (NULL);
	full = search_in_dirs(dirs, cmd);
	if (full)
	{
		free_splits(dirs);
		return (full);
	}
	free_splits(dirs);
	return (NULL);
}
