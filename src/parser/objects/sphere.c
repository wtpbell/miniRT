/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   sphere.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bewong <bewong@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:24:01 by bewong        #+#    #+#                 */
/*   Updated: 2025/05/15 16:49:46 by bewong        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	parse_diameter(float *out, const char *str)
{
	t_v2f	radius_range;

	radius_range = init_v2f(MIN_RADIUS, MAX_RADIUS);
	return (parse_and_validate_float(out, str, radius_range, "parse diameter"));
}

static t_sphere	*create_sphere(t_v3f pos, float diameter, t_col32 color)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (NULL);
	sphere->t.pos = pos;
	sphere->radius = diameter / 2.0f;
	sphere->r.col = color;
	sphere->t.dir = (t_v3f){{0, 0, 0}};
	return (sphere);
}

bool	parse_sphere(char **tokens, t_scene *scene)
{
	t_sphere	*sphere;
	t_v3f		pos;
	float		diameter;
	t_col32		color;
	t_object	*obj;

	if (!parse_v3f(&pos, tokens[1]) || !parse_diameter(&diameter, tokens[2])
		|| !parse_col(&color, tokens[3]))
		return (false);
	sphere = create_sphere(pos, diameter, color);
	if (!sphere)
		return (false);
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (free(sphere), false);
	obj->obj = sphere;
	obj->ray_intersect = NULL;
	if (!vector_add(&scene->objects, obj))
		return (free(sphere), free(obj), false);
	return (true);
}
