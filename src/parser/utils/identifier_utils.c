/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 18:00:01 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 10:06:22 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	has_duplicate_identifier(const char *type, t_scene *scene)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < scene->objects.size)
	{
		obj = vector_get(&scene->objects, i);
		if (obj && obj->obj)
		{
			if (ft_strcmp(type, "sp") == 0)
				return (true);
		}
		i++;
	}
	return (false);
}
