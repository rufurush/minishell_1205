/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 12:23:18 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 18:00:29 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_to_backup(int fd, char *backup)
{
	char	*buf;
	ssize_t	bytes_read;
	char	*tmp;

	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !newline_or_not(backup))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free(buf), free(backup), backup = NULL, NULL);
		if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		tmp = ft_strjoin_g(backup, buf);
		if (!tmp)
			return (free(buf), free(backup), backup = NULL, NULL);
		free(backup);
		backup = tmp;
	}
	free(buf);
	return (backup);
}

char	*process_backup(char **backup)
{
	char	*line;
	char	*tmp;

	if (!*backup || **backup == '\0')
		return (free(*backup), *backup = NULL, NULL);
	if (!newline_or_not(*backup))
	{
		line = ft_strdup(*backup);
		free(*backup);
		*backup = NULL;
		return (line);
	}
	line = extract_line(*backup);
	if (!line)
		return (free(*backup), *backup = NULL, NULL);
	tmp = update_stash(*backup);
	*backup = tmp;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*backup;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	backup = read_to_backup(fd, backup);
	return (process_backup(&backup));
}

char	*update_stash(char *stash)
{
	char	*result;
	size_t	i;
	size_t	start;

	if (!stash)
		return (NULL);
	i = 0;
	start = 0;
	while (stash[start] && stash[start] != '\n')
		start++;
	if (stash[start] == '\n')
		start++;
	else
		return (free(stash), NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(stash + start) + 1));
	if (!result)
		return (NULL);
	while (stash[start])
		result[i++] = stash[start++];
	result[i] = '\0';
	free(stash);
	return (result);
}
