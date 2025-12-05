/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_buildin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:34:00 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/27 15:33:44 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_n_options(t_ast *node_list, int *newline)
{
	int	i;
	int	j;

	i = 1;
	*newline = 1;
	while (node_list->argv[i] && node_list->argv[i][0] == '-'
		&& node_list->argv[i][1] == 'n')
	{
		j = 2;
		while (node_list->argv[i][j] == 'n')
			j++;
		if (node_list->argv[i][j] != '\0')
			break ;
		*newline = 0;
		i++;
	}
	return (i);
}

static void	print_echo_args(t_ast *node_list, int i)
{
	while (node_list->argv[i])
	{
		ft_putstr_fd(node_list->argv[i], STDOUT_FILENO);
		if (node_list->argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
}

int	exec_builtin_echo(t_ast *node_list)
{
	int	i;
	int	newline;

	if (!node_list->argv)
		return (1);
	i = skip_n_options(node_list, &newline);
	print_echo_args(node_list, i);
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
