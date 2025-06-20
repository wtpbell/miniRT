/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 18:31:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/18 18:01:41 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	has_expected_token_count(const char *type, size_t token_count)
{
	if (ft_strcmp(type, "A") == 0)
		return (token_count == 3);
	else if (ft_strcmp(type, "L") == 0)
		return (token_count == 4);
	else if (ft_strcmp(type, "C") == 0)
		return (token_count >= 4);
	else if (ft_strcmp(type, "sp") == 0 || ft_strcmp(type, "pl") == 0)
		return (token_count == 4 || token_count == 5);
	else if (ft_strcmp(type, "tri") == 0)
		return (token_count == 5 || token_count == 6);
	else if (ft_strcmp(type, "cy") == 0 || ft_strcmp(type, "co") == 0)
		return (token_count == 6 || token_count == 7);
	else if (ft_strncmp(type, "m_", 2) == 0)
		return (token_count > 2);
	return (-1);
}

bool	validate_tokens(const char *first_token, const char *line)
{
	int	cond;

	cond = has_expected_token_count(first_token, token_count_in_str(line));
	if (cond == -1)
		return (print_error(ERR_UNKNOWN_TOKEN, "token", first_token), false);
	else if (cond == 0)
		return (print_error(ERR_TOKEN_COUNT, "token", line), false);
	return (true);
}
