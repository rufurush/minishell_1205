/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_end_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:27:10 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/12/05 13:10:24 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_quote_state(char *line, size_t *index, int *s_quote,
		int *d_quote)
{
	size_t	start;

	*s_quote = 0;
	*d_quote = 0;
	start = *index;
	if (line[start] == '\'')
		*s_quote = 1;
	if (line[start] == '"')
		*d_quote = 1;
	if (*s_quote || *d_quote)
		(*index)++;
}

static void	update_single_quote(char *line, size_t *index, int *s_quote,
		int d_quote)
{
	if (line[*index] == '\'' && d_quote != 1)
	{
		if (*s_quote == 0)
			*s_quote = 1;
		else if (is_space(line[*index + 1]) && (*s_quote != 1)
			&& (line[*index] != '\0'))
			*s_quote = 0;
		else
			*s_quote = 0;
	}
}

static void	update_double_quote(char *line, size_t *index, int s_quote,
		int *d_quote)
{
	if (*index > 0 && line[*index - 1] == '\\' && line[*index] == '"')
		return ;
	if (line[*index] == '"' && s_quote != 1)
	{
		if (*d_quote == 0)
			*d_quote = 1;
		else if (is_space(line[*index + 1]) && (*d_quote == 0))
			*d_quote = 0;
		else
			*d_quote = 0;
	}
}

static int	check_token_boundary(char *line, size_t *index, int s_quote,
		int d_quote)
{
	if (s_quote == 0 && d_quote == 0)
	{
		if (is_space(line[*index]) || is_metachar(line[*index]))
			return (1);
	}
	return (0);
}

void	token_end(char *line, size_t *index, int *quoted)
{
	int	s_quote;
	int	d_quote;

	if (quoted)
		*quoted = 0;
	init_quote_state(line, index, &s_quote, &d_quote);
	if (quoted && (s_quote || d_quote))
		*quoted = 1;
	while (line[*index] != '\0')
	{
		update_single_quote(line, index, &s_quote, d_quote);
		update_double_quote(line, index, s_quote, &d_quote);
		if (s_quote || d_quote)
			*quoted = 1;
		if (check_token_boundary(line, index, s_quote, d_quote))
			return ;
		(*index)++;
	}
}
