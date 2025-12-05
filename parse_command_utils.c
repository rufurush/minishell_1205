/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:46:52 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:13:12 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_word_token(t_ast *cmd, t_token_list **cur, int *seen)
{
	if (!cmd_push_arg_node(cmd, (*cur)->content))
		return (1);
	*cur = (*cur)->next;
	*seen = 1;
	return (0);
}

char	*strip_all_quotes(const char *s)
{
	size_t	i;
	size_t	len;
	char	*res;
	size_t	j;

	len = ft_strlen(s);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			i++;
		else
			res[j++] = s[i++];
	}
	res[j] = '\0';
	return (res);
}

int	handle_heredoc_token(t_ast *cmd, t_token_list **cur, int *seen)
{
	char	*raw;
	char	*delim;
	int		quoted_flag;

	(void)cmd;
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != WORD)
		return (2);
	raw = (*cur)->content;
	quoted_flag = (*cur)->quoted;
	if (quoted_flag)
	{
		delim = strip_all_quotes(raw);
		if (!delim)
			return (2);
	}
	else
		delim = raw;
	append_redir(cmd, make_heredoc_node(delim, quoted_flag));
	if (quoted_flag)
		free(delim);
	*cur = (*cur)->next;
	*seen = 1;
	return (0);
}

int	handle_redir_token(t_ast *cmd, t_token_list **cur, int *seen)
{
	t_token_type	kind;

	kind = (*cur)->type;
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != WORD)
		return (1);
	if (!parse_redirection(cmd, kind, *cur))
		return (1);
	*cur = (*cur)->next;
	*seen = 1;
	return (0);
}

int	handle_other_token(t_ast *cmd, t_token_list **cur)
{
	(void)cmd;
	parse_command_syntax_err((*cur)->type);
	return (1);
}
