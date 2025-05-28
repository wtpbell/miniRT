/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 12:05:06 by bewong            #+#    #+#             */
/*   Updated: 2025/05/28 18:09:15 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "color_utils.h"

static bool	parse_height(float *out, const char *str)
{
	t_v2f	height_range;

	height_range = init_v2f(MIN_HEIGHT, MAX_HEIGHT);
	return (parse_and_validate_float(out, str, height_range, "parse height"));
}

bool	parse_cylinder(char **tokens, t_scene *scene)
{
	t_obj	*obj;
	t_v3f	pos;
	t_v3f	dir;
	t_v2f	dm;
	t_col32	color;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_diameter(&dm.x, tokens[3]) || !parse_height(&dm.y, tokens[4])
		|| !parse_col(&color, tokens[5]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (false);
	obj->t.pos = pos;
	obj->t.dir = dir;
	obj->r.mat = create_lambertian(col32_to_v3f(color), 0.95f, 256.0f);
	obj->t.up = (t_v3f){.x = 0, .y = 1, .z = 0};
	obj->r.col = color;
	obj->type = OBJ_CYLINDER;
	obj->calc_norm = cylinder_normal;
	obj->u_shape.cy = (t_cy){.radius = dm.x, .height = dm.y};
	obj->intersect = cylinder_intersect;
	init_object_matrices(obj);
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
