/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   identifier_utils.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 18:00:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/12 09:51:37 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	vector_size(t_vector *vec)
{
	return (vec->size);
}

bool	has_duplicate_identifier(const char *type, t_scene *scene)
{
	int			i;
	t_object	*obj;
	t_sphere	*sphere;

	i = 0;
	while (i < vector_size(&scene->objects))
	{
		obj = vector_get(&scene->objects, i);
		if (obj && obj->obj)
		{
			sphere = (t_sphere *)obj->obj;
			if (sphere->type == OBJ_SPHERE && ft_strcmp(type, "sp") == 0)
			{
				*error() = ERR_DUPLICATE;
				return (true);
			}
		}
		i++;
	}
	return (false);
}
