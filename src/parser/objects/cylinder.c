/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cylinder.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 12:05:06 by bewong        #+#    #+#                 */
/*   Updated: 2025/08/28 16:57:24 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "color.h"

static bool	parse_height(float *out, const char *str)
{
	t_v2f	height_range;

	height_range = init_v2f(MIN_HEIGHT, MAX_HEIGHT);
	return (parse_float(out, str, height_range, "parse height"));
}

static inline void	cylinder_init(t_obj *obj, t_v2f dm)
{
	obj->type = OBJ_CYLINDER;
	obj->cy = (t_cy){.radius = dm.x * 0.5f, .height = dm.y};
	obj->calc_norm = cylinder_normal;
	obj->intersect = cylinder_intersect;
}

bool	parse_cylinder(char **tokens, t_scene *scene)
{
	t_obj	*obj;
	t_v3f	pos;
	t_v3f	dir;
	t_v3f	color;
	t_v2f	dm;

	if (!parse_v3f(&pos, tokens[1]) || !parse_dir(&dir, tokens[2])
		|| !parse_diameter(&dm.x, tokens[3]) || !parse_height(&dm.y, tokens[4])
		|| !parse_col(&color, tokens[5]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (false);
	init_object_transform(obj, pos, dir, g_v3f_up);
	init_object_renderer(obj, color, cylinder_texcoord);
	cylinder_init(obj, dm);
	if (!assign_material(obj, &scene->shared_materials, tokens[6]))
		return (free(obj), false);
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
