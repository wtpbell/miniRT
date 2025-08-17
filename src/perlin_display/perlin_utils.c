/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_util.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 15:32:10 by bewong            #+#    #+#             */
/*   Updated: 2025/08/17 15:32:10 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin_display.h"

void	*ft_memdup(const void *src, size_t n)
{
	void	*dest;

	dest = malloc(n);
	if (!dest)
		return (NULL);
	return (ft_memcpy(dest, src, n));
}

t_ui_element	*find_child_by_type(t_ui_element *parent, t_ui_type type)
{
	t_ui_element	*child;

	if (!parent)
		return (NULL);
	child = parent->first_child;
	while (child)
	{
		if (child->type == type)
			return (child);
		child = child->next_sibling;
	}
	return (NULL);
}

bool	is_key_press(mlx_key_data_t keydata, keys_t key)
{
	return (keydata.key == key && keydata.action == MLX_PRESS);
}
