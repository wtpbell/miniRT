/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 15:03:43 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/09 15:07:45 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

size_t	token_count(char **tokens)
{
	size_t	count;

	if (!tokens)
		return (NULL);
	count = 0;
	while (tokens[count])
		count++;
	return (count);
}
