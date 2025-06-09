/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 13:01:42 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 13:01:42 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	token_count(char **tokens)
{
	size_t	count;

	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

size_t	count_vector_components(const char *str)
{
	size_t	count;

	count = 1;
	while (*str)
	{
		if (*str != ',')
			++count;
		++str;
	}
	return (count);
}
