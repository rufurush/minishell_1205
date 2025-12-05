/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:29:26 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:04:06 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metachar(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (1);
	else
		return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	else
		return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	else
		return (0);
}

t_token_type	classify_tokens(void)
{
	return (WORD);
}
