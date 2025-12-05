/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:12:23 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:20:11 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_all(char **result, int idx)
{
	int	i;

	i = 0;
	while (i < idx)
		free(result[i++]);
	free(result);
}

static char	*alloc_word(const char *s, int start, int end)
{
	char	*word;
	int		j;

	word = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	j = 0;
	while (start < end)
		word[j++] = s[start++];
	word[j] = '\0';
	return (word);
}

static int	ft_splits_count(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			count++;
			while (s[i] != '\0' && s[i] != c)
				i++;
		}
	}
	return (count);
}

static int	ft_split_fill(char **result, const char *s, char c, int split)
{
	int	i;
	int	index;
	int	start;

	i = 0;
	index = 0;
	while (index < split)
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		result[index] = alloc_word(s, start, i);
		if (!result[index])
			return (free_all(result, index), 0);
		index++;
	}
	return (1);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		split;

	if (!s)
		return (NULL);
	split = ft_splits_count(s, c);
	result = (char **)malloc(sizeof(char *) * (split + 1));
	if (!result)
		return (NULL);
	if (!ft_split_fill(result, s, c, split))
		return (NULL);
	result[split] = NULL;
	return (result);
}
