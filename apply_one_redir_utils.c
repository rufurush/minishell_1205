/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_one_redir_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:01:50 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:53:09 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redir(t_redir *node)
{
	int	fd;

	fd = open(node->filename, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, node->filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, 0) == -1)
	{
		perror("dup2 file");
		return (1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir(t_redir *node)
{
	int	fd;

	fd = open(node->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, node->filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("open file");
		exit(1);
	}
	close(fd);
	return (0);
}

static int	handle_append_redir(t_redir *node)
{
	int	fd;

	fd = open(node->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, node->filename, strerror(errno));
		return (-1);
	}
	if (dup2(fd, 1) == -1)
	{
		perror("open");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_redir(t_pipex *ps, t_redir *node)
{
	int	fd;

	fd = build_heredoc_fd(ps, node->delim, node->quoted);
	if (fd == HEREDOC_ABORT)
		return (HEREDOC_ABORT);
	if (fd < 0)
		return (-1);
	if (dup2(fd, 0) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_one_redir(t_pipex *ps, t_redir *node)
{
	if (node->kind == INPUT)
		return (handle_input_redir(node));
	else if (node->kind == OUTPUT)
		return (handle_output_redir(node));
	else if (node->kind == APPEND)
		return (handle_append_redir(node));
	else if (node->kind == HEREDOC)
		return (handle_heredoc_redir(ps, node));
	return (0);
}
