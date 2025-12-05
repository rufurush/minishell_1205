/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:34:43 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:59:47 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	split_kv_alloc_fail(char **out_key, char **out_val)
{
	free(*out_key);
	free(*out_val);
	*out_key = NULL;
	*out_val = NULL;
}

static void	split_kv_with_eq(const char *arg, char *eq, char **out_key,
		char **out_val)
{
	size_t	klen;

	klen = (size_t)(eq - arg);
	*out_key = ft_substr(arg, 0, klen);
	*out_val = ft_strdup(eq + 1);
}

static void	split_kv_without_eq(const char *arg, char **out_key, char **out_val)
{
	*out_key = ft_strdup(arg);
	*out_val = ft_strdup("");
}

int	split_key_value(const char *arg, char **out_key, char **out_val)
{
	char	*eq;

	*out_key = NULL;
	*out_val = NULL;
	if (!arg)
		return (0);
	eq = ft_strchr(arg, '=');
	if (eq)
		split_kv_with_eq(arg, eq, out_key, out_val);
	else
		split_kv_without_eq(arg, out_key, out_val);
	if (!*out_key || !*out_val)
	{
		split_kv_alloc_fail(out_key, out_val);
		return (0);
	}
	return (1);
}

void	print_env_list_simple(t_env_list *head)
{
	t_env_list	*n;

	n = head;
	while (n)
	{
		if (n->key)
		{
			ft_putstr_fd(n->key, STDOUT_FILENO);
			if (n->value)
			{
				ft_putstr_fd("=", STDOUT_FILENO);
				ft_putstr_fd(n->value, STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		n = n->next;
	}
}
