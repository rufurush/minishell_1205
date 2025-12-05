/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 12:14:35 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:22:54 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_unclosed_quote(const char *line)
{
	int		s_quote;
	int		d_quote;
	size_t	i;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		if (line[i] == '"' && !s_quote)
		{
			if (i > 0 && line[i - 1] == '\\')
			{
				i++;
				continue ;
			}
			d_quote = !d_quote;
		}
		i++;
	}
	return (s_quote || d_quote);
}

int	try_parent_builtin_then_cleanup(t_ast *node_list, t_pipex *ps, char *line,
		int *handled)
{
	*handled = 0;
	if (is_parent_builtin(node_list) == SUCCESS)
	{
		exec_builtin(ps, node_list);
		ast_free_a(node_list);
		free(line);
		*handled = 1;
		return (0);
	}
	return (0);
}

int	run_pipeline_and_cleanup(t_ast *node_list, t_pipex *ps)
{
	int	status;

	status = run_node(ps, node_list);
	ast_free_a(node_list);
	return (status);
}

int	build_nodes_from_line(const char *line, t_ast **out, t_pipex *ps)
{
	t_token_list	*tokens;
	t_token_list	*head;

	*out = NULL;
	if (has_unclosed_quote(line))
		return (ft_putstr_fd("minishell$: syntax error: unclosed quote\n", 2),
			ps->last_status = 258, 1);
	tokens = create_token_list((char *)line);
	if (!tokens)
		return (-1);
	head = tokens;
	*out = parse_pipeline(&tokens);
	if (!*out)
		return (free_token_list(&head), ps->last_status = 258, 1);
	expand_command_line(*out, ps);
	if (!*out)
		return (free_token_list(&head), 1);
	free_token_list(&head);
	return (0);
}

char	*ms_readline(void)
{
	return (readline("minishell$"));
}
