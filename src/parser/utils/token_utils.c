/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:31:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 10:33:07 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	get_expected_token_count(const char *type)
{
	if (ft_strcmp(type, "sp") == 0
		|| ft_strcmp(type, "pl") == 0
		|| ft_strcmp(type, "C") == 0
		|| ft_strcmp(type, "L") == 0)
		return (4);
	if (ft_strcmp(type, "cy") == 0)
		return (6);
	if (ft_strcmp(type, "A") == 0)
		return (3);
	return (0);
}

char	*get_first_token(const char *str)
{
	return (ft_substr(str, 0, ft_strchrnul(str, ' ') - str));
}

bool	validate_tokens(const char *first_token, const char *line)
{
	size_t	expected;
	size_t	count;

	expected = get_expected_token_count(first_token);
	if (expected == 0)
	{
		print_error(ERR_UNKNOWN_TOKEN, "token", first_token);
		return (false);
	}
	count = token_count_in_str(line);
	if (count != expected)
	{
		print_error(ERR_TOKEN_COUNT, "token", line);
		return (false);
	}
	return (true);
}
