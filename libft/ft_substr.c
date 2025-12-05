/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rufurush <rufurush@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:16:12 by kotadashiru       #+#    #+#             */
/*   Updated: 2025/11/25 19:22:48 by rufurush         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_substr_len(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	available_len;

	s_len = ft_strlen(s);
	if (len == 0 || s_len <= start)
		return (0);
	available_len = s_len - start;
	if (available_len <= len)
		return (available_len);
	else
		return (len);
}

static char	*ft_store_substr(char const *s, char *result, unsigned int start,
		size_t len)
{
	size_t		substr_len;
	char		*tmp;
	const char	*substr_start;

	substr_len = ft_substr_len(s, start, len);
	substr_start = s + start;
	tmp = result;
	while (substr_len-- > 0)
	{
		*tmp = *substr_start;
		tmp++;
		substr_start++;
	}
	*tmp = '\0';
	return (result);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	substr_len;
	char	*result;

	if (!s)
		return (NULL);
	substr_len = ft_substr_len(s, start, len);
	result = (char *)malloc(sizeof(char) * (substr_len + 1));
	if (!result)
		return (NULL);
	result = ft_store_substr(s, result, start, len);
	return (result);
}

// int main(void) //意地悪テスター
// {
//     char *s = "42Tokyo!!!";

//     printf("test1: \"%s\"\n", ft_substr(s, 0, 2));    // "42"
//     printf("test2: \"%s\"\n", ft_substr(s, 2, 3));    // "Tok"
//     printf("test3: \"%s\"\n", ft_substr(s, 6, 10));   // "0!!！"
//     printf("test4: \"%s\"\n", ft_substr(s, 10, 5));   // ""
//     printf("test5: \"%s\"\n", ft_substr(s, 0, 0));    // ""

//     // 追加の意地悪テスト
//     printf("NULL test: \"%s\"\n", ft_substr(NULL, 0, 5));
//     printf("start out of range: \"%s\"\n", ft_substr(s, 1000, 5));
//     printf("len too big: \"%s\"\n", ft_substr(s, 2, 1000));
//     printf("start 0, len 0: \"%s\"\n", ft_substr(s, 0, 0));
//     printf("empty string: \"%s\"\n", ft_substr("", 0, 0));
// }
