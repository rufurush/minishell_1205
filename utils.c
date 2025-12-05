/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbaba <sbaba@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 13:48:35 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/29 19:52:33 by sbaba            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_dprintf(int fd, char *filename, char *err_msg)
{
	ft_putstr_fd("minishell$ ", fd);
	if (!filename || !*filename)
		return ;
	if (filename)
		ft_putstr_fd((char *)filename, fd);
	ft_putstr_fd(": ", fd);
	if (err_msg)
		ft_putstr_fd((char *)err_msg, fd);
	ft_putstr_fd("\n", fd);
}
