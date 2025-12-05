/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 20:15:15 by rufurush          #+#    #+#             */
/*   Updated: 2025/12/05 13:09:56 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delim_line(char *line, const char *delim)
{
	size_t	dlen;

	dlen = ft_strlen(delim);
	if (!ft_strncmp(line, delim, dlen) && (line[dlen] == '\n'
			|| line[dlen] == '\0'))
		return (1);
	return (0);
}

char	*expand_heredoc_line(char *line, int quoted, t_pipex *ps)
{
	char	*expanded;

	if (quoted)
		return (line);
	expanded = expand_one(line, ps);
	if (!expanded)
		return (NULL);
	return (expanded);
}

void	write_and_cleanup(int wfd, char *line, char *to_write, int quoted)
{
	write(wfd, to_write, ft_strlen(to_write));
	if (line && line != to_write)
		free(line);
	if (!quoted && to_write)
		free(to_write);
}

void	read_one_heredoc_line(int wfd, const char *delim, int quoted,
		t_pipex *ps)
{
	char	*line;
	char	*to_write;

	write(2, "heredoc> ", 9);
	line = get_next_line(0);
	if (!line)
		return ;
	if (is_delim_line(line, delim))
	{
		free(line);
		return ;
	}
	to_write = expand_heredoc_line(line, quoted, ps);
	if (!to_write)
	{
		free(line);
		return ;
	}
	write_and_cleanup(wfd, line, to_write, quoted);
}
