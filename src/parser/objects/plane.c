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
	obj->t.pos = pos;
	obj->t.dir = dir;
	obj->r.color = color;
	if (!assign_material(obj, &scene->shared_materials, tokens[4]))
		return (free(obj), false);
	obj->t.up = g_v3f_up;
	obj->pl = (t_pl){};
	obj->type = OBJ_PLANE;
	obj->intersect = plane_intersect;
	obj->calc_norm = plane_normal;
	init_object_matrices(obj);
	obj->r.get_texcoord = plane_texcoord;
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
