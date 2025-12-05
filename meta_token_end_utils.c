/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_token_end_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:30:12 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/16 16:39:21 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token_type	handle_double_meta(char *line, size_t *index)
{
	if (line[*index] == '<' && line[*index + 1] == '<')
	{
		(*index) += 2;
		return (HEREDOC);
	}
	if (line[*index] == '>' && line[*index + 1] == '>')
	{
		(*index) += 2;
		return (APPEND);
	}
	return (WORD);
}

static t_token_type	handle_single_meta(char *line, size_t *index)
{
	if (line[*index] == '<')
	{
		(*index)++;
		return (INPUT);
	}
	if (line[*index] == '>')
	{
		(*index)++;
		return (OUTPUT);
	}
	if (line[*index] == '|')
	{
		(*index)++;
		return (PIPE);
	}
	return (WORD);
}

t_token_type	classify_meta_tokens(char *line, size_t *index)
{
	t_token_type	type;

	type = handle_double_meta(line, index);
	if (type != WORD)
		return (type);
	return (handle_single_meta(line, index));
}
