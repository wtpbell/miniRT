/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bewong <bewong@student.codam.nl>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 15:02:02 by bewong            #+#    #+#             */
/*   Updated: 2025/06/17 18:15:00 by bewong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "color.h"

static bool	parse_cone_height(float *out, const char *str)
{
	t_v2f	height_range;

	height_range = init_v2f(MIN_HEIGHT, MAX_HEIGHT);
	return (parse_float(out, str, height_range, "parse cone height"));
}

static inline void	cone_init(t_obj *obj, t_v2f dm)
{
	obj->type = OBJ_CONE;
	obj->cone = (t_cone){.radius = dm.x * 0.5f, .height = dm.y};
	obj->calc_norm = cone_normal;
	obj->intersect = cone_intersect;
}

bool	parse_cone(char **tokens, t_scene *scene)
{
	t_obj	*obj;
	t_v3f	pos;
	t_v3f	dir;
	t_v3f	color;
	t_v2f	dm;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_diameter(&dm.x, tokens[3])
		|| !parse_cone_height(&dm.y, tokens[4])
		|| !parse_col(&color, tokens[5]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (false);
	init_obj_transform(obj, pos, dir, g_v3f_up);
	init_obj_renderer(obj, color, cone_texcoord);
	cone_init(obj, dm);
	if (!assign_material(obj, &scene->shared_materials, tokens[6]))
		return (free(obj), false);
	init_object_matrices(obj);
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
