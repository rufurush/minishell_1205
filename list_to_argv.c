/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_argv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:43:45 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/26 14:53:21 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	arg_list_len(t_arg *list)
{
	size_t	len;
	t_arg	*p;

	len = 0;
	p = list;
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}

static void	free_argv_partial(char **argv, size_t count)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (i < count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	fill_argv_from_list(t_arg *list, char **argv)
{
	size_t	i;
	t_arg	*p;

	i = 0;
	p = list;
	while (p)
	{
		argv[i] = ft_strdup(p->value);
		if (!argv[i])
		{
			free_argv_partial(argv, i);
			return (0);
		}
		i++;
		p = p->next;
	}
	argv[i] = NULL;
	return (1);
}

char	**list_to_argv(t_arg *list)
{
	size_t	len;
	char	**argv;

	len = arg_list_len(list);
	argv = malloc(sizeof(char *) * (len + 1));
	if (!argv)
		return (NULL);
	if (!fill_argv_from_list(list, argv))
		return (NULL);
	return (argv);
}

t_env_list	*env_push_front(t_env_list *head, const char *kv)
{
	t_env_list	*node;
	const char	*eq;

	eq = ft_strchr(kv, '=');
	if (!eq)
		return (head);
	node = (t_env_list *)malloc(sizeof(*node));
	if (!node)
		return (head);
	node->key = ft_substr(kv, 0, (size_t)(eq - kv));
	node->value = ft_strdup(eq + 1);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (head);
	}
	node->next = head;
	return (node);
}
