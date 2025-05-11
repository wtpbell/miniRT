/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:31:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/11 18:37:17 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	get_expected_token_count(const char *type)
{
	if (ft_strcmp(type, "sp") == 0)
		return (4);
	if (ft_strcmp(type, "pl") == 0)
		return (4);
	if (ft_strcmp(type, "cy") == 0)
		return (6);
	if (ft_strcmp(type, "A") == 0)
		return (3);
	if (ft_strcmp(type, "C") == 0)
		return (4);
	if (ft_strcmp(type, "L") == 0)
		return (4);
	return (0);
}

char *get_first_token(const char *str)
{
	const char	*start;
	const char	*end;
	char		*token;

	while (*str == ' ' || *str == '\t')
		str++;
	start = str;
	while (*str && *str != ' ' && *str != '\t')
		str++;
	end = str;
	token = ft_substr(start, 0, end - start);
	return (token);
}

bool	validate_tokens(const char *first_token, const char *line)
{
	size_t	expected;
	size_t	count;

	expected = get_expected_token_count(first_token);
	if (expected == 0)
	{
		*error() = ERR_UNKNOWN_TOKEN;
		return (false);
	}
	count = count_tokens_in_str(line);
	if (count != expected)
	{
		*error() = ERR_TOKEN_COUNT;
		return (false);
	}
	return (true);
}
