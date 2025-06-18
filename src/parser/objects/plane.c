/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:09 by bewong            #+#    #+#             */
/*   Updated: 2025/05/14 12:05:09 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "color.h"

static void	plane_init(t_obj *obj)
{
	obj->pl = (t_pl){};
	obj->type = OBJ_PLANE;
	obj->intersect = plane_intersect;
	obj->calc_norm = plane_normal;
}

bool	parse_plane(char **tokens, t_scene *scene)
{
	t_obj	*obj;
	t_v3f	pos;
	t_v3f	dir;
	t_v3f	color;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_col(&color, tokens[3]))
		return (false);

	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (false);
	init_obj_transform(obj, pos, dir, g_v3f_up);
	init_obj_renderer(obj, color, plane_texcoord);
	plane_init(obj);
	if (!assign_material(obj, &scene->shared_materials, tokens[4]))
		return (free(obj), false);
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
