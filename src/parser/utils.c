/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 15:03:43 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/09 18:07:50 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	token_count(char **tokens)
{
	size_t	count;

	if (!tokens)
		return (0);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}

void	free_tokens(char **tokens)
{
	(void) tokens;
}
