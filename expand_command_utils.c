/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 14:05:10 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:12:06 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_flags(const char *orig, int *has_quote, int *has_dollar)
{
	*has_quote = 0;
	*has_dollar = 0;
	if (ft_strchr(orig, '\'') || ft_strchr(orig, '"'))
		*has_quote = 1;
	if (ft_strchr(orig, '$'))
		*has_dollar = 1;
}

int	should_remove_arg(const char *orig, const char *new_val)
{
	int	has_quote;
	int	has_dollar;

	init_flags(orig, &has_quote, &has_dollar);
	if (!has_quote && has_dollar && new_val[0] == '\0')
		return (1);
	return (0);
}

void	remove_current_arg(t_arg **pp, t_arg **cur,
		char *orig, char *new_val)
{
	t_arg	*next;

	next = (*cur)->next;
	*pp = next;
	free(orig);
	free(new_val);
	free(*cur);
	*cur = next;
}

void	replace_arg_value(t_arg *cur, char *orig, char *new_val)
{
	free(orig);
	cur->value = new_val;
}

int	process_one_arg(t_arg **pp, t_arg **cur, t_pipex *ps, int *removed)
{
	char	*orig;
	char	*new_val;

	orig = (*cur)->value;
	new_val = expand_one(orig, ps);
	if (!new_val)
		return (FAILURE);
	if (should_remove_arg(orig, new_val))
	{
		remove_current_arg(pp, cur, orig, new_val);
		*removed = 1;
		return (SUCCESS);
	}
	replace_arg_value(*cur, orig, new_val);
	*removed = 0;
	return (SUCCESS);
}
