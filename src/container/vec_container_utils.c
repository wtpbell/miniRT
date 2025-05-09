/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vector_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/09 11:08:23 by jboon         #+#    #+#                 */
/*   Updated: 2025/05/09 11:12:09 by jboon         ########   odam.nl         */
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
