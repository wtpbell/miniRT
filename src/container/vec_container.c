/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_container.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:26:09 by jboon             #+#    #+#             */
/*   Updated: 2025/05/14 17:33:53 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "container.h"

bool	vector_init(t_vector *vec, int capacity)
{
	if (capacity <= 0)
		capacity = COL_INIT_CAPACITY;
	vec->items = ft_calloc(capacity + 1, sizeof(void *));
	if (vec->items != NULL)
	{
		vec->size = 0;
		vec->capacity = capacity;
	}
	return (vec->items != NULL);
}

static bool	vector_resize(t_vector *vec, int capacity)
{
	void	**items;
	void	**curr;

	items = vec->items;
	if (!vector_init(vec, capacity))
	{
		vec->items = items;
		return (false);
	}
	curr = items;
	while (*curr != NULL)
	{
		vector_add(vec, *curr);
		++curr;
	}
	return (free(items), true);
}

bool	vector_add(t_vector *vec, void *item)
{
	if (vec->size == vec->capacity
		&& !vector_resize(vec, vec->capacity * 2))
		return (false);
	vec->items[vec->size++] = item;
	return (true);
}

bool	vector_rm(t_vector *vec, int i, void (*del)(void *))
{
	if (i < 0 || i >= vec->size)
		return (false);
	del(vec->items[i]);
	while (i < (vec->size - 1))
	{
		vec->items[i] = vec->items[i + 1];
		++i;
	}
	vec->items[i] = NULL;
	--vec->size;
	if (vec->size > 0 && vec->size == vec->capacity / 4)
		return (vector_resize(vec, vec->capacity / 2));
	return (true);
}

void	vector_free(t_vector *vec, void (*del)(void *))
{
	int	i;

	i = 0;
	while (i < vec->size)
	{
		del(vec->items[i]);
		++i;
	}
	free(vec->items);
	ft_bzero(vec, sizeof(t_vector));
}
