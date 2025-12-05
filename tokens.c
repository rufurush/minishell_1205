/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kotadashirui <kotadashirui@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 14:32:14 by rufurush          #+#    #+#             */
/*   Updated: 2025/11/30 10:47:49 by kotadashiru      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_list	*create_token_node(t_token_type type, char *content, int quoted)
{
	t_token_list	*new_node;

	new_node = malloc(sizeof(*new_node));
	if (!new_node)
	{
		perror("create_token_node");
		return (NULL);
	}
	new_node->type = type;
	new_node->content = content;
	new_node->quoted = quoted;
	new_node->next = NULL;
	return (new_node);
}

t_token_list	*create_new_token(char *line, size_t *index)
{
	t_token_type	type;
	char			*token;
	size_t			start;
	int				quote;

	quote = 0;
	start = *index;
	if (is_metachar(line[*index]))
	{
		type = classify_meta_tokens(line, index);
		token = ft_substr(line, start, *index - start);
		if (!token)
			return (NULL);
		return (create_token_node(type, token, quote));
	}
	else
	{
		token_end(line, index, &quote);
		token = ft_substr(line, start, *index - start);
		if (!token)
			return (NULL);
		return (create_token_node(WORD, token, quote));
	}
}

static void	append_token_node(t_token_list **list_head, t_token_list *new_node)
{
	t_token_list	*last;

	if (!list_head || !new_node)
		return ;
	if (!*list_head)
		*list_head = new_node;
	else
	{
		last = *list_head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
}

t_token_list	*create_token_list(char *line)
{
	t_token_list	*token_list;
	t_token_list	*new_node;
	size_t			index;

	token_list = NULL;
	index = 0;
	while (line[index] != '\0')
	{
		while (line[index] && is_space(line[index]))
			index++;
		if (line[index] == '\0')
			break ;
		new_node = create_new_token(line, &index);
		if (!new_node)
			break ;
		append_token_node(&token_list, new_node);
	}
	return (token_list);
}
