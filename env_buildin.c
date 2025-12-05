/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_buildin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:43:56 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 17:53:44 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_pipex *ps, const char *key)
{
	t_env_list	*node;

	if (!key || !ps)
		return (NULL);
	node = ps->env_list;
	while (node)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0
			&& ft_strlen(node->key) == ft_strlen(key))
			return (node->value);
		node = node->next;
	}
	return (NULL);
}

static int	update_existing_env(t_env_list *node, const char *value)
{
	free(node->value);
	node->value = ft_strdup(value);
	if (!node->value)
		return (1);
	return (0);
}

static int	add_new_env(t_pipex *ps, const char *key, const char *value)
{
	t_env_list	*new;

	new = malloc(sizeof(t_env_list));
	if (!new)
		return (1);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	if (!new->key || !new->value)
	{
		free(new->key);
		free(new->value);
		free(new);
		return (1);
	}
	new->next = ps->env_list;
	ps->env_list = new;
	return (0);
}

int	update_env_value(t_pipex *ps, const char *key, const char *value)
{
	t_env_list	*node;

	if (!ps || !key || !value)
		return (1);
	node = ps->env_list;
	while (node)
	{
		if (ft_strncmp(node->key, key, ft_strlen(key)) == 0
			&& ft_strlen(node->key) == ft_strlen(key))
			return (update_existing_env(node, value));
		node = node->next;
	}
	return (add_new_env(ps, key, value));
}
