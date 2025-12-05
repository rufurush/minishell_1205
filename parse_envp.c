/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_envp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 14:26:30 by sbaba             #+#    #+#             */
/*   Updated: 2025/11/29 11:02:17 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env_len(t_env_list *node)
{
	return ((ft_strlen(node->key) + ft_strlen(node->value)));
}

int	count_envp_nodes(t_env_list *first_node)
{
	int			c;
	t_env_list	*node;

	c = 0;
	node = first_node;
	while (node)
	{
		c++;
		node = node->next;
	}
	return (c);
}

int	ft_memcpy_by_offset(char *dst, char *src, int offset)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[offset + i] = src[i];
		i++;
	}
	return (offset + i);
}

char	**get_envp_as_string(t_pipex *ps)
{
	t_env_list	*node;
	char		**array;
	char		*pair;
	int			offset;
	int			array_index;

	node = ps->env_list;
	array = (char **)malloc(sizeof(char *) * (count_envp_nodes(node) + 1));
	if (!array)
		return (NULL);
	array_index = 0;
	while (node)
	{
		pair = (char *)ft_calloc((get_env_len(node) + 2), sizeof(char));
		if (!pair)
			return (free_envp(array), NULL);
		offset = 0;
		offset = ft_memcpy_by_offset(pair, node->key, offset);
		offset = ft_memcpy_by_offset(pair, "=", offset);
		offset = ft_memcpy_by_offset(pair, node->value, offset);
		array[array_index++] = pair;
		node = node->next;
	}
	array[array_index] = NULL;
	return (array);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
