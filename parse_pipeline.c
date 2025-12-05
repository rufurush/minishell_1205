/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 15:30:39 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:16:12 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_pipeline_start_error(t_token_list **cur)
{
	ft_putstr_fd("minishell$: syntax error near unexpected token `", 2);
	if (!*cur)
		ft_putstr_fd("newline", 2);
	else if ((*cur)->type == PIPE)
		ft_putstr_fd("|", 2);
	else if ((*cur)->type == INPUT)
		ft_putstr_fd("<", 2);
	else if ((*cur)->type == OUTPUT)
		ft_putstr_fd(">", 2);
	else
		ft_putstr_fd((*cur)->content, 2);
	ft_putstr_fd("'\n", 2);
}

static t_ast	*handle_pipeline_mid_error(t_ast *left)
{
	ft_putstr_fd("minishell$: syntax error near unexpected token `newline'\n",
		2);
	ast_free_a(left);
	return (NULL);
}

t_ast	*parse_pipeline(t_token_list **cur)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(cur);
	if (!left)
		return (print_pipeline_start_error(cur), NULL);
	while (*cur && (*cur)->type == PIPE)
	{
		consume(cur, PIPE);
		right = parse_command(cur);
		if (!right)
			return (handle_pipeline_mid_error(left));
		left = ast_new_pipe(left, right);
	}
	return (left);
}

int	parse_command_loop(t_token_list **cur, t_ast *cmd, int *seen)
{
	int	ret;

	while (*cur && (*cur)->type != PIPE)
	{
		if ((*cur)->type == WORD)
			ret = handle_word_token(cmd, cur, seen);
		else if ((*cur)->type == HEREDOC)
			ret = handle_heredoc_token(cmd, cur, seen);
		else if (is_redir((*cur)->type))
			ret = handle_redir_token(cmd, cur, seen);
		else
			ret = handle_other_token(cmd, cur);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
