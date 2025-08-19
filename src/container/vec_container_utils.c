/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vec_container_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 11:08:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/08/13 09:47:30 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "container.h"

void	*vector_get(t_vector *vec, int i)
{
	if (i < 0 || i >= vec->size)
		return (NULL);
	return (vec->items[i]);
}

void	vector_set(t_vector *vec, int i, void *item)
{
	if (i < 0 || i >= vec->size)
		return ;
	vec->items[i] = item;
}

size_t	vector_size(t_vector *vec)
{
	if (!vec)
		return (0);
	return ((size_t)vec->size);
}

void	*vector_find(t_vector *vec, void *ctx,
	int (*fn)(int i, void *item, void *ctx))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		if (fn(i, vec->items[i], ctx) == 0)
			return (*(vec->items + i));
		++i;
	}
	return (NULL);
}
