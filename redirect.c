/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 16:15:52 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/27 15:37:27 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_loop(int wfd, const char *delim, int quoted, t_pipex *ps)
{
	char	*line;

	while (1)
	{
		write(2, "heredoc> ", 9);
		line = get_next_line(0);
		if (!line)
			break ;
		if (is_delim_line(line, delim))
		{
			free(line);
			break ;
		}
		line = expand_heredoc_line(line, quoted, ps);
		if (!line)
			break ;
		write_and_cleanup(wfd, line, line, quoted);
	}
}

static void	run_heredoc_child(int wfd, const char *delim, int quoted,
		t_pipex *ps)
{
	int	tty;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	tty = open("/dev/tty", O_RDWR);
	if (tty >= 0)
	{
		dup2(tty, STDIN_FILENO);
		dup2(tty, STDERR_FILENO);
		close(tty);
	}
	heredoc_loop(wfd, delim, quoted, ps);
	close(wfd);
	exit(0);
}

static int	handle_heredoc_status(t_pipex *ps, char *path, pid_t pid)
{
	int	st;
	int	rfd;

	waitpid(pid, &st, 0);
	setup_signal_parents();
	if (WIFSIGNALED(st) && WTERMSIG(st) == SIGINT)
	{
		write(2, "\n", 1);
		ps->last_status = 130;
		unlink(path);
		return (HEREDOC_ABORT);
	}
	if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
	{
		unlink(path);
		return (-1);
	}
	rfd = open(path, O_RDONLY);
	unlink(path);
	return (rfd);
}

int	build_heredoc_fd(t_pipex *ps, const char *delim, int quoted)
{
	char	path[23];
	int		wfd;
	pid_t	pid;

	ft_memcpy(path, "/tmp/minish_hdocXXXXXX", 23);
	wfd = mkstemp(path);
	if (wfd < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(wfd);
		unlink(path);
		return (-1);
	}
	if (pid == 0)
		run_heredoc_child(wfd, delim, quoted, ps);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(wfd);
	return (handle_heredoc_status(ps, path, pid));
}

int	apply_redirs(t_pipex *ps, t_redir *list)
{
	int			r;
	t_redir		*p;

	p = list;
	while (p)
	{
		r = apply_one_redir(ps, p);
		if (r != 0)
			return (r);
		p = p->next;
	}
	return (0);
}
