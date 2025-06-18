/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sphere.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jboon <jboon@student.codam.nl>               +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:24:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/06/18 14:24:55 by jboon         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "material.h"
#include "color.h"

bool	parse_diameter(float *out, const char *str)
{
	t_v2f	radius_range;

	radius_range = init_v2f(MIN_RADIUS, MAX_RADIUS);
	return (parse_float(out, str, radius_range, "parse diameter"));
}

static inline t_sp	create_sphere(float diameter)
{
	return ((t_sp){
		.radius = diameter / 2.0f
	});
}

bool	parse_sphere(char **tokens, t_scene *scene)
{
	t_v3f	pos;
	t_v3f	color;
	float	diameter;
	t_obj	*obj;

	if (!parse_v3f(&pos, tokens[1]) || !parse_diameter(&diameter, tokens[2])
		|| !parse_col(&color, tokens[3]))
		return (false);
	obj = ft_calloc(1, sizeof(t_obj));
	if (!obj)
		return (false);
	obj->t.pos = pos;
	obj->r.color = color;
	if (!assign_material(obj, &scene->shared_materials, tokens[4]))
		return (free(obj), false);
	obj->t.up = g_v3f_up;
	obj->type = OBJ_SPHERE;
	obj->sp = create_sphere(diameter);
	obj->intersect = sphere_intersect;
	obj->calc_norm = sphere_normal;
	init_object_matrices(obj);
	obj->r.get_texcoord = sphere_texcoord;
	if (!vector_add(&scene->objects, obj))
		return (free(obj), false);
	return (true);
}
