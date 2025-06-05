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
	// obj->r.mat = create_lambertian(color, 0.1f, 16.0f);
	// obj->r.mat = create_dielectric(color, 2.5f, 1.0f);
	obj->r.mat = create_metal(color, 1.0f);
	obj->t.up = (t_v3f){.x = 0, .y = 1, .z = 0};
	obj->pl = (t_pl){};
	obj->type = OBJ_PLANE;
	obj->intersect = plane_intersect;
	obj->calc_norm = plane_normal; 
	init_object_matrices(obj);
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
