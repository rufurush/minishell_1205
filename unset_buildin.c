/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_buildin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 19:21:05 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:04:36 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_env_node(t_env_list *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

static void	unset_identifier_error(const char *key, int *status)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd((char *)key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	*status = 1;
}

static void	unset_one_key(t_pipex *shell, const char *key, int *status)
{
	t_env_list	*prev;
	t_env_list	*curr;

	if (!is_valid_identifier(key))
	{
		unset_identifier_error(key, status);
		return ;
	}
	prev = NULL;
	curr = shell->env_list;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key)) == 0
			&& ft_strlen(curr->key) == ft_strlen(key))
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env_list = curr->next;
			free_env_node(curr);
			break ;
		}
		prev = curr;
		curr = curr->next;
	}
}

int	exec_builtin_unset(t_pipex *shell, t_ast *node_list)
{
	size_t	i;
	int		status;

	if (!shell || !node_list || !node_list->argv)
		return (1);
	if (!node_list->argv[1])
		return (0);
	status = 0;
	i = 1;
	while (node_list->argv[i])
	{
		unset_one_key(shell, node_list->argv[i], &status);
		i++;
	}
	return (status);
}
